#pragma once
#include <vector>
#include "Component.h"
#include <cstdint>
#include "Time/Timer.h"

class GameObject;

class OptionUIController : public MonoBehaviour
{
public:
    void SetOptionRoot(GameObject* root);

    // 4 points + modal flag
    void AddItem(GameObject* textNormal,
        GameObject* textSelected,
        GameObject* frameSelected,
        GameObject* framePressed,
        bool isModal);

    void Start() override;
    void Update() override;

    int GetSelectedIndex() const { return m_selected; }

private:
    GameObject* m_optionRoot = nullptr;
    bool m_isOpen = false;

    int m_selected = 0;
    bool m_isPressedVisual = false; //

    std::vector<GameObject*> m_textNormal;
    std::vector<GameObject*> m_textSelected;
    std::vector<GameObject*> m_frameNormal;
    std::vector<GameObject*> m_frameBreak;

    // modal flags (same size as items)
    std::vector<bool> m_isModal;

private:
    void Open();
    void Close();

    void MoveUp();
    void MoveDown();
    void OnEnter();

    void ApplyVisual();

    bool IsModalShowing() const;
private:
	enum class Operation : std::uint8_t
	{
		None,
		ReturnToGame,
        Restart,
		GoToStageSelect,
		GoToTitle,
	};
	Operation m_operation = Operation::None;
	static constexpr float kDelayDuration = 0.3f; // seconds
	Timer m_delayTimer;
	bool m_isDelayActive = false;
	void TriggerOperation();
};