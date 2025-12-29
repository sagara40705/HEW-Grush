#include "BreakUiRenderer.h"

void BreakUiRenderer::Start()
{
    m_idTex = TextureManager::Get(m_idTag);
}

void BreakUiRenderer::Update()
{
    if (!m_isBreaking) return;

    m_time += 1.0f / 60.0f;

    if (m_time >= m_fadeEnd)
    {
        m_isBreaking = false;
        m_time = 0.0f;
    }

    ApplyToPixelShader();
}

void BreakUiRenderer::StartBreak()
{
    m_isBreaking = true;
    m_time = 0.0f;
}

void BreakUiRenderer::ApplyToPixelShader()
{
    if (!m_idTex) return;

    auto* ps = ShaderList::GetPS(ShaderList::PS_SPRITE);
    if (!ps) return;

    CB_BreakUi cb{};
    cb.time = m_time;
    cb.hitCenterUv = m_hitCenterUv;
    cb.explodePower = m_explodePower;
    cb.gravity = m_gravity;
    cb.fadeStart = m_fadeStart;
    cb.fadeEnd = m_fadeEnd;
    cb.crackWidth = m_crackWidth;

    ps->WriteBuffer(0, &cb);        // b0
    ps->SetTexture(1, m_idTex);     // t1ÅiTexture*Åj
}