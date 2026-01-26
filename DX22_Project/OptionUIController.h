#pragma once
#include <vector>
#include "Component.h"

class GameObject;

class OptionUIController : public MonoBehaviour
{
public:
    void SetOptionRoot(GameObject* root);

    // 1項目を4点セットで登録（pressedFrameはnullptr可）
    void AddItem(GameObject* textNormal, GameObject* textSelected,
        GameObject* frameSelected, GameObject* framePressed);

    void Start() override;
    void Update() override;

    int GetSelectedIndex() const { return m_selected; }

private:
    GameObject* m_optionRoot = nullptr;
    bool m_isOpen = false;

    int m_selected = 0;

    // Enterが押された直後の見た目状態を保持
    bool m_isPressedVisual = false;

    std::vector<GameObject*> m_textNormal;
std::vector<GameObject*> m_textSelected;
std::vector<GameObject*> m_frameSelected;
std::vector<GameObject*> m_framePressed; // nullptr可

private:
    void Open();
    void Close();

    void MoveUp();
    void MoveDown();

    void OnEnter();

    void ApplyVisual();
};