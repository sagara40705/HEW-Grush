// BreakUiRenderer.cpp
#include "BreakUiRenderer.h"

BreakUiRenderer::BreakUiRenderer(TextureTag idTag)
    : m_idTag(idTag)
{
}

void BreakUiRenderer::Start()
{
    m_idTex = TextureManager::Get(m_idTag);

    // b2 初期化（breakT=0）
    m_breakT = 0.0f;
    ApplyToSpritePS();
}

void BreakUiRenderer::StartBreak()
{
    m_isBreaking = true;
    m_breakT = 0.0f;
}

void BreakUiRenderer::Update()
{
    if (!m_isBreaking) return;

    m_breakT += m_speed * (1.0f / 60.0f);
    if (m_breakT >= 1.0f)
    {
        m_breakT = 1.0f;
        m_isBreaking = false;
    }

    ApplyToSpritePS();
}

void BreakUiRenderer::Render()
{
    auto* ps = ShaderList::GetPS(ShaderList::PS_SPRITE);
    ps->Bind();

    // ここで b2 を毎回セット（テスト）
    struct CB_BreakParam2 { float breakParam[4]; };
    CB_BreakParam2 cb{};
    cb.breakParam[0] = 1.0f; // 強制で赤
    ps->WriteBuffer(2, &cb);
}

void BreakUiRenderer::ApplyToSpritePS()
{
    auto* ps = ShaderList::GetPS(ShaderList::PS_SPRITE);
    if (!ps) return;

    struct CB_BreakParam2
    {
        float breakParam[4]; // x=breakT
    };

    CB_BreakParam2 cb{};
    cb.breakParam[0] = m_breakT;

    ps->WriteBuffer(2, &cb); // b2 に書く
}
