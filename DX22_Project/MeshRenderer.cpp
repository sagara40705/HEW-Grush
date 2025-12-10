#include "MeshRenderer.h"
#include "MeshFilter.h"
#include "Transform.h"
#include "Camera.h"
#include "Animator.h"
#include "GameObject.h"
#include "GlassObject.h"
#include "GlassShard.h"
#include "DebugLog.h"
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
        break;
    }

    glass = gameObject->GetComponent<GlassObject>();
    if (glass)
    {
        psKind = ShaderList::PS_GLASS;
    }
    shard = gameObject->GetComponent<GlassShard>();
    if (shard)
    {
        vsKind = ShaderList::VS_SHARD;
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

    // ==== フラスタムカリング ====
    const auto& m = tf->GetMatrix();
    Vector3 center{ m._41, m._42, m._43 };  // ワールド位置
    center += mf->boundsOffset;             // MeshFilter 側で決めたオフセット
    const float r = mf->boundsRadius;
    // 半径が 0 以下ならカリングしない（任意）
    if (r > 0.0f)
    {
        // フラスタム（視錐台）外なら描画スキップ
        if (!cam->IsVisibleSphere(center, r))
        {
            return;
        }
    }

    // === WVP セット ===
    WVPMatrices wvp{};
    DirectX::XMFLOAT4X4 fWVP[3];
    DirectX::XMMATRIX world, view, proj; // 各変換行列の格納先 

    world = DirectX::XMLoadFloat4x4(&tf->GetMatrix());
    view = DirectX::XMLoadFloat4x4(&Camera::Main()->GetView());
    proj = DirectX::XMLoadFloat4x4(&Camera::Main()->GetProjection());
    DirectX::XMStoreFloat4x4(&fWVP[0], DirectX::XMMatrixTranspose(world));
    DirectX::XMStoreFloat4x4(&fWVP[1], DirectX::XMMatrixTranspose(view));
    DirectX::XMStoreFloat4x4(&fWVP[2], DirectX::XMMatrixTranspose(proj));
    ShaderList::SetWVP(fWVP);

    switch (mf->GetSource())
    {
    case MeshSource::Primitive: PrimitiveRender(); break;

    case MeshSource::Model:     ModelRender();     break;

    default: break;
    }
}

void MeshRenderer::PrimitiveRender()
{
    if (shard)
    {
        shard->SetGlassParamPS();
        shard->SetShardParamVS();
    }

    MeshBuffer* mesh = mf->GetPrimitive();
    if (!mesh) return;

    // === VS / PS を取得 ===
    VertexShader* vs = ShaderList::GetVS(vsKind);
    PixelShader* ps = ShaderList::GetPS(psKind);
    if (!vs || !ps) return;

    vs->Bind();
    ps->Bind();

    mesh->Draw();
}

void MeshRenderer::ModelRender()
{
    if (glass) glass->SetGlassParamPS();

    Model* model = mf->GetModel();
    if (!model) return;

    //=== ① VS_ANIME のときだけボーン行列を設定 ===
    if (vsKind == ShaderList::VS_ANIME)
    {
        for (unsigned int i = 0; i < model->GetMeshNum(); ++i)
        {
            const Model::Mesh* mesh = model->GetMesh(i);
            if (!mesh) continue;

            // ボーン行列をシェーダーに設定
            DirectX::XMFLOAT4X4 bones[200]; // Model::MAX_BONE を使ってもOK
            for (int boneIdx = 0; boneIdx < mesh->bones.size(); ++boneIdx)
            {
                const Model::Bone& bone = mesh->bones[boneIdx];
                DirectX::XMStoreFloat4x4(
                    &bones[boneIdx],
                    DirectX::XMMatrixTranspose(
                        bone.invOffset * model->GetBoneMatrix(bone.nodeIndex)
                    )
                );
            }

            ShaderList::SetBones(bones);
        }
    }

    //=== VS / PS を取得 ===
    VertexShader* vs = ShaderList::GetVS(vsKind);
    PixelShader* ps = ShaderList::GetPS(psKind);

    if (!vs || !ps) return;

    // ModelRenderer と同じ流れ
    model->SetVertexShader(vs);
    model->SetPixelShader(ps);
    
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
