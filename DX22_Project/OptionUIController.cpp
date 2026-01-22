#include "OptionUIController.h"
#include "GameObject.h"
#include "InputKeyboard.h"

void OptionUIController::SetOptionRoot(GameObject* root)
{
    m_optionRoot = root;
    m_isOpen = optionType::OPTION_Back;
}

void OptionUIController::Start()
{
    if (!m_optionRoot)
    {
        m_isOpen = false;
        return;
    }

    m_isOpen = m_optionRoot->IsActiveInHierarchy();
}

void OptionUIController::Update()
{
    if (!m_optionRoot) {
        m_isOpen = optionType::OPTION_Back;
        return;
    }

    if (InputKeyboard::GetKeyDown(VK_ESCAPE))
    {
        if (!m_isOpen) Open();
        else           Close();
    }
	else if (InputKeyboard::GetKeyDown(VK_UP))
    {
        switch (m_NowPosition)
        {
        case OptionUIController::OPTION_Back:
			m_NowPosition = OptionUIController
                ::OPTION_Title;
            break;
        case OptionUIController::OPTION_Reset:
			m_NowPosition = OptionUIController
				::OPTION_Back;
            break;
        case OptionUIController::OPTION_Stage:
            m_NowPosition = OptionUIController
                ::OPTION_Reset;
            break;
        case OptionUIController::OPTION_Operation:
			m_NowPosition = OptionUIController
				::OPTION_Stage;
            break;
        case OptionUIController::OPTION_Title:
            m_NowPosition = OptionUIController
                ::OPTION_Operation;
            break;
        }
    }
    else if (InputKeyboard::GetKeyDown(VK_DOWN))
    {
        switch (m_NowPosition)
        {
        case OptionUIController::OPTION_Back:
			m_NowPosition = OptionUIController
				::OPTION_Reset;
            break;
        case OptionUIController::OPTION_Reset:
			m_NowPosition = OptionUIController
				::OPTION_Stage;
            break;
        case OptionUIController::OPTION_Stage:
			m_NowPosition = OptionUIController
				::OPTION_Operation;
            break;
        case OptionUIController::OPTION_Operation:
			m_NowPosition = OptionUIController
				::OPTION_Title;
            break;
        case OptionUIController::OPTION_Title:
			m_NowPosition = OptionUIController
				::OPTION_Back;
            break;
        }
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