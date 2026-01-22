#pragma once
#include <vector>
#include "Component.h"

class GameObject;

enum class OptionResult
{
    None,
    Selected,   // Enter で確定した（どれかは GetSelectedIndex() で取得）
};

class OptionUIController : public MonoBehaviour
{
public:
    void SetOptionRoot(GameObject* root);

    // メニュー項目(見た目)を登録（CreateOptionUIで作った子のポインタを渡す）
    // 登録した順番が 0,1,2,... の選択順になる
    void AddMenuItem(GameObject* item);

    void Start() override;
    void Update() override;

    // 外部(SceneGame等)が結果を読むため
    OptionResult GetResult() const { return m_result; }
    void ClearResult() { m_result = OptionResult::None; }
    int GetSelectedIndex() const { return m_selected; }

private:
    GameObject* m_optionRoot = nullptr;

    bool m_isOpen = false;

    // 追加：選択状態
    int m_selected = 0;
    std::vector<GameObject*> m_items;

    // 追加：確定結果
    OptionResult m_result = OptionResult::None;
private:
    DirectX::XMFLOAT4 m_colorNormal = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 0.60f);
    DirectX::XMFLOAT4 m_colorSelected = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.00f);
private:
    void Open();
    void Close();

    void MoveUp();
    void MoveDown();

    void ApplySelectionVisual();
};