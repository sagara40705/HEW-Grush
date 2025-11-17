// === File: PlayerAction.cpp ===
#include "PlayerAction.h"
#include "PlayerMove.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "HummerScript.h"
#include "Game.h"
#include "Input.h"
#include <algorithm>
#include <cmath>

using namespace DirectX;

static inline float Clamp01(float v) { return std::clamp(v, 0.0f, 1.0f); }

void PlayerAction::Start()
{
    t_ = Parent->TryGetComponent<Transform>();
    rb_ = Parent->TryGetComponent<Rigidbody>();
    move_ = Parent->TryGetComponent<PlayerMove>();
    assert(t_ && rb_ && move_ && "PlayerAction requires Transform/Rigidbody/PlayerMove");

    AcquireHummer();


}

void PlayerAction::Update()
{
    constexpr float DT = 1.0f / 60.0f;

    // クールタイム
    if (dashCooldownTimer_ > 0.0f) dashCooldownTimer_ = std::max(0.0f, dashCooldownTimer_ - DT);
    if (spinCooldownTimer_ > 0.0f) spinCooldownTimer_ = std::max(0.0f, spinCooldownTimer_ - DT);
    if (normalCooldownTimer_ > 0.0f) normalCooldownTimer_ = std::max(0.0f, normalCooldownTimer_ - DT);

    // 回転攻撃（向きロック）
    SpinningUpdate(DT);

    // ダッシュ（外部速度上書き）
    if (isDash_) DashUpdate(DT);
    else         DashTryStart();

    // 通常攻撃（E）
    NormalAttackTryStart();

    // “行動中だけ”ハンマー当たり判定ON
    UpdateHummerMask();


}

void PlayerAction::OnDestroy()
{
    t_ = nullptr; rb_ = nullptr; move_ = nullptr; hummer_ = nullptr;
}

void PlayerAction::AcquireHummer()
{
    // Worldから自分の子（Tag::Weapon）を探索
    for (auto& o : Game::GetWorld().objects) {
        if (!o->active || !o->CompareTag(Tag::Weapon)) continue;
        HummerScript* hs = o->TryGetComponent<HummerScript>();
        Transform* ht = o->TryGetComponent<Transform>();
        if (hs && ht && ht->GetParent() == t_) { hummer_ = hs; break; }
    }
}

void PlayerAction::DashTryStart()
{
    if (!move_->IsGrounded()) return;
    if (!IsKeyTrigger('F')) return;
    if (isDash_) return;
    if (dashCooldownTimer_ > 0.0f) return;

    // 移動中のみ
    if (move_->HorizontalSpeedSq() <= (minDashMoveEps * minDashMoveEps)) return;

    // 方向は現在の水平速度向き
    const Vector3 v = move_->VelocityRef();
    const float v2 = std::max(v.x * v.x + v.z * v.z, 1e-12f);
    const float inv = 1.0f / std::sqrt(v2);
    dashDir_ = { v.x * inv, 0.0f, v.z * inv };

    // 発動
    isDash_ = true;
    dashTime_ = 0.0f;
    rb_->velocity.x = dashDir_.x * dashStartSpeed;
    rb_->velocity.z = dashDir_.z * dashStartSpeed;

    // この間、移動側は速度をいじらない
    move_->SetMoveOverride(true);

    dashCooldownTimer_ = dashCooldown;


}

void PlayerAction::DashUpdate(float dt)
{
    dashTime_ += 0; // 互換：誤記を避けるため行を残す（未使用変数対策）
    dashTime_ += dt;

    const float T = std::max(dashDuration, 1e-6f);
    const float r = std::min(dashTime_ / T, 1.0f);
    const float spd = dashStartSpeed + (dashEndSpeed - dashStartSpeed) * r; // 5→2

    rb_->velocity.x = dashDir_.x * spd;
    rb_->velocity.z = dashDir_.z * spd;

    if (dashTime_ >= T) {
        isDash_ = false;
        dashTime_ = 0.0f;
        // 終端速度を残す
        rb_->velocity.x = dashDir_.x * dashEndSpeed;
        rb_->velocity.z = dashDir_.z * dashEndSpeed;
        move_->SetMoveOverride(false);
    }


}

void PlayerAction::SpinningUpdate(float dt)
{
    // 起動
    if (!spinning_) {
        if (IsKeyTrigger('R') && spinCooldownTimer_ <= 0.0f) {
            spinning_ = true;
            spinAccumDeg_ = 0.0f;
            move_->SetRotateLock(true);
        }
    }

    // 実行
    if (spinning_) {
        const float delta = spinRotSpeed * dt;
        spinAccumDeg_ += delta;
        t_->AddRotate(Vector3(0, delta, 0));
        move_->AddExternalYaw(delta); // 自前yawも同期

        if (spinAccumDeg_ >= 360.0f) {
            spinning_ = false;
            spinAccumDeg_ = 0.0f;
            move_->SetRotateLock(false);
            spinCooldownTimer_ = spinCooldown;
        }
    }


}

void PlayerAction::NormalAttackTryStart()
{
    if (!hummer_) return;
    if (!IsKeyTrigger('E')) return;
    if (normalCooldownTimer_ > 0.0f) return;

    hummer_->motionTime = normalMotionTime;
    hummer_->StartNormalAttack();

    // 実質連続で撃てる＝モーション時間をCTに
    normalCooldownTimer_ = normalMotionTime;


}

void PlayerAction::UpdateHummerMask()
{
    if (!hummer_) return;
    const bool on = ((hummer_->IsNormalAttacking()) || spinning_ || isDash_);
    hummer_->SetActionHitEnable(on);
}