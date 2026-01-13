#include "UIFrostRenderer.h"
#include "Sprite.h"
#include "Canvas.h"
#include "TextureManager.h"
#include "DirectX.h"
#include <cassert>
using namespace DirectX;

struct SpriteParamCB
{
    XMFLOAT4 offset_size;   // xy=offset(px), zw=size(px)
    XMFLOAT4 uvPos_uvScale; // xy=uvPos,    zw=uvScale
    XMFLOAT4 color;         // RGBA
};


std::unique_ptr<VertexShader>  UIFrostRenderer::vs_ = nullptr;
std::unique_ptr<PixelShader>   UIFrostRenderer::ps_ = nullptr;
Texture* UIFrostRenderer::tex_ = nullptr;


void UIFrostRenderer::Start()
{
	if (!vs_) CreateVS();
	if (!ps_) CreatePS();
    tex_ = TextureManager::Get(TextureTag::UIFrostNoise);
    canvas = Canvas::GetMain();
}

void UIFrostRenderer::OnDestroy()
{
	vs_.reset();
	ps_.reset();
}

void UIFrostRenderer::Render()
{
    if (!vs_ || !ps_ || !tex_ || !canvas) return;
    if (!IsVisible()) return;
    //SetDepthTest(false);

    // 1) WVP を構築
    XMMATRIX W = XMMatrixIdentity();
    XMMATRIX V = XMMatrixIdentity();
    XMMATRIX P = XMLoadFloat4x4(&canvas->GetProjMatrix());
    W = XMMatrixTranspose(W);
    V = XMMatrixTranspose(V);
    P = XMMatrixTranspose(P);
    struct WVP_CB
    {
        XMFLOAT4X4 world;
        XMFLOAT4X4 view;
        XMFLOAT4X4 proj;
    } wvp;
    XMStoreFloat4x4(&wvp.world, W);
    XMStoreFloat4x4(&wvp.view, V);
    XMStoreFloat4x4(&wvp.proj, P);
    vs_->WriteBuffer(0, &wvp);

    // 2) SpriteParam(b1)
    const float screenW = 1920.0f; 
    const float screenH = 1080.0f; 
    SpriteParamCB sp{};
    sp.offset_size = { 0.0f, 0.0f, screenW, screenH };
    sp.uvPos_uvScale = { 0.0f, 0.0f, 1.0f, 1.0f };
    sp.color = { 1.0f, 1.0f, 1.0f, 1.0f };
    vs_->WriteBuffer(1, &sp);

    // 3) UIFrostParam(b0)（PS側）
    timeSec_ += (1.0f / 60.0f); // TODO: 実deltaTimeに
    params_.noise.w = timeSec_;
    ps_->WriteBuffer(0, &params_);

	// 4) シェーダー設定＆テクスチャ設定
    vs_->Bind();
	ps_->Bind();
	ps_->SetTexture(0, tex_);

	// 5) スプライト描画
    Sprite::Draw();
}

void UIFrostRenderer::CreateVS()
{
    const char* code = R"EOT(
struct VS_IN {
    float3 pos : POSITION0;
    float2 uv  : TEXCOORD0;
};

struct VS_OUT {
    float4 pos   : SV_POSITION;
    float2 uv    : TEXCOORD0;
    float4 color : COLOR0;
};

// 既存の SetWVP と同じ cbuffer(b0) を利用
cbuffer WVP : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
    // world/view/proj だけ使う。残りの要素があっても無視される前提
};

// Sprite 専用パラメータ（b1）
cbuffer SpriteParam : register(b1)
{
    float4 offset_size;   // xy = offset(px), zw = size(px)
    float4 uvPos_uvScale; // xy = uvPos,    zw = uvScale
    float4 color;         // RGBA
};

VS_OUT main(VS_IN vin)
{
    VS_OUT vout;

    // 頂点位置（ローカル [-0.5,0.5] など）に size を掛けて offset を足す
    float2 p = vin.pos.xy * offset_size.zw + offset_size.xy;

    float4 pos = float4(p, 0.0f, 1.0f);
    pos = mul(pos, world);
    pos = mul(pos, view);
    pos = mul(pos, proj);
    vout.pos = pos;

    // UV は uvScale / uvPos で調整
    float2 uv = vin.uv * uvPos_uvScale.zw + uvPos_uvScale.xy;
    vout.uv = uv;

    vout.color = color;
    return vout;
}
)EOT";
	vs_ = std::make_unique<VertexShader>();
    vs_->Compile(code);
}

void UIFrostRenderer::CreatePS()
{
    const char* code = R"EOT(
struct PS_IN {
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD0;
};

cbuffer UIFrostParam : register(b0)
{
    float4 tint_opacity; // rgb=tint, a=opacity(0..1)
    float4 vignette;     // x=strength(0..1), y=radius(0..1), z=softness(0..1), w=unused
    float4 noise;        // x=strength(0..1), y=tileScale, z=speed, w=timeSec
    float4 chroma;       // x=amount(0..1), yzw unused
};

Texture2D noiseTex : register(t0);
SamplerState samp0 : register(s0); // エンジンの Sampler(slot0) を使用（今はWRAP想定）

float3 ApplyVignette(float3 col, float2 uv)
{
    float2 d = uv - 0.5f;
    float r = length(d) * 2.0f; // 0..~1.4
    float radius   = saturate(vignette.y);
    float softness = max(1e-3, vignette.z);
    float v = smoothstep(radius, radius + softness, r);      // 外側ほど1
    float k = 1.0f - v * saturate(vignette.x);               // 端ほど暗く
    return col * k;
}

float4 main(PS_IN pin) : SV_TARGET
{
    float2 uv = pin.uv;

    // ---- 0) ベース色（暗幕＋色味） ----
    float opacity = saturate(tint_opacity.a);
    float3 base = tint_opacity.rgb;

    // ---- 1) 画像ノイズ（すりガラス粒状） ----
    // tileScale は 64 - 512 くらいで調整
    float tileScale = max(1.0f, noise.y);
    float t = noise.w;

    // ゆっくり流す（speed=0なら固定）
    float2 nUV = uv * tileScale + float2(t * noise.z, t * noise.z * 0.37);

    // 0..1 -> -0.5..+0.5
    float n = noiseTex.Sample(samp0, nUV).r - 0.5f;
    base += n * noise.x;

    // ---- 2) 疑似色ズレ（背景をサンプルできないので “板感” だけ作る） ----
    float ca = saturate(chroma.x);
    base.r += ca * 0.04f;
    base.b -= ca * 0.04f;

    // ---- 3) ビネット ----
    base = ApplyVignette(base, uv);

    return float4(base, opacity);
}
)EOT";
	ps_ = std::make_unique<PixelShader>();
	ps_->Compile(code);
}
