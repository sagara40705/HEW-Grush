#pragma once
#include <DirectXMath.h>
#include "Component.h"

// 解像度固定の簡易 Canvas。
// ・スクリーンスペース用の正射影行列を管理
// ・Canvas 用カメラは作らない
// ・当面は 1 つだけ存在する前提（s_main）

class Canvas final : public Component
{
public:
    enum class RenderMode
    {
        ScreenSpaceOverlay,   // 今回はこれだけ
        // 将来: ScreenSpaceCamera, WorldSpace などを追加してもよい
    };

private:
    static Canvas* s_main;     // メイン Canvas（1個だけ想定）

    RenderMode          mode = RenderMode::ScreenSpaceOverlay;
    int                 screenWidth = 1280;
    int                 screenHeight = 720;
    DirectX::XMFLOAT4X4 proj{};       // UI 用射影行列

public:
    Canvas();
    ~Canvas() override = default;

    // メイン Canvas 取得（存在しない場合は nullptr）
    static Canvas* GetMain() { return s_main; }
    const DirectX::XMFLOAT4X4& GetProjMatrix() { return proj; }

    // 解像度設定（ゲーム起動時に 1 回呼ぶイメージ）
    void SetScreenSize(int w, int h);

    int  GetWidth() const { return screenWidth; }
    int  GetHeight() const { return screenHeight; }

    const DirectX::XMFLOAT4X4& GetProjMatrix() const { return proj; }

    RenderMode GetRenderMode() const { return mode; }
    void SetRenderMode(RenderMode m) { mode = m; }

    //=== Component ライフサイクル ===
    void Awake() override;
    void OnDestroy() override;
};
