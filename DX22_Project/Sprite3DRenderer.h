#pragma once
#include <memory>
#include "Component.h"
#include "Vector2.h"
#include "MeshBuffer.h"
#include "Shader.h"
#include "TextureManager.h"

// 3D用 Sprite描画コンポーネント
// コンストラクタ (使用する画像タグ, world座標, 描画サイズ)
class Sprite3DRenderer final : public Renderer
{
private:
    Transform* tf_ = nullptr;
    Camera* cam_ = nullptr;
    TextureTag texTag;
    Texture* texture_ = nullptr;

    Vector2 offset = { 0.f, 0.f };
    Vector2 size = { 100.0f, 100.0f }; // 幅・高さ [px]
    Vector2 uvPos = { 0.0f, 0.0f };
    Vector2 uvScale = { 1.0f, 1.0f };
    DirectX::XMFLOAT4 color = { 1, 1, 1, 1 };
public:
    Sprite3DRenderer(TextureTag tex, const Vector2& o, const Vector2& s, const Vector2& uvP = Vector2::zero(), const Vector2& uvS = Vector2::one())
        : texTag(tex), offset(o), size(s), uvPos(uvP), uvScale(uvS) {}
    void Start() override;
    void OnDestroy() override;
    void Render() override;

};