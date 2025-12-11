#include "GlassShardRenderer.h"
#include "PrimitiveMeshManager.h"
#include "ShaderList.h"
#include "Camera.h"
#include "DirectX.h"
#include <cassert>
using namespace DirectX;

GlassShardRenderer* GlassShardRenderer::s_instance_ = nullptr;

//--------------------------------------------------------
// ライフサイクル
//--------------------------------------------------------

void GlassShardRenderer::Start()
{
    assert(!s_instance_ && "GlassShardRenderer は 1 つだけの想定です");
    s_instance_ = this;
}

void GlassShardRenderer::OnDestroy()
{
    if (s_instance_ == this)
    {
        s_instance_ = nullptr;
    }
}

//void GlassShardRenderer::Update()
//{
//    BeginFrame();
//}

void GlassShardRenderer::Render()
{
    DrawAll();
}

//--------------------------------------------------------
// ユーティリティ
//--------------------------------------------------------

void GlassShardRenderer::BeginFrame()
{
    if (s_instance_)
    {
        s_instance_->list_.clear();
    }
}

void GlassShardRenderer::Push(const ShardDrawData& d)
{
    if (!s_instance_) return;
    s_instance_->list_.push_back(d);
}

//--------------------------------------------------------
// 描画本体
//--------------------------------------------------------

void GlassShardRenderer::DrawAll()
{
    if (list_.empty())
    {
        return;
    }

    // 必要なリソース取得
    auto* vs = ShaderList::GetVS(ShaderList::VS_SHARD);
    auto* ps = ShaderList::GetPS(ShaderList::PS_GLASS);
    if (!vs || !ps)
    {
        return;
    }

    auto* cam = Camera::Main();
    if (!cam)
    {
        return;
    }

    MeshBuffer* mesh = PrimitiveMeshManager::Get(PrimitiveMeshTag::GlassShard);
    if (!mesh)
    {
        return;
    }

    // 共通のview・projを生成
    DirectX::XMMATRIX view, proj;
    view = DirectX::XMLoadFloat4x4(&Camera::Main()->GetView());
    proj = DirectX::XMLoadFloat4x4(&Camera::Main()->GetProjection());
   

    // シェーダバインド
    vs->Bind();
    ps->Bind();

    // NOTE:
    //   今は 1 つずつ Draw() しています（インスタンシング前段階）。
    //   とりあえず「一括で管理してまとめ描き」だけ済ませておき、
    //   後でインスタンス VB を追加して DrawIndexedInstanced に置き換えれば OK です。
    for (auto& d : list_)
    {
        // VS b0 : 行列・転置して書き込み
        DirectX::XMFLOAT4X4 fWVP[3];
        DirectX::XMStoreFloat4x4(&fWVP[0], DirectX::XMMatrixTranspose(d.world));
        DirectX::XMStoreFloat4x4(&fWVP[1], DirectX::XMMatrixTranspose(view));
        DirectX::XMStoreFloat4x4(&fWVP[2], DirectX::XMMatrixTranspose(proj));
        vs->WriteBuffer(0, &fWVP);

        // VS b1 : 破片用ランダムパラメータ
        vs->WriteBuffer(1, &d.randParam);


        // PS : glass の色など（GlassShard::SetGlassParamPS と同じ定義）
        {
            static const XMFLOAT4 Tint[ToIndex(GlassGrade::Count)] =
            {
                { 0.6f, 0.6f, 0.9f, 0.7f },
                { 0.6f, 0.9f, 0.6f, 0.7f },
                { 0.9f, 0.6f, 0.6f, 0.7f },
            };

            ShaderList::SetGlassParam(
                Tint[ToIndex(d.grade)],
                3.0f,   // refractPower
                5.0f,   // specularPower
                5.0f,   // fresnelPower
                0.0f    // extraParam など
            );
        }

        // メッシュ描画
        mesh->Draw();
    }
}
