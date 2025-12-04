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
// --- XInput（ボタンでアクション。コントローラ入力が“あれば”キーボードを無視）---
#include <Xinput.h>
#pragma comment(lib, "Xinput.lib")
using namespace DirectX;
#define CONIOEX
#include"conioex.h"

static inline float Clamp01(float v) { return std::clamp(v, 0.0f, 1.0f); }
INT_PTR g_WeaponSwingSE;
INT_PTR g_DashAttackSE;
INT_PTR g_SpinAttackSE;

// ボタン割り当て（必要なら変更）
namespace PadMap 
{
    constexpr WORD Dash = XINPUT_GAMEPAD_RIGHT_SHOULDER; // R1相当
    constexpr WORD Spin = XINPUT_GAMEPAD_LEFT_SHOULDER;  // L1相当
    constexpr WORD Normal = XINPUT_GAMEPAD_X; // □相当
    constexpr SHORT STICK_DEAD = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
}
// 簡易エッジ判定をこの翻訳単位で保持（他所の XInput 呼び出しと独立）
namespace 
{
    struct PadState { XINPUT_STATE prev{}, curr{}; bool connected = false; } gPad;
    void PollPad()
    {
        gPad.prev = gPad.curr;
        gPad.connected = (XInputGetState(0, &gPad.curr) == ERROR_SUCCESS);
    }
    bool Press(WORD m) { return gPad.connected && (gPad.curr.Gamepad.wButtons & m); }
    bool Trigger(WORD m) { return gPad.connected && (gPad.curr.Gamepad.wButtons & m) && !(gPad.prev.Gamepad.wButtons & m); }
    bool AnyStickActive()
    {
        const auto& gp = gPad.curr.Gamepad;
        return (std::abs(gp.sThumbLX) > PadMap::STICK_DEAD) || (std::abs(gp.sThumbLY) > PadMap::STICK_DEAD);
    }
    bool ControllerActiveThisFrame()
    {
        if (!gPad.connected) return false;
        const bool anyBtn = (gPad.curr.Gamepad.wButtons != 0);
        return anyBtn || AnyStickActive();
    }
}

PlayerAction::PlayerAction()
{
    g_WeaponSwingSE = opensound("Sound/WeaponSwing.mp3");
    g_DashAttackSE = opensound("Sound/DashAttack.mp3");
    g_SpinAttackSE = opensound("Sound/SpinAttack.mp3");
}

PlayerAction::~PlayerAction()
{
    closesound(g_WeaponSwingSE);
    closesound(g_DashAttackSE);
    closesound(g_SpinAttackSE);
}

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

    PollPad();

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
    if (isDash_) return;
    if (isAction_) return;
    if (dashCooldownTimer_ > 0.0f) return;

    // “入力デバイスの選択”：このフレームにパッド入力があればキーボードは無視
    const bool usePad = ControllerActiveThisFrame();
    const bool dashKey = (!usePad) && IsKeyTrigger('F');
    const bool dashPad = usePad && Trigger(PadMap::Dash);
    if (!(dashKey || dashPad)) return;

    // 移動中のみ
    if (move_->HorizontalSpeedSq() <= (minDashMoveEps * minDashMoveEps)) return;

    // 方向は現在の水平速度向き
    const Vector3 v = move_->VelocityRef();
    const float v2 = std::max(v.x * v.x + v.z * v.z, 1e-12f);
    const float a = 1.00f;
    const float inv = 1.0f / std::sqrt(v2);
    dashDir_ = { v.x * inv * a, 0.0f, v.z * inv * a };

    // 発動
    isDash_ = true;
    isAction_ = true;
    dashTime_ = 0.0f;
    rb_->velocity.x = dashDir_.x * dashStartSpeed;
    rb_->velocity.z = dashDir_.z * dashStartSpeed;

    // この間、移動側は速度をいじらない
    move_->SetMoveOverride(true);

    playsound(g_DashAttackSE, 0);
}

void PlayerAction::DashUpdate(float dt)
{
    dashTime_ += 0; // 互換：誤記を避けるため行を残す（未使用変数対策）
    dashTime_ += dt;

    const float T = std::max(dashDuration, 1e-6f);
    const float t = std::min(dashTime_, T);
    const float ratio = t / T; // 0��1
    const float usi = dashEndSpeed - dashStartSpeed;
    const float spd = dashStartSpeed + ((dashEndSpeed - dashStartSpeed)) * ratio; //

    rb_->velocity.x = dashDir_.x * spd;
    rb_->velocity.z = dashDir_.z * spd;

    if (dashTime_ >= T) {
        isDash_ = false;
        isAction_ = false;
        dashTime_ = 0.0f;
        dashCooldownTimer_ = dashCooldown;
        // 終端速度を残す
        rb_->velocity.x = dashDir_.x * dashEndSpeed;
        rb_->velocity.z = dashDir_.z * dashEndSpeed;
        move_->SetMoveOverride(false);
    }
}

void PlayerAction::SpinningUpdate(float dt)
{
    const bool usePad = ControllerActiveThisFrame();
    const bool spinKey = (!usePad) && IsKeyTrigger('R');
    const bool spinPad = usePad && Trigger(PadMap::Spin);

    // 起動
    if (!spinning_ && !isAction_)
    {
        if ((spinKey || spinPad) && spinCooldownTimer_ <= 0.0f) {
            spinning_ = true;
            spinAccumDeg_ = 0.0f;
            move_->SetRotateLock(true);

            playsound(g_SpinAttackSE,0);
        }
    }

    // 実行
    if (spinning_) {
        const float delta = spinRotSpeed * dt;
        spinAccumDeg_ += delta;
        t_->AddRotate(Vector3(0, delta, 0));
        move_->AddExternalYaw(delta); // 自前yawも同期

        if (spinAccumDeg_ >= spinRotate) {
            spinning_ = false;
            isAction_ = false;
            spinAccumDeg_ = 0.0f;
            move_->SetRotateLock(false);
            spinCooldownTimer_ = spinCooldown;
        }
    }
}

void PlayerAction::NormalAttackTryStart()
{
    if (!hummer_) return;
    if (normalCooldownTimer_ > 0.0f) return;
    if (isAction_) return;

    const bool usePad = ControllerActiveThisFrame();
    const bool atkKey = (!usePad) && IsKeyTrigger('E');
    const bool atkPad = usePad && Trigger(PadMap::Normal);
    if (!(atkKey || atkPad)) return;

    hummer_->motionTime = normalMotionTime;
    hummer_->StartNormalAttack();
    playsound(g_WeaponSwingSE,0);

    // 実質連続で撃てる＝モーション時間をCTに
    normalCooldownTimer_ = normalMotionTime;
}

void PlayerAction::UpdateHummerMask()
{
    if (!hummer_) return;
    const bool on = ((hummer_->IsNormalAttacking()) || spinning_ || isDash_);
    isAction_ = on;
    hummer_->SetActionHitEnable(on);
}