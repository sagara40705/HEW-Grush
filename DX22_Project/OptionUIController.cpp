#include "OptionUIController.h"
#include "GameObject.h"
#include "World.h"
#include "Canvas.h"
#include "UiSpriteRenderer.h"
#include"InputKeyboard.h"

void OptionUIController::SetOptionRoot(GameObject* root)
{
    m_optionRoot = root;
}

void OptionUIController::Start()
{
    // 常にActiveな側に付くので、UI Rootの状態を見て初期化
    if (m_optionRoot)
    {
        m_isOpen = m_optionRoot->IsActiveInHierarchy();
    }
    else
    {
        m_isOpen = false;
    }
}

void OptionUIController::Update()
{
    // ここに必ず入る（このコンポーネントは常にActiveなオブジェクトに付けるため）
    if (!m_optionRoot) return;

    if (InputKeyboard::GetKeyDown(VK_ESCAPE))
    {
        if (!m_isOpen) Open();
        else           Close();
    }
}

void OptionUIController::Open()
{
    if (!m_optionRoot) return;
    m_isOpen = true;
    m_optionRoot->SetActive(true);
}

void OptionUIController::Close()
{
    if (!m_optionRoot) return;
    m_isOpen = false;
    m_optionRoot->SetActive(false);
}

bool OptionUIController::IsInThisHierarchy(GameObject* go) const
{
    if (!go) return false;
    if (!m_optionRoot) return false;

    GameObject* cur = go;
    while (cur)
    {
        if (cur == m_optionRoot) return true;
        cur = cur->GetParent();
    }
    return false;
}