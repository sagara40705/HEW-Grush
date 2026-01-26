#include "OptionUIController.h"
#include "GameObject.h"
#include "InputKeyboard.h"

void OptionUIController::SetOptionRoot(GameObject* root)
{
    m_optionRoot = root;
}

void OptionUIController::AddMenuTriple
(GameObject* normalGo, GameObject* selectedGo,
    GameObject* pressedGo)
{
    if (!normalGo) return;
    if (!selectedGo) return;
    if (!pressedGo) return;

    m_normals.push_back(normalGo);
    m_selecteds.push_back(selectedGo);
    m_presseds.push_back(pressedGo);
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
    const int n = (int)m_normals.size();
    if (n <= 0) return;

    --m_selected;
    if (m_selected < 0) m_selected = n - 1;

    m_isPressedVisual = false;
    ApplyVisual();
}

void OptionUIController::MoveDown()
{
    const int n = (int)m_normals.size();
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
    // 念のため、個数がズレていても落ちないようにする
    int n = (int)m_normals.size();
    int s = (int)m_selecteds.size();
    int p = (int)m_presseds.size();

    int cnt = n;
    if (s < cnt) cnt = s;
    if (p < cnt) cnt = p;
    if (cnt <= 0) return;

    for (int i = 0; i < cnt; ++i)
    {
        if (!m_normals[i] || !m_selecteds[i] || !m_presseds[i]) continue;

        // まず全部OFF
        m_normals[i]->SetActive(false);
        m_selecteds[i]->SetActive(false);
        m_presseds[i]->SetActive(false);

        if (i == m_selected)
        {
            // 選択中は Enter状態ならPressed、そうでなければSelected
            if (m_isPressedVisual) m_presseds[i]->SetActive(true);
            else                   m_selecteds[i]->SetActive(true);
        }
        else
        {
            // 非選択はNormal
            m_normals[i]->SetActive(true);
        }
    }
}