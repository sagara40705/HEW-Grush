#pragma once
#include <vector>
#include <cstdint>
#include <cstddef>
#include <DirectXMath.h>
#include "Shader.h"
#include "MeshBuffer.h"

const DirectX::XMFLOAT4 kRed = { 1.f, 0.f, 0.f, 1.f };
const DirectX::XMFLOAT4 kGreen = { 0.f, 1.f, 0.f, 1.f };
const DirectX::XMFLOAT4 kBlue = { 0.f, 0.f, 1.f, 1.f };

class BoxCollider;
class GameObject;

// デバッグライン管理（Gizmos 的なもの）
class LineManager final
{
public:
    struct LineVertex
    {
        DirectX::XMFLOAT3 pos;
        DirectX::XMFLOAT4 color;
    };

    // 用途ごとのラインレイヤー
    enum class LineGroup : std::uint8_t
    {
        General = 0,   // 汎用
        BoxCollider,   // コライダー可視化用
        Count          // 番兵（= 全レイヤー対象）
    };

public:
    // 初期化 / 終了
    static void Init();
    static void Uninit();

    // ビュー・プロジェクション行列の設定
    static void SetViewProj(const DirectX::XMFLOAT4X4& view,
        const DirectX::XMFLOAT4X4& proj);

    // クリア
    static void ClearAll();
    static void Clear(LineGroup group);

    // ライン追加
    static void AddLine(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b, const DirectX::XMFLOAT4& color, LineGroup group = LineGroup::General);

    // 折れ線（points[0]-points[1]-...）を追加
    static void AddPolyline(const std::vector<DirectX::XMFLOAT3>& points, const DirectX::XMFLOAT4& color, LineGroup group = LineGroup::General);

    // BoxCollider の枠線を追加（Transform 付きのワールド OBB を表示）
    static void AddBoxCollider(const BoxCollider* bc, const DirectX::XMFLOAT4& color = kGreen, LineGroup group = LineGroup::BoxCollider);

    static void AddBoxCollider(GameObject* go, const DirectX::XMFLOAT4& color = kGreen, LineGroup group = LineGroup::BoxCollider);

    // 描画（group = Count のときは全レイヤーを描画）
    static void Draw(LineGroup group = LineGroup::Count);

private:
    struct LineBucket
    {
        std::vector<LineVertex> cpu;   // CPU 側頂点バッファ
        MeshBuffer* vb = nullptr;      // GPU 側 VB（D3D11_USAGE_DYNAMIC）
        bool dirty = false;            // CPU→GPU 転送が必要か
        std::uint32_t vbVertexCount = 0; // 現在の VB 頂点数（Write 時のチェック用）
    };

    // レイヤーごとのバケット
    static LineBucket s_buckets[static_cast<std::size_t>(LineGroup::Count)];

    // シェーダ
    static VertexShader* s_vs;
    static PixelShader* s_ps;

    // VP 行列
    static DirectX::XMFLOAT4X4 s_view;
    static DirectX::XMFLOAT4X4 s_proj;

private:
    static void EnsureShaders();
    static void BuildDynamicVB(LineBucket& bucket);

    static void PushLine(LineBucket& bucket,
        const DirectX::XMFLOAT3& a,
        const DirectX::XMFLOAT3& b,
        const DirectX::XMFLOAT4& color);
};

