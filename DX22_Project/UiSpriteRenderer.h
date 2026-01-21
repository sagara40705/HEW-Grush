#pragma once
#include <DirectXMath.h>
#include "Component.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Sprite.h"
#include "TextureManager.h"

class Canvas;
class Transform;

// UI 専用スプライト描画コンポーネント。
// ・World の Renderer リストに登録される（Renderer 継承）
// ・Transform の position.x / y を「画面左上からのピクセル座標」として扱う
// ・Canvas の正射影行列を使ってスクリーンスペースに描画する
class UiSpriteRenderer final : public Renderer
{
private:

private:
    Canvas* canvas = nullptr;           // nullptr の場合は Canvas::GetMain() を使う
    TextureTag texTag;
    Texture* texture_ = nullptr;

    Vector2 offset = { 0.f, 0.f };
    Vector2 size = { 100.0f, 100.0f }; // 幅・高さ [px]
    Vector2 uvPos = { 0.0f, 0.0f };
    Vector2 uvScale = { 1.0f, 1.0f };
    DirectX::XMFLOAT4 color = { 1, 1, 1, 1 };

public:
    UiSpriteRenderer(TextureTag tex, const Vector2& o, const Vector2& s, const Vector2& uvP = Vector2::zero(), const Vector2& uvS = Vector2::one())
        : texTag(tex), offset(o), size(s), uvPos(uvP), uvScale(uvS) {}
    void Start() override;
    void OnDestroy() override;

	Vector2 GetOffset() { return offset; }
    Vector2& GetSize() { return size; }

    // Renderer::Render の実装
    void Render() override;

    // 設定系
    void SetCanvas(Canvas* c) { canvas = c; }
    void SetTexture(TextureTag tag);
    void SetOffset(const Vector2& o) { offset = o; }
    void SetSize(const Vector2& s) { size = s; }

    void SetUV(const Vector2& pos, const Vector2& scale)
    {
        uvPos = pos;
        uvScale = scale;
    }

    void SetUVPos(const Vector2& p) { uvPos = p; }
    void SetUVPosX(float x) { uvPos.x = x; }
    void SetUVPosY(float y) { uvPos.y = y; }

    void SetUVScale(const Vector2& s) { uvScale = s; }
    void SetUVScaleX(float x) { uvScale.x = x; }
    void SetUVScaleY(float y) { uvScale.y = y; }

    void SetColor(const DirectX::XMFLOAT4& c) { color = c; }
private:
    Sprite::ParamCB GetSpriteParamCB();
};
