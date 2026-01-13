// GroundRenderer.hpp
#pragma once

#include <memory>
#include "Component.h"    // Renderer を含む
#include "MeshFilter.h"
#include "Texture.h"
#include "Shader.h"

// 地面専用レンダラー
// ・プリミティブの立方体をスケールして使用
// ・スケールに応じてテクスチャをタイル表示
class GroundRenderer final : public Renderer
{
public:
    void Start() override;
    void Render() override;
    void OnDestroy() override;

    // 地面のテクスチャを設定
    void SetTexture(Texture* tex) noexcept { texture_ = tex; }

    // 1 ワールドユニットあたりのタイル数
    // 例) 1.0f = 1m ごとに 1 回タイル
    void SetTilingPerUnit(float t) noexcept { tilingPerUnit_ = t; }

private:
    void EnsureShader(); // static シェーダ生成

private:
    MeshFilter* meshFilter_ = nullptr;
    Texture* texture_ = nullptr;

    // スケール → タイル枚数変換用
    float tilingPerUnit_ = 0.1f;

    // GroundRenderer 専用シェーダ（全インスタンスで共有）
    static std::unique_ptr<VertexShader> s_vs_;
    static std::unique_ptr<PixelShader>  s_ps_;
    static bool                          s_initialized_;
};
