#pragma once
#include <vector>
#include "Component.h"

class GameObject;

class OptionUIController : public MonoBehaviour
{
public:
    void SetOptionRoot(GameObject* root);

    // 追加：1項目につき「通常」「選択」の2つを登録
    void AddMenuPair(GameObject* normalGo, GameObject* selectedGo);

    void Start() override;
    void Update() override;

    int GetSelectedIndex() const { return m_selected; }

private:
    GameObject* m_optionRoot = nullptr;
    bool m_isOpen = false;

    int m_selected = 0;

    std::vector<GameObject*> m_normals;
    std::vector<GameObject*> m_selecteds;

private:
    void Open();
    void Close();

    void MoveUp();
    void MoveDown();
    void ApplySelection();
};