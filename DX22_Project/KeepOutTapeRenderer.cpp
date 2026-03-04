#include "KeepOutTapeRenderer.h"

#include "Sprite.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "DirectX.h"
#include "Time/Time.h"

#include <cassert>
using namespace DirectX;

// ---------- static ----------
std::unique_ptr<VertexShader> KeepOutTapeRenderer::s_vs_;
std::unique_ptr<PixelShader>  KeepOutTapeRenderer::s_ps_;
bool                          KeepOutTapeRenderer::s_initialized_ = false;

// ---------- HLSL ----------
// b0: WVP
// b1: TapeParam（SpriteParam互換 + time/speed）
static const char* kKeepOutTapeVS = R"EOT(
struct VS_IN {
    float3 pos : POSITION0;
    float2 uv  : TEXCOORD0;
};

struct VS_OUT {
    float4 pos   : SV_POSITION;
    float2 uv    : TEXCOORD0;
    float4 color : COLOR0;
};

cbuffer WVP : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
};

cbuffer TapeParam : register(b1)
{
    float4 offset_size;     // xy=offset, zw=size
    float4 uvPos_uvScale;   // xy=uvPos,  zw=uvScale
    float4 color;           // RGBA
    float4 time_speed;      // x=time(sec), y=speed(UV/sec)
};

VS_OUT main(VS_IN vin)
{
    VS_OUT o;

    // Quad(0..1) を中心基準にしてサイズ/オフセット適用（ローカル平面上）
    float2 local = (vin.pos.xy - float2(0.5f, 0.5f)) * offset_size.zw + offset_size.xy;

    // Transform の world 行列に乗せる
    float4 wpos = mul(float4(local.xy, 0.0f, 1.0f), world);
    float4 vpos = mul(wpos, view);
    o.pos = mul(vpos, proj);

    // UV: base + scale + scroll
    float2 uv = vin.uv * uvPos_uvScale.zw + uvPos_uvScale.xy;
    uv.x += time_speed.x * time_speed.y; // 横方向に流す
    o.uv = uv;

    o.color = color;
    return o;
}
)EOT";

static const char* kKeepOutTapePS = R"EOT(
struct PS_IN {
    float4 pos   : SV_POSITION;
    float2 uv    : TEXCOORD0;
    float4 color : COLOR0;
};

Texture2D tex     : register(t0);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
    // U方向だけ繰り返し（SamplerがClampでも動くように frac）
    float2 uv = pin.uv;
    uv.x = frac(uv.x);

    float4 t = tex.Sample(samp, uv);
    return t * pin.color;
}
)EOT";

// ---------- impl ----------
void KeepOutTapeRenderer::EnsureShader()
{
    if (s_initialized_) return;

    s_vs_ = std::make_unique<VertexShader>();
    s_ps_ = std::make_unique<PixelShader>();

    s_vs_->Compile(kKeepOutTapeVS);
    s_ps_->Compile(kKeepOutTapePS);

    s_initialized_ = true;
}

void KeepOutTapeRenderer::Start()
{
    EnsureShader();

    tf_ = gameObject->GetComponent<Transform>();
    cam_ = Camera::Main();
    texture_ = TextureManager::Get(texTag_);

    assert(tf_ && cam_ && "KeepOutTapeRenderer: tf_/cam_ is nullptr");

	scrollSpeedU_ = 0.25f; // デフォルト値
}

void KeepOutTapeRenderer::OnDestroy()
{
    tf_ = nullptr;
    cam_ = nullptr;
    texture_ = nullptr;
}

void KeepOutTapeRenderer::SetTextureTag(TextureTag tag)
{
    texTag_ = tag;
    texture_ = TextureManager::Get(texTag_);
}

void KeepOutTapeRenderer::Render()
{
    if (!tf_ || !cam_) return;
    if (!texture_) return;
    if (!s_vs_ || !s_ps_) return;

    // 1) WVP (b0)
    XMFLOAT4X4 fWVP[3];
    XMMATRIX W = XMLoadFloat4x4(&tf_->GetMatrix());
    XMMATRIX V = XMLoadFloat4x4(&cam_->GetView());
    XMMATRIX P = XMLoadFloat4x4(&cam_->GetProjection());
    XMStoreFloat4x4(&fWVP[0], XMMatrixTranspose(W));
    XMStoreFloat4x4(&fWVP[1], XMMatrixTranspose(V));
    XMStoreFloat4x4(&fWVP[2], XMMatrixTranspose(P));
    s_vs_->WriteBuffer(0, &fWVP);

    // 2) TapeParam (b1)
    float t = 0.0f;
    if (useUnscaledTime_)
    {
        unscaledAccum_ += Time::UnscaledDeltaTime();
        t = unscaledAccum_;
    }
    else
    {
        t = Time::TimeSinceStartup();
    }

    struct TapeParamCB
    {
        XMFLOAT4 offset_size;
        XMFLOAT4 uvPos_uvScale;
        XMFLOAT4 color;
        XMFLOAT4 time_speed;
    } cb;

    cb.offset_size = XMFLOAT4(offset_.x, offset_.y, size_.x, size_.y);
    cb.uvPos_uvScale = XMFLOAT4(uvPos_.x, uvPos_.y, uvScale_.x, uvScale_.y);
    cb.color = color_;
    cb.time_speed = XMFLOAT4(t, scrollSpeedU_, 0.0f, 0.0f);

    s_vs_->WriteBuffer(1, &cb);

    // 3) texture
    s_ps_->SetTexture(0, texture_);

    // 4) bind & draw
    s_vs_->Bind();
    s_ps_->Bind();
    Sprite::Draw();
}