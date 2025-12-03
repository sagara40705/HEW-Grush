#pragma once
#include "Component.h"
#include "Vector3.h"
#include "Vector2.h"
#include <cstdint>

enum class ActionId : std::uint8_t
{
    NormalAttack = 0,
    SpinAttack = 1,
    DashAttack = 2,
    Count = 3,
    None = 99
};
constexpr size_t ToIndex(ActionId id) noexcept 
{
    return static_cast<size_t>(id);
}
constexpr ActionId ToId(size_t n) noexcept
{
    return (n < ToIndex(ActionId::Count)) ? static_cast<ActionId>(n) : ActionId::None;
}

class PlayerState: public Component
{
public:
    // 入力
    Vector2 moveAxis = Vector2::zero();

    // 行動状態
    bool isGrounded = true;
    bool canMove = true;
    bool canAction = true;
    bool canControl = true;

    // --- アクション関連 ---
    bool  wantUseAction[static_cast<size_t>(ActionId::Count)]{}; //入力されたか
    ActionId usingAction = ActionId::None; //使用中のアクション
    bool  canUseAction[static_cast<size_t>(ActionId::Count)]{};   // 今使えるか？
    float actionTimer[static_cast<size_t>(ActionId::Count)]{}; //アクション中のタイマー
    float cooltime[static_cast<size_t>(ActionId::Count)]{}; // 0〜1 (0=準備完了,1=撃った直後) 等
public:
    static constexpr Vector3 kInitPos = { -3.0f, 0.0f, 0.0f };
    static constexpr Vector3 kSize = { 1.0f, 1.0f, 1.0f };
    static constexpr float kRadius = 0.5f;
public:
    const float kActionTime[ToIndex(ActionId::Count)] = { 0.5f, 1.f, 2.f };
    const float kActionCT[ToIndex(ActionId::Count)] = { 0.1f, 5.f, 5.f };
public:
    float GetCoolDownRatio(ActionId action) 
    {
        return (cooltime[ToIndex(action)] / kActionCT[ToIndex(action)]);
    }
};
