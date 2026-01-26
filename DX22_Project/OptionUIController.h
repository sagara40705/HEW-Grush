#pragma once
#include <vector>
#include "Component.h"

class GameObject;

class OptionUIController : public MonoBehaviour
{
public:
    void SetOptionRoot(GameObject* root);

    // 1€–Ú‚É‚Â‚«3‚Â“o˜^F’Êí / ‘I‘ğ’† / Enter(‰Ÿ‚µ‚½)
    void AddMenuTriple(GameObject* normalGo, GameObject* selectedGo, GameObject* pressedGo);

    void Start() override;
    void Update() override;

    int GetSelectedIndex() const { return m_selected; }

private:
    GameObject* m_optionRoot = nullptr;
    bool m_isOpen = false;

    int m_selected = 0;

    // Enter‚ª‰Ÿ‚³‚ê‚½’¼Œã‚ÌŒ©‚½–Úó‘Ô‚ğ•Û
    bool m_isPressedVisual = false;

    std::vector<GameObject*> m_normals;
    std::vector<GameObject*> m_selecteds;
    std::vector<GameObject*> m_presseds;

private:
    void Open();
    void Close();

    void MoveUp();
    void MoveDown();

    void OnEnter();

    void ApplyVisual();
};