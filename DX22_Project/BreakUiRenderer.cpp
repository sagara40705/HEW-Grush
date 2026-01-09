// BreakUiRenderer.cpp
#include "BreakUiRenderer.h"

void BreakUiRenderer::Start()
{
    m_crackTex = TextureManager::Get(m_crackTag);
    m_idTex = TextureManager::Get(m_idTag);

    // 初期状態でも b2=0 を入れておく（安全）
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
    if (m_isBreaking)
    {
        // deltaTime が取れない前提なので仮で 60fps 固定
        const float dt = 1.0f / 60.0f;
        m_breakT += dt * m_speed;

        if (m_breakT >= 1.0f)
        {
            m_breakT = 1.0f;
            m_isBreaking = false;
        }
    }

    ApplyToSpritePS();
}

void BreakUiRenderer::ApplyToSpritePS()
{
    auto* ps = ShaderList::GetPS(ShaderList::PS_SPRITE);
    if (!ps) return;

    // t1,t2 を刺す（UiSpriteRenderer は t0 しか触らない想定）
    if (m_crackTex) ps->SetTexture(1, m_crackTex);
    if (m_idTex)    ps->SetTexture(2, m_idTex);

    // b2 に割れ進行度
    CB_BreakParam cb{};
    cb.breakT = m_breakT;
    ps->WriteBuffer(2, &cb); // b2
}
