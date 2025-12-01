#pragma once
#include "Component.h"
#include "Vector3.h"
#include "Vector2.h"
#include <cstdint>

enum class ActionId : std::uint8_t
{
    NormalAttack,
    SpinAttack,
    DashAttack,
    Count
};


class PlayerState: public Component
{
public:
    // 入力
    Vector2 moveAxis = Vector2::zero();

    // 行動状態
    bool isGrounded = true;
    bool canMove = true;
    bool canControl = true;

    // --- アクション関連 ---
    bool  wantUseAction[static_cast<size_t>(ActionId::Count)]{};
    bool  canUseAction[static_cast<size_t>(ActionId::Count)]{};   // 今使えるか？
    float cooldownRatio[static_cast<size_t>(ActionId::Count)]{};  // 0〜1 (0=準備完了,1=撃った直後) 等
public:
    static constexpr Vector3 kInitPos = { -3.0f, 0.0f, 0.0f };
    static constexpr Vector3 kSize = { 1.0f, 1.0f, 1.0f };
    static constexpr float kRadius = 0.5f;
};
