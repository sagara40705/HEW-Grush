#include "ModelRenderer.h"
#include "ModelComponent.h"
#include "Transform.h"
#include "Camera.h"
#include "GameObject.h"
#include "DirectX.h"
#include <DirectXMath.h>
#include "DebugLog.h"
#include "CameraShake.h"
#include "GlassScript.h"

using namespace DirectX;



struct MatrixCB
{
    XMMATRIX world{};
    XMMATRIX view{};
    XMMATRIX proj{};
};
struct WVPMatrices
{
    XMFLOAT4X4 world;
    XMFLOAT4X4 view;
    XMFLOAT4X4 proj;
};

void ModelRenderer::Start()
{
    t = gameObject->TryGetComponent<Transform>();
    c = Camera::Main();
    mc = gameObject->TryGetComponent<ModelComponent>();


    if (psKind == ShaderList::PS_GLASS)
    {
        gs = gameObject->TryGetComponent<GlassScript>();
    }
    //EnsureShaders();
}

void ModelRenderer::EnsureShaders()
{
    /*
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
    return float4(1, 1, 1, 1);
    return g_tex0.Sample(g_samp, pin.uv);
}
)EOT";
        m_pPS = new PixelShader();
        m_pPS->Compile(PSCode);
    }
    */
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
    //m_pVS->WriteBuffer(0, &cb);

    // ==== 行列を取得して転置 → ShaderList にセット ====
    WVPMatrices wvp{};
    XMFLOAT4X4 wMat = t->GetMatrix();
    CameraShake::Shake(gameObject, wMat);
    const XMFLOAT4X4& vMat = c->GetView();
    const XMFLOAT4X4& pMat = c->GetProjection();
    XMMATRIX W = XMMatrixTranspose(XMLoadFloat4x4(&wMat));
    XMMATRIX V = XMMatrixTranspose(XMLoadFloat4x4(&vMat));
    XMMATRIX P = XMMatrixTranspose(XMLoadFloat4x4(&pMat));
    XMStoreFloat4x4(&wvp.world, W);
    XMStoreFloat4x4(&wvp.view, V);
    XMStoreFloat4x4(&wvp.proj, P);
    ShaderList::SetWVP(reinterpret_cast<XMFLOAT4X4*>(&wvp));

    // ==== カメラ位置のセット（Specular / Toon / Fog 用） ====
    if (psKind == ShaderList::PS_SPECULAR ||
        psKind == ShaderList::PS_TOON ||
        psKind == ShaderList::PS_FOG ||
        psKind == ShaderList::PS_GLASS)
    {
        if (auto* camTf = c->gameObject->TryGetComponent<Transform>())
        {
            XMFLOAT3 camPos = camTf->GetWorldPosition();
            ShaderList::SetCameraPos(camPos);
        }
    }

    if (psKind == ShaderList::PS_GLASS && gs)
    {
        // ガラス固有パラメータ
        DirectX::XMFLOAT4 tint = kTintColor[ToInt(gs->grade)];
        ShaderList::SetGlassParam(tint, 3.0f, 0.5f, 1.0f);
    }
    else
    {
        DirectX::XMFLOAT4 tint = kTintColor[0];
        ShaderList::SetGlassParam(tint, 3.0f, 0.5f, 1.0f);
    }

    if (!gs) psKind = ShaderList::PS_LAMBERT;

    // ==== マテリアルのセット（とりあえず 0 番マテリアルを使用） ====
    //if (model->GetMaterialNum() > 0)
    //{
    //    const Model::Material* mat = model->GetMaterial(0);
    //    if (mat)
    //    {
    //        ShaderList::SetMaterial(*mat);
    //    }
    //}
    // ==== VS / PS を ShaderList から取得してモデルにセット ====
    m_pVS = ShaderList::GetVS(vsKind);
    m_pPS = ShaderList::GetPS(psKind);
    //model->SetVertexShader(vs);
    //model->SetPixelShader(ps);
    m_pVS->Bind();
    m_pPS->Bind();

    // マテリアルのテクスチャ自動設定を活かすため meshNo = -1 で全描画
    //model->Draw(-1);
    // マテリアル別にメッシュを表示 
    for (unsigned int i = 0; i < model->GetMeshNum(); ++i) {
        // モデルのメッシュを取得 
        const Model::Mesh* mesh = model->GetMesh(i);
        // メッシュに割り当てられているマテリアルを取得 
        Model::Material material = *model->GetMaterial(mesh->materialID);
        // シェーダーへマテリアルを設定 
        ShaderList::SetMaterial(material);
        // モデルの描画 
        model->Draw(i);
    }

}

void ModelRenderer::OnDestroy()
{
    //SAFE_DELETE(m_pVS);
    //SAFE_DELETE(m_pPS);
}
