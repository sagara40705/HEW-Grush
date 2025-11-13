#pragma once
#include <vector>
#include <DirectXMath.h>
#include "Shader.h"
#include "MeshBuffer.h"

const DirectX::XMFLOAT4 kRed = { 1.f, 0.f, 0.f, 1.f };
const DirectX::XMFLOAT4 kGreen = { 0.f, 1.f, 0.f, 1.f };
const DirectX::XMFLOAT4 kBlue = { 0.f, 0.f, 1.f, 1.f };

class BoxCollider;
class GameObject;

class LineManager final
{
public:
    // 準備と破棄
    static void Init();
    static void Term();

    // 行列（毎フレ更新）
    static void SetView(const DirectX::XMFLOAT4X4& view);
    static void SetProjection(const DirectX::XMFLOAT4X4& proj);

    // 直線を登録（Scene::init などで一度だけ呼ぶ想定）
    static void AddLine(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, const DirectX::XMFLOAT4& color);

    static void AddBoxCollider(GameObject* go);
    static void AddBoxCollider(const BoxCollider* bc);

    // 必要ならまとめ登録ヘルパ
    static void AddPolyline(const std::vector<DirectX::XMFLOAT3>& points, const DirectX::XMFLOAT4& color,  bool loop = false);

    // 毎フレ呼ぶ：登録済みの全ラインを描画
    static void Draw();

    // すべて消去（再登録したいとき）
    static void ClearAll();

private:
    struct LineVertex {
        float pos[3];
        float color[4];
    };

    static void ensureShaders();
    static void buildImmutableVB();     // CPUバッファ→不変VBへ確定（初回/変更時のみ）

private:
    // CPU側
    static std::vector<LineVertex> s_cpu; // 2頂点/本で蓄積
    static bool  s_dirty;                 // 変更フラグ

    // GPU側
    static MeshBuffer* s_vb;            // 不変VB（LINELIST）
    static VertexShader* s_vs;
    static PixelShader* s_ps;

    // 行列
    static DirectX::XMFLOAT4X4 s_view;
    static DirectX::XMFLOAT4X4 s_proj;
};
