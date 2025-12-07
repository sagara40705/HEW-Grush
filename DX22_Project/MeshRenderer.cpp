#include "MeshRenderer.h"
#include "MeshFilter.h"
#include "Transform.h"
#include "Camera.h"
#include "Animator.h"
#include "GameObject.h"
#include "GlassObject.h"
#include "GlassShard.h"
using namespace DirectX;

struct WVPMatrices
{
    XMFLOAT4X4 world;
    XMFLOAT4X4 view;
    XMFLOAT4X4 proj;
};

void MeshRenderer::Awake()
{

}

void MeshRenderer::Start()
{
    tf = gameObject->GetComponent<Transform>();
    cam = Camera::Main();
    mf = gameObject->GetComponent<MeshFilter>();
    animator = gameObject->GetComponent<Animator>();

    switch (mf->GetSource())
    {
    case MeshSource::Primitive:
        vsKind = ShaderList::VS_PRIM;
        psKind = ShaderList::PS_PRIM;
        break;

    case MeshSource::Model:
        vsKind = ShaderList::VS_WORLD;
        psKind = ShaderList::PS_LAMBERT;
        if(animator)  vsKind = ShaderList::VS_ANIME;
        break;
    }

    glass = gameObject->GetComponent<GlassObject>();
    shard = gameObject->GetComponent<GlassShard>();
    if (glass || shard)
    {
        psKind = ShaderList::PS_GLASS;
    }

}

void MeshRenderer::OnDestroy()
{
    tf = nullptr;
    cam = nullptr;
    mf = nullptr;
    animator = nullptr;
    glass = nullptr;
}

void MeshRenderer::Render()
{
    if (!(tf && cam && mf)) return;

    SetDepthTest(true);
    SetCullingMode(D3D11_CULL_NONE);

    // === WVP ƒZƒbƒg ===
    WVPMatrices wvp{};
    XMFLOAT4X4 wMat = tf->GetMatrix();
    const XMFLOAT4X4& vMat = cam->GetView();
    const XMFLOAT4X4& pMat = cam->GetProjection();

    XMMATRIX W = XMMatrixTranspose(XMLoadFloat4x4(&wMat));
    XMMATRIX V = XMMatrixTranspose(XMLoadFloat4x4(&vMat));
    XMMATRIX P = XMMatrixTranspose(XMLoadFloat4x4(&pMat));

    XMStoreFloat4x4(&wvp.world, W);
    XMStoreFloat4x4(&wvp.view, V);
    XMStoreFloat4x4(&wvp.proj, P);
    ShaderList::SetWVP(reinterpret_cast<XMFLOAT4X4*>(&wvp));

    switch (mf->GetSource())
    {
    case MeshSource::Primitive: PrimitiveRender(); break;

    case MeshSource::Model:     ModelRender();     break;

    default: break;
    }
}

void MeshRenderer::PrimitiveRender()
{
    MeshBuffer* mesh = mf->GetPrimitive();
    if (!mesh) return;

    if (psKind == ShaderList::PS_GLASS && shard)
    {
        shard->SetGlassParamPS();
    }


    // === VS / PS ‚ðŽæ“¾ ===
    VertexShader* vs = ShaderList::GetVS(vsKind);
    PixelShader* ps = ShaderList::GetPS(psKind);
    if (!vs || !ps) return;

    vs->Bind();
    ps->Bind();

    mesh->Draw();
}

void MeshRenderer::ModelRender()
{
    Model* model = mf->GetModel();
    if (!model) return;


    if (psKind == ShaderList::PS_GLASS && glass)
    {
        glass->SetGlassParamPS();
    }

    //=== VS / PS ‚ðŽæ“¾ ===
    VertexShader* vs = ShaderList::GetVS(vsKind);
    PixelShader* ps = ShaderList::GetPS(psKind);

    if (!vs || !ps) return;

    // ModelRenderer ‚Æ“¯‚¶—¬‚ê
    model->SetVertexShader(vs);
    model->SetPixelShader(ps);
    model->Draw(-1);
  
}
