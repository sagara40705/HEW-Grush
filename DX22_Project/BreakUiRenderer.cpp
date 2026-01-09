// BreakUiRenderer.cpp
#include "BreakUiRenderer.h"

BreakUiRenderer::BreakUiRenderer(TextureTag idTag)
    : m_idTag(idTag)
{
}

void BreakUiRenderer::Start()
{
    m_idTex = TextureManager::Get(m_idTag);
}

void BreakUiRenderer::StartBreak()
{
    m_isBreaking = true;
    m_breakT = 0.0f;
}

void BreakUiRenderer::Update()
{
    if (!m_isBreaking) return;

    m_breakT += 0.02f;
    if (m_breakT > 1.0f)
    {
        m_breakT = 1.0f;
        m_isBreaking = false;
    }
}

void BreakUiRenderer::Render()
{
   /* auto* ps = ShaderList::GetPS(PS_BREAKUI);
    auto* vs = ShaderList::GetVS(VS_SPRITE);
    vs->Bind();
    ps->Bind();
    ...
        DrawSprite();*/
}

void BreakUiRenderer::ApplyToSpritePS()
{
    auto* ps = ShaderList::GetPS(ShaderList::PS_SPRITE);
    if (!ps) return;

    struct CB_BreakParam
    {
        float breakT;
        float pad[3];
    };

    CB_BreakParam cb{};
    cb.breakT = m_isBreaking ? 1.0f : 0.0f; // ‚Ü‚¸‚Í ON/OFF ‚ÅOK

    ps->WriteBuffer(2, &cb); // š b2 ‚É‘—‚é
}
