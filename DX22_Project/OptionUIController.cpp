#include "OptionUIController.h"
#include "GameObject.h"
#include "InputKeyboard.h"

void OptionUIController::SetOptionRoot(GameObject* root)
{
    m_optionRoot = root;
}

void OptionUIController::AddItem(GameObject* textNormal, GameObject* textSelected,
    GameObject* frameSelected, GameObject* framePressed)
{
    if (!textNormal) return;
    if (!textSelected) return;

    m_textNormal.push_back(textNormal);
    m_textSelected.push_back(textSelected);
    m_frameNormal.push_back(frameSelected);
    m_frameBreak.push_back(framePressed); // nullptr可
}

void OptionUIController::Start()
{
    if (!m_optionRoot)
    {
        m_isOpen = false;
        return;
    }

    m_isOpen = m_optionRoot->IsActiveInHierarchy();
    m_selected = 0;
    m_isPressedVisual = false;

    if (m_isOpen) ApplyVisual();
}

void OptionUIController::Update()
{
    if (!m_optionRoot) return;

    // ESCで開閉（不要なら消してOK）
    if (InputKeyboard::GetKeyDown(VK_ESCAPE))
    {
        if (!m_isOpen) Open();
        else           Close();
        return;
    }

    if (!m_isOpen) return;

    // 上下で選択移動したら「Enter見た目」は解除して通常の選択見た目へ戻す
    if (InputKeyboard::GetKeyDown(VK_UP))
    {
        MoveUp();
        return;
    }
    if (InputKeyboard::GetKeyDown(VK_DOWN))
    {
        MoveDown();
        return;
    }

    // Enterで見た目を「押した状態」にする
    if (InputKeyboard::GetKeyDown(VK_RETURN))
    {
        OnEnter();
        return;
    }
}

void OptionUIController::Open()
{
    if (!m_optionRoot) return;

    m_isOpen = true;
    m_optionRoot->SetActive(true);

    m_isPressedVisual = false;
    ApplyVisual();
}

void OptionUIController::Close()
{
    if (!m_optionRoot) return;

    m_isOpen = false;
    m_optionRoot->SetActive(false);

    m_isPressedVisual = false;
}

void OptionUIController::MoveUp()
{
    const int n = (int)m_textNormal.size();
    if (n <= 0) return;

    --m_selected;
    if (m_selected < 0) m_selected = n - 1;

    m_isPressedVisual = false;
    ApplyVisual();
}

void OptionUIController::MoveDown()
{
    const int n = (int)m_textNormal.size();
    if (n <= 0) return;

    ++m_selected;
    if (m_selected >= n) m_selected = 0;

    m_isPressedVisual = false;
    ApplyVisual();
}

void OptionUIController::OnEnter()
{
    // ここでは「押した見た目に切り替える」だけ
    // 決定処理(シーン遷移等)は SceneGame 側で GetSelectedIndex() を見てやる想定でもOK
    m_isPressedVisual = true;
    ApplyVisual();
}

void OptionUIController::ApplyVisual()
{
    int n = (int)m_textNormal.size();
    if ((int)m_textSelected.size() < n) n = (int)m_textSelected.size();
    if ((int)m_frameNormal.size() < n)  n = (int)m_frameNormal.size();
    if ((int)m_frameBreak.size() < n)   n = (int)m_frameBreak.size();
    if (n <= 0) return;

    for (int i = 0; i < n; ++i)
    {
        GameObject* tn = m_textNormal[i];
        GameObject* ts = m_textSelected[i];
        GameObject* fn = m_frameNormal[i]; // 通常枠
        GameObject* fb = m_frameBreak[i];  // 割れ枠（nullptr可）

        if (!tn || !ts || !fn) continue;

        const bool isSelected = (i == m_selected);

        // 文字：選択中だけ切り替え
        tn->SetActive(!isSelected);
        ts->SetActive(isSelected);

        // 枠：通常は常に通常枠
        fn->SetActive(true);
        if (fb) fb->SetActive(false);

        // Enter中だけ割れ枠に差し替え
        if (isSelected && m_isPressedVisual && fb)
        {
            fn->SetActive(false);
            fb->SetActive(true);
        }
    }
}