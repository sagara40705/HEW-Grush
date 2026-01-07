#include "BreakUiRenderer.h"

void BreakUiRenderer::Start()
{
    m_idTex = TextureManager::Get(m_idTag);
}

void BreakUiRenderer::StartBreak()
{
    m_isBreaking = true;
    m_time = 0.0f;

    // PS_SPRITE を PS_BREAKUI に差し替える
    ShaderList::SetSpritePsOverride(ShaderList::GetPS(ShaderList::PS_BREAKUI));
}

void BreakUiRenderer::Updete()
{
    if (!m_isBreaking) return;

    // とりあえず固定 60fps
    m_time += 1.0f / 60.0f;

    ApplyToPixelShader();

    if (m_time >= m_fadeEnd)
    {
        m_isBreaking = false;
        m_time = 0.0f;

        // 元に戻す
        ShaderList::SetSpritePsOverride(nullptr);
    }
}

void BreakUiRenderer::ApplyToPixelShader()
{
    auto* ps = ShaderList::GetPS(ShaderList::PS_BREAKUI);
    if (!ps) return;

    // ここは「赤くなる確認」だけなら ID テクスチャ不要
    // ただし今後 ID を使うなら ps->SetTexture(1, m_idTex); などを追加

    CB_BreakUi cb{};
    cb.time = m_time;

    // PS_BREAKUI の b0 に書く
    ps->WriteBuffer(0, &cb);
}