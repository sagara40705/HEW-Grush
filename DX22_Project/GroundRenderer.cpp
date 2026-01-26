// GroundRenderer.cpp
#include "GroundRenderer.h"
#include "GameObject.h"
#include "DirectX.h"
#include "Transform.h"
#include "Camera.h"     
#include "TextureManager.h"
#include <cassert>
using namespace DirectX;

// static メンバ定義
std::unique_ptr<VertexShader> GroundRenderer::s_vs_;
std::unique_ptr<PixelShader>  GroundRenderer::s_ps_;
bool                          GroundRenderer::s_initialized_ = false;

//======================================================================
// 地面専用 HLSL
//======================================================================
namespace
{
    // VS: World/View/Proj + タイル UV
    const char* kGroundVSCode = R"EOT(
struct VS_IN
{
    float3 pos    : POSITION;
    float3 normal : NORMAL0;
    float2 uv     : TEXCOORD0;
};

struct VS_OUT
{
    float4 pos    : SV_POSITION;
    float3 normal : NORMAL0;
    float2 uv     : TEXCOORD0;
    float3 wPos   : POSITION0;
};

// b0: 行列
cbuffer WVP : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
};

// b1: 地面専用パラメータ
// uvTiling.xy : X/Z 方向のタイル数
// uvOffset.xy : オフセット（必要なら）
cbuffer GroundParam : register(b1)
{
    float2 uvTiling;
    float2 uvOffset;
};

VS_OUT main(VS_IN vin)
{
    VS_OUT vout;

    float4 w = mul(float4(vin.pos, 1.0f), world);
    vout.wPos = w.xyz;

    float4 v = mul(w, view);
    vout.pos = mul(v, proj);

    vout.normal = normalize(mul(vin.normal, (float3x3)world));

    float2 localXZ = float2(vin.pos.x, vin.pos.z) + float2(0.5f, 0.5f);
    vout.uv = localXZ * uvTiling + uvOffset;

    return vout;
}
)EOT";

    // PS: 単純にテクスチャをサンプル
    const char* kGroundPSCode = R"EOT(
struct PS_IN
{
    float4 pos    : SV_POSITION;
    float3 normal : NORMAL0;
    float2 uv     : TEXCOORD0;
    float3 wPos   : POSITION0;
};

Texture2D    tex  : register(t0);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
    float4 col = tex.Sample(samp, pin.uv);
    return col;
}
)EOT";
}

//======================================================================
// GroundRenderer 本体
//======================================================================

void GroundRenderer::EnsureShader()
{
    if (s_initialized_) return;

    s_vs_ = std::make_unique<VertexShader>();
    s_ps_ = std::make_unique<PixelShader>();

    s_vs_->Compile(kGroundVSCode);
    s_ps_->Compile(kGroundPSCode);

    s_initialized_ = true;
}

void GroundRenderer::OnDestroy()
{
    if (!s_initialized_) return;
    s_initialized_ = false;

    s_vs_.reset();
    s_ps_.reset();
}

void GroundRenderer::Start()
{
    EnsureShader();

    // 同じ GameObject 上の MeshFilter を取得
    meshFilter_ = GetComponent<MeshFilter>();
    // texture_ は外から SetTexture() で設定してもらう前提
    if(!texture_)  texture_ = TextureManager::Get(TextureTag::Game_Ground);
 
    assert(meshFilter_ && "GroundRenderer::Start()でmeshFilter_がnullptrです");
    assert(texture_ && "GroundRenderer::Start()でtexture_がnullptrです");
}

void GroundRenderer::Render()
{
    MeshBuffer* mesh = meshFilter_->GetPrimitive();

    if (!mesh)
        return;

    if (!texture_)
        return;

    //=========================
    // 1) 行列定数バッファ (b0)
    //=========================
    Transform* tf = gameObject->transform;
    Camera* mainCam = Camera::Main();       

    // === WVP セット ===
    XMFLOAT4X4 fWVP[3];
    XMMATRIX world, view, proj; // 各変換行列の格納先 

    world = XMLoadFloat4x4(&tf->GetMatrix());
    view = XMLoadFloat4x4(&Camera::Main()->GetView());
    proj = XMLoadFloat4x4(&Camera::Main()->GetProjection());
    XMStoreFloat4x4(&fWVP[0], XMMatrixTranspose(world));
    XMStoreFloat4x4(&fWVP[1], XMMatrixTranspose(view));
    XMStoreFloat4x4(&fWVP[2], XMMatrixTranspose(proj));

    s_vs_->WriteBuffer(0, &fWVP);  // cbuffer WVP : b0

    //=========================
    // 2) GroundParam (b1)
    //=========================

    // スケールからタイル数を計算
    // Transform 側の API に合わせて調整（例では GetScale()）
    const Vector3 scale = tf->GetScale();

    const float tileX = scale.x * tilingPerUnit_;
    const float tileZ = scale.z * tilingPerUnit_;

    struct GroundParamCB
    {
        XMFLOAT2 uvTiling;
        XMFLOAT2 uvOffset;
    } groundCB{
        XMFLOAT2(tileX, tileZ),
        XMFLOAT2(0.0f, 0.0f)    // 必要ならオフセットを入れる
    };

    s_vs_->WriteBuffer(1, &groundCB); // cbuffer GroundParam : b1

    //=========================
    // 3) テクスチャ設定
    //=========================

    s_ps_->SetTexture(0, texture_);

    //=========================
    // 4) シェーダをバインドして描画
    //=========================

    s_vs_->Bind();
    s_ps_->Bind();

    mesh->Draw();
}


