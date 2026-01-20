#pragma once
#include <vector>
#include "Component.h"

class Component;

class OptionUIController : public MonoBehaviour
{
public:
    void Start() override;
    void Update() override;

    // 追加：操作対象のUI Rootを渡す
    void SetOptionRoot(GameObject* root);

private:
    bool m_isOpen = false;

    // 追加：UIのRoot（表示/非表示の対象）
    GameObject* m_optionRoot = nullptr;

    // オプション中に停止したいコンポーネントを退避
    std::vector<Component*> m_paused;

private:
    void Open();
    void Close();
    bool IsInThisHierarchy(GameObject* go) const;
};