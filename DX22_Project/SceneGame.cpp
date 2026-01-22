#include "OptionUIController.h"
#include "GameObject.h"
#include "InputKeyboard.h"

void OptionUIController::SetOptionRoot(GameObject* root)
{
    m_optionRoot = root;
}

void OptionUIController::AddMenuItem(GameObject* item)
{
    if (!item) return;
    m_items.push_back(item);
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
    m_result = OptionResult::None;

    // もし最初から開いているなら見た目を整える
    if (m_isOpen) ApplySelectionVisual();
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

    // 閉じている間は上下/決定は受け付けない
    if (!m_isOpen) return;

    // ↑↓で選択切り替え
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

    // Enterで確定（どれが選ばれたかは GetSelectedIndex()）
    if (InputKeyboard::GetKeyDown(VK_RETURN))
    {
        m_result = OptionResult::Selected;

        // ここで閉じるかどうかは好み
        // 閉じたいなら Close(); を呼ぶ
        // Close();
        return;
    }
}

void OptionUIController::Open()
{
    if (!m_optionRoot) return;

    m_isOpen = true;
    m_result = OptionResult::None;
    m_selected = 0;

    m_optionRoot->SetActive(true);
    ApplySelectionVisual();
}

void OptionUIController::Close()
{
    if (!m_optionRoot) return;

    m_isOpen = false;
    m_optionRoot->SetActive(false);
}

void OptionUIController::MoveUp()
{
    if (m_items.empty()) return;

    m_selected--;
    if (m_selected < 0) m_selected = static_cast<int>(m_items.size()) - 1;

    ApplySelectionVisual();
}

void OptionUIController::MoveDown()
{
    if (m_items.empty()) return;

    m_selected++;
    if (m_selected >= static_cast<int>(m_items.size())) m_selected = 0;

    ApplySelectionVisual();
}

void OptionUIController::ApplySelectionVisual()
{
    // 全部表示にする（SetActiveは触らない）
    // 選択中だけアルファ1.0、他はアルファ0.6 みたいにする

    for (int i = 0; i < static_cast<int>(m_items.size()); ++i)
    {
        GameObject* item = m_items[i];
        if (!item) continue;

        // ここ：各メニュー文字の UiSpriteRenderer を探す
        UiSpriteRenderer* spr = item->GetComponent<UiSpriteRenderer>();
        if (!spr) continue;

        if (i == m_selected)
        {
            spr->SetColor(m_colorSelected);
        }
        else
        {
            spr->SetColor(m_colorNormal);
        }
    }
}