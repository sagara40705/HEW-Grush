#pragma once
#include "TextureManager.h"
#include "ShaderList.h"
#include <DirectXMath.h>

class BreakUiRenderer 
    //public MonoBehaviour
{
public:
    BreakUiRenderer() = default;

    // B案：TextureTag を受け取るコンストラクタ
    explicit BreakUiRenderer(TextureTag idTag)
        : m_idTag(idTag)
    {
    }

    void Start() ;
    void Updete();

    void StartBreak();

private:
    struct CB_BreakUi
    {
        float time;
        DirectX::XMFLOAT2 pad0;
        float dummy0;
        float dummy1;
    };

private:
    void ApplyToPixelShader();

private:
    bool  m_isBreaking = false;
    float m_time = 0.0f;

    TextureTag m_idTag = TextureTag::Title_BreakID;
    Texture* m_idTex = nullptr;

    float m_fadeEnd = 0.90f;
};