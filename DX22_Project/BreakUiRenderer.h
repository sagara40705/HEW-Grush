// BreakUiRenderer.h
#pragma once
#include "Component.h"
#include "TextureManager.h"
#include "ShaderList.h"

class BreakUiRenderer : public MonoBehaviour
{
public:
    BreakUiRenderer() = default;

    // B案：コンストラクタでタグを渡す
    explicit BreakUiRenderer(TextureTag idTag);

    void Start() override;
    void Update() override;
    void Render();

    void StartBreak();

private:
    struct CB_BreakParam
    {
        float breakT;
        float pad[3];
    };

    void ApplyToSpritePS();

private:
    TextureTag m_crackTag = TextureTag::Title_Break;    // 例
    TextureTag m_idTag = TextureTag::Title_BreakID;  // 例

    Texture* m_crackTex = nullptr;
    Texture* m_idTex = nullptr;

    bool  m_isBreaking = false;
    float m_breakT = 0.0f;
    float m_speed = 1.0f; // 1秒で0->1にしたいなら 1.0f
};