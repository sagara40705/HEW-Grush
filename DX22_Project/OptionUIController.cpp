#include "OptionUIController.h"
#include "GameObject.h"
#include "InputKeyboard.h"
#include "DebugLog.h"
#include "SceneManager.h"
#include "SceneGame.h"
#include "SceneStageSelect.h"
#include "SceneTitle.h"
#include"InputGamepad.h"

void OptionUIController::SetOptionRoot(GameObject* root)
{
    m_optionRoot = root;
}

void OptionUIController::AddItem(GameObject* textNormal,
    GameObject* textSelected,
    GameObject* frameSelected,
    GameObject* framePressed,
    bool isModal)
{
    if (!textNormal) return;
    if (!textSelected) return;
    if (!frameSelected) return;

    m_textNormal.push_back(textNormal);
    m_textSelected.push_back(textSelected);
    m_frameNormal.push_back(frameSelected);
    m_frameBreak.push_back(framePressed); // nullptr ok
    m_isModal.push_back(isModal);
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

	Open(); //最初から表示する
    if (m_isOpen) ApplyVisual();
}

void OptionUIController::Update()
{
    if (!m_optionRoot) return;

    if (m_isDelayActive)
    {
        if (!m_delayTimer.IsRunning()) return;
        if (m_delayTimer.ElapsedRawSeconds() >= kDelayDuration)
        {
            m_isDelayActive = false;
            m_delayTimer.Stop();
            DebugLog::Output("OptionUI: Triggering operation after delay");
            TriggerOperation();
        }
    }

    // modal screen: ESC closes only the modal (do not close option)
    if (IsModalShowing())
    {
        if (InputKeyboard::GetKeyDown(VK_RETURN)
            || InputGamepad::GetButtonDown(XINPUT_GAMEPAD_X))
        {
            m_isPressedVisual = false; // back to normal menu
            ApplyVisual();
        }
        return; // ignore other inputs while modal
    }

    // normal ESC toggles option open/close
    if (InputKeyboard::GetKeyDown(VK_ESCAPE)
        || InputGamepad::GetButtonDown(XINPUT_GAMEPAD_START))
    {
        if (!m_isOpen) Open();
        else           Close();
        return;
    }

    if (!m_isOpen) return;

    // --- Move (Keyboard or Left Stick) ---
    if (InputKeyboard::GetKeyDown(VK_UP)) { MoveUp();   return; }
    if (InputKeyboard::GetKeyDown(VK_DOWN)) { MoveDown(); return; }

    // Left stick: Up/Down (edge trigger)

    const float ly = InputGamepad::GetLStickY();
    constexpr float dead = 0.35f;

    // ニュートラル -> 倒した瞬間だけ反応（連続暴発防止）
    static bool wasNeutral = true;

    const bool nowNeutral = (fabsf(ly) < dead);

    if (wasNeutral && !nowNeutral)
    {
        wasNeutral = false;

        if (ly > 0.0f) { MoveUp();   return; }
        else { MoveDown(); return; }
    }

    if (nowNeutral)
    {
        wasNeutral = true;
    }

    if (InputKeyboard::GetKeyDown(VK_RETURN) ||
        InputGamepad::GetButtonDown(XINPUT_GAMEPAD_START))
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
	// sanity check
    int n = (int)m_textNormal.size();
    if ((int)m_textSelected.size() < n) n = (int)m_textSelected.size();
    if ((int)m_frameNormal.size() < n)  n = (int)m_frameNormal.size();
    if ((int)m_frameBreak.size() < n)   n = (int)m_frameBreak.size();
    if ((int)m_isModal.size() < n)      n = (int)m_isModal.size();
    if (n <= 0) return;

    const bool modal = IsModalShowing();

    for (int i = 0; i < n; ++i)
    {
        GameObject* tn = m_textNormal[i];
        GameObject* ts = m_textSelected[i];
        GameObject* fn = m_frameNormal[i]; // normal frame
        GameObject* fb = m_frameBreak[i];  // break frame / modal screen

        if (!tn || !ts || !fn) continue;

        const bool isSelected = (i == m_selected);

        if (modal)
        {
            // hide all menu items
            tn->SetActive(false);
            ts->SetActive(false);
            fn->SetActive(false);

            if (fb) fb->SetActive(false);

            // show only selected modal screen image
            if (isSelected && fb)
            {
                fb->SetActive(true);
            }
            continue;
        }

        // normal menu mode
        tn->SetActive(!isSelected);
        ts->SetActive(isSelected);

        fn->SetActive(true);
        if (fb) fb->SetActive(false);

        // enter pressed: swap to break frame (non-modal items)
        if (isSelected && m_isPressedVisual && fb && !m_isModal[i])
        {
            fn->SetActive(false);
            fb->SetActive(true);
        }
    }

	// 選択ごとの処理ログ出力
	if (!m_isPressedVisual) return;

	m_isDelayActive = true;
	m_delayTimer.Start();

    switch (m_selected)
    {
	case 0:     
		m_operation = Operation::ReturnToGame;
        DebugLog::Output("OptionUI: Selected 仕事に戻る");
		break;
	case 1:	
        m_operation = Operation::Restart;
        DebugLog::Output("OptionUI: Selected やり直す");
		break;
	case 2:      
        m_operation = Operation::GoToStageSelect;
        DebugLog::Output("OptionUI: Selected ステージ選択に戻る");
		break;
	case 3:	
        m_operation = Operation::None;
        DebugLog::Output("OptionUI: Selected 操作方法を表示");
		break;
	case 4:    
        m_operation = Operation::GoToTitle;
        DebugLog::Output("OptionUI: Selected タイトルへ");
		break;
	default:        DebugLog::Output("OptionUI: Selected 不明な項目");
        break;
    }
}

bool OptionUIController::IsModalShowing() const
{
    const int n = (int)m_isModal.size();
    if (!m_isOpen) return false;
    if (!m_isPressedVisual) return false;
    if (m_selected < 0 || m_selected >= n) return false;
    if (!m_isModal[m_selected]) return false;

    // frameBreak exists -> modal screen
    return (m_frameBreak[m_selected] != nullptr);
}

void OptionUIController::TriggerOperation()
{
    switch (m_operation)
    {
    case OptionUIController::Operation::None:
        break;
    case OptionUIController::Operation::ReturnToGame:
        SceneManager::PopScene();
        break;
    case OptionUIController::Operation::Restart:
        SceneManager::ChangeSceneAsync<SceneGame>();
        break;
    case OptionUIController::Operation::GoToStageSelect:
        SceneManager::ChangeSceneAsync<SceneStageSelect>();
        break;
    case OptionUIController::Operation::GoToTitle:
        SceneManager::ChangeSceneAsync<SceneTitle>();
        break;
    default:
        break;
    }

	m_operation = Operation::None; // reset
}
