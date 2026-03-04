#pragma once
#include <memory>
#include "Component.h"
#include "Vector2.h"
#include "Shader.h"
#include "TextureManager.h"

// ------------------------------------------------------------
// KeepOutTapeRenderer
//  - Sprite3DRenderer と同じ「3D空間上Quad描画」流儀で、テープUVを横スクロールさせる専用Renderer
//  - Time.h の経過時間を使って、Shader側で常にスライド
// ------------------------------------------------------------
class KeepOutTapeRenderer final : public Renderer
{
private:
    Transform* tf_ = nullptr;
    Camera* cam_ = nullptr;

    TextureTag texTag_;
    Texture* texture_ = nullptr;

    Vector2 offset_ = { 0.f, 0.f };
    Vector2 size_ = { 1.f, 1.f };
    Vector2 uvPos_ = { 0.f, 0.f };
    Vector2 uvScale_ = { 1.f, 1.f };
    DirectX::XMFLOAT4 color_ = { 1, 1, 1, 1 };

    float scrollSpeedU_ = 0.25f; // UV/sec
    bool  useUnscaledTime_ = false;
    float unscaledAccum_ = 0.0f;

    static std::unique_ptr<VertexShader> s_vs_;
    static std::unique_ptr<PixelShader>  s_ps_;
    static bool                          s_initialized_;

public:
    // tex: TextureManagerのタグ
    // offset/size: Sprite3DRenderer と同じ解釈（Quad(0..1)を中心基準にサイズ＆オフセット適用）
    // uvPos/uvScale: 繰り返し数やオフセット（uvScale.x を伸ばすほど横に繰り返す）
    KeepOutTapeRenderer(
        TextureTag tex,
        const Vector2& offset,
        const Vector2& size)
        : texTag_(tex)
        , offset_(offset)
        , size_(size)
    {}

    void Start() override;
    void OnDestroy() override;
    void Render() override;

public:
    // --- setters (Sprite3DRenderer互換寄り) ---
    void SetOffset(const Vector2& o) { offset_ = o; }
    void SetSize(const Vector2& s) { size_ = s; }

    void SetUV(const Vector2& pos, const Vector2& scale) { uvPos_ = pos; uvScale_ = scale; }
    void SetUVPos(const Vector2& p) { uvPos_ = p; }
    void SetUVScale(const Vector2& s) { uvScale_ = s; }

    void SetColor(const DirectX::XMFLOAT4& c) { color_ = c; }

    void SetScrollSpeedU(float uvPerSec) { scrollSpeedU_ = uvPerSec; }
    void SetUseUnscaledTime(bool v)
    {
        useUnscaledTime_ = v;
        if (v) unscaledAccum_ = 0.0f;
    }

    // 途中でタグを差し替える場合
    void SetTextureTag(TextureTag tag);

private:
    static void EnsureShader();
};