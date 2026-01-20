#pragma once
#include <vector>
#include "Component.h"

class Component;

class OptionUIController : public MonoBehaviour
{
public:
    void Start() override;
    void Update() override;

private:
    bool m_isOpen = false;

    // オプション中に停止したいコンポーネントを退避
    std::vector<Component*> m_paused;

private:
    void Open();
    void Close();
    bool IsInThisHierarchy(GameObject* go) const;
};