#include "OptionUIController.h"
#include "GameObject.h"
#include "InputKeyboard.h"

void OptionUIController::SetOptionRoot(GameObject* root)
{
    m_optionRoot = root;
}

void OptionUIController::AddMenuPair(GameObject* normalGo, GameObject* selectedGo)
{
    if (!normalGo) return;
    if (!selectedGo) return;

    m_normals.push_back(normalGo);
    m_selecteds.push_back(selectedGo);
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

    if (m_isOpen) ApplySelection();
}

void OptionUIController::Update()
{
    if (!m_optionRoot) return;

    // Escで開閉
    if (InputKeyboard::GetKeyDown(VK_ESCAPE))
    {
        if (!m_isOpen) Open();
        else           Close();
        return;
    }

    if (!m_isOpen) return;

    // ↑↓で選択
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
}

void OptionUIController::Open()
{
    if (!m_optionRoot) return;

    m_isOpen = true;
    m_optionRoot->SetActive(true);

    ApplySelection();
}

void OptionUIController::Close()
{
    if (!m_optionRoot) return;

    m_isOpen = false;
    m_optionRoot->SetActive(false);
}

void OptionUIController::MoveUp()
{
    const int n = (int)m_normals.size();
    if (n <= 0) return;

    --m_selected;
    if (m_selected < 0) m_selected = n - 1;

    ApplySelection();
}

void OptionUIController::MoveDown()
{
    const int n = (int)m_normals.size();
    if (n <= 0) return;

    ++m_selected;
    if (m_selected >= n) m_selected = 0;

    ApplySelection();
}

void OptionUIController::ApplySelection()
{
    const int n = (int)m_normals.size();
    if (n <= 0) return;

    // 必ず同数であること（安全策）
    const int m = (int)m_selecteds.size();
    const int cnt = (n < m) ? n : m;

    for (int i = 0; i < cnt; ++i)
    {
        if (m_normals[i])   m_normals[i]->SetActive(i != m_selected);
        if (m_selecteds[i]) m_selecteds[i]->SetActive(i == m_selected);
    }
}