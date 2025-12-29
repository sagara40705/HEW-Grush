#pragma once
#include "Component.h"
#include <DirectXMath.h>
#include "TextureManager.h"
#include "ShaderList.h"

class BreakUiRenderer : public Component
{
public:
    explicit BreakUiRenderer(TextureTag idTag)
        : m_idTag(idTag)
    {
    }

    void Start() override;

    // 基底の更新関数名が不明なので、いったん override は付けない
    void Update();

    void StartBreak();

private:
    struct CB_BreakUi
    {
        float time;
        DirectX::XMFLOAT2 hitCenterUv;
        float explodePower;
        float gravity;
        float fadeStart;
        float fadeEnd;
        float crackWidth;
    };

    void ApplyToPixelShader();

private:
    bool  m_isBreaking = false;
    float m_time = 0.0f;

    TextureTag m_idTag = TextureTag::UI_BreakID;

    // ここが重要：Texture* を保持する
    Texture* m_idTex = nullptr;

    DirectX::XMFLOAT2 m_hitCenterUv = { 0.5f, 0.5f };
    float m_explodePower = 0.18f;
    float m_gravity = 0.35f;
    float m_fadeStart = 0.35f;
    float m_fadeEnd = 0.9f;
    float m_crackWidth = 0.015f;
};