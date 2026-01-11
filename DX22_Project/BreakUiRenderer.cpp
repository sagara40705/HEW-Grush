// BreakUiRenderer.cpp
#include "BreakUiRenderer.h"

BreakUiRenderer::BreakUiRenderer(TextureTag idTag)
    : m_idTag(idTag)
{
}

void BreakUiRenderer::Start()
{
    m_idTex = TextureManager::Get(m_idTag);

    m_breakT = 0.0f;
    ApplyToSpritePS();
}

void BreakUiRenderer::StartBreak()
{
    m_isBreaking = true;
    m_breakT = 0.0f;
    ApplyToSpritePS();
}

void BreakUiRenderer::Update()
{
    if (!m_isBreaking) return;

    m_breakT += 1.0f / 60.0f;
    if (m_breakT >= 1.0f)
    {
        m_breakT = 1.0f;
        m_isBreaking = false;
    }

    ApplyToSpritePS();
}

void BreakUiRenderer::Render()
{
    //// UiSpriteRenderer.cpp の Render() 内の、Draw する直前あたり
    //auto* ps = ShaderList::GetPS(ShaderList::PS_SPRITE);
    //if (ps)
    //{
    //    struct CB_BreakParam2
    //    {
    //        float breakParam[4]; // x = breakT
    //    };

    //    CB_BreakParam2 cb{};
    //    cb.breakParam[0] = m_breakT;

    //    ps->WriteBuffer(2, &cb); // b2
    //}
}

void BreakUiRenderer::ApplyToSpritePS()
{
    // ShaderList 側に breakT を保持させる（あなたの方針のまま）
    ShaderList::SetSpriteBreakT(m_breakT);
}
