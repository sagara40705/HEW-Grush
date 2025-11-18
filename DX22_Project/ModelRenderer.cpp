#include "ModelRenderer.h"
#include "ModelComponent.h"
#include "Transform.h"
#include "Camera.h"
#include "Model.h"
#include "GameObject.h"
#include "DirectX.h"
#include <DirectXMath.h>
#include "DebugLog.h"

using namespace DirectX;

struct MatrixCB
{
    XMMATRIX world{};
    XMMATRIX view{};
    XMMATRIX proj{};
};

void ModelRenderer::Start()
{
    t = Parent->TryGetComponent<Transform>();
    c = Camera::Main();
    mc = Parent->TryGetComponent<ModelComponent>();
    EnsureShaders();
}

void ModelRenderer::EnsureShaders()
{
    if (!m_pVS)
    {
        const char* VSCode = R"EOT(
cbuffer MatrixCB : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
};
struct VS_IN  { 
    float3 pos : POSITION; 
    float3 normal : NORMAL0;float2 uv : 
    TEXCOORD0; };
struct VS_OUT { 
    float4 pos : SV_POSITION; 
    float3 normal : NORMAL0;
    float2 uv : TEXCOORD0; };
VS_OUT main(VS_IN vin)
{
    VS_OUT v;
    float4 wpos = mul(float4(vin.pos, 1.0f), world);
    float4 vpos = mul(wpos, view);
    v.pos = mul(vpos, proj);
    v.uv  = vin.uv;
    return v;
}
)EOT";
        m_pVS = new VertexShader();
        m_pVS->Compile(VSCode);
    }
    if (!m_pPS)
    {
        const char* PSCode = R"EOT(
struct PS_IN { 
    float4 pos : SV_POSITION;
    float3 normal : NORMAL0;
    float2 uv : TEXCOORD0;
};
Texture2D    g_tex0 : register(t0);
SamplerState g_samp : register(s0);
float4 main(PS_IN pin) : SV_Target
{
    return float4(1, 0, 0, 1); 
    return g_tex0.Sample(g_samp, pin.uv);
}
)EOT";
        m_pPS = new PixelShader();
        m_pPS->Compile(PSCode);
    }
}

void ModelRenderer::Draw()
{
    if (!(t && c && mc)) return;
    Model* model = mc->Get();
    if (!model) return;

    // 行列CB更新（Transform/Cameraの既存APIを使用）
    MatrixCB cb{};
    cb.world = XMMatrixTranspose(XMLoadFloat4x4(&t->GetMatrix()));
    cb.view = XMMatrixTranspose(XMLoadFloat4x4(&c->GetView()));
    cb.proj = XMMatrixTranspose(XMLoadFloat4x4(&c->GetProjection()));
    m_pVS->WriteBuffer(0, &cb);

    model->SetVertexShader(m_pVS);
    model->SetPixelShader(m_pPS);

    // マテリアルのテクスチャ自動設定を活かすため meshNo = -1 で全描画
    model->Draw(-1);
}

void ModelRenderer::OnDestroy()
{
    SAFE_DELETE(m_pVS);
    SAFE_DELETE(m_pPS);
}
