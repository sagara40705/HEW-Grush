// === File: PlayerControll.cpp ===
#include "PlayerControll.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "Camera.h"
#include "SphereCollider.h"
#include "GameObject.h"
#include "World.h"
#include "Input.h"
#include <algorithm>
#include <cmath>
#include "DebugLog.h"
#include "Game.h"

using namespace DirectX;

static inline float Clamp01(float v) { return std::clamp(v, 0.0f, 1.0f); }
static inline float Sign(bool pos, bool neg) { return (pos ? 1.0f : 0.0f) - (neg ? 1.0f : 0.0f); }

constexpr XMFLOAT3 RespawnPos = { 0.0f, 1.0f, 0.0f };
constexpr float ResetPosY = -20.0f;

static inline bool OverlapXZ(const Collider::AABB& a, const Collider::AABB& b)
{
    const bool ox = (a.min.x <= b.max.x) && (b.min.x <= a.max.x);
    const bool oz = (a.min.z <= b.max.z) && (b.min.z <= a.max.z);
    return ox && oz;
}

void PlayerControll::Start()
{
    t = Parent->TryGetComponent<Transform>();
    camera_ = Camera::Main()->Parent->TryGetComponent<Transform>();
    rb_ = Parent->TryGetComponent<Rigidbody>();

    // 自分の最初の Collider を拾う
    col_ = nullptr;
    for (auto& kv : Parent->components)
    {
        if (auto* c = dynamic_cast<Collider*>(kv.second.get()))
        {
            col_ = c;
            break;
        }
    }

    assert(t && "PlayerControll requires Transform");
    assert(rb_ && "PlayerControll requires Rigidbody");
    assert(col_ && "PlayerControll requires Collider (Box/Sphere/...)");


}

void PlayerControll::Update()
{
    constexpr float DT = 1.0f / 60.0f;

    // クールタイム減算
    if (dashCooldownTimer > 0.0f) dashCooldownTimer = std::max(0.0f, dashCooldownTimer - DT);
    if (spinCooldownTimer > 0.0f) spinCooldownTimer = std::max(0.0f, spinCooldownTimer - DT);

    const bool grounded = IsGrounded();

    if (isDash) {
        DashUpdate(DT);
    }
    else {
        ApplyHorizontal(DT, grounded);
        ApplyJump(grounded);
        DashTryStart(grounded); // ← ここで発動判定
        Spinning(DT);           // ← クールタイム管理版
    }

    if (t->GetPosition().y < ResetPosY)
    {
        t->SetPosition(RespawnPos);
        Debug::Log(" PlayerControll::RespawnPos");
    }


}

bool PlayerControll::IsGrounded() const
{
    const auto me = col_->GetWorldAABB();

    for (auto& o : Game::GetWorld().objects)
    {
        if (!o->active) continue;
        if (o.get() == Parent) continue;

        for (auto& kv : o->components)
        {
            auto* other = dynamic_cast<Collider*>(kv.second.get());
            if (!other) continue;
            if (!other->isStatic) continue;

            const auto g = other->GetWorldAABB();
            if (!OverlapXZ(me, g)) continue;

            const float dy = me.min.y - g.max.y;
            if (dy >= -GROUND_EPS && dy <= GROUND_EPS) return true;
        }
    }
    return false;


}

void PlayerControll::ApplyHorizontal(float dt, bool grounded)
{
    // 入力
    const float ix = Sign(IsKeyPress('D'), IsKeyPress('A'));
    const float iz = Sign(IsKeyPress('W'), IsKeyPress('S'));

    // カメラ基底（XZ）
    XMVECTOR fwdXZ = XMVectorSet(0.f, 0.f, 1.f, 0.f);
    XMVECTOR rightXZ = XMVectorSet(1.f, 0.f, 0.f, 0.f);
    if (camera_)
    {
        const auto p = t->GetPosition();
        const auto c = camera_->GetPosition();
        XMVECTOR camFwd = XMVectorSet(p.x - c.x, p.y - c.y, p.z - c.z, 0.f);
        camFwd = XMVectorSet(XMVectorGetX(camFwd), 0.f, XMVectorGetZ(camFwd), 0.f);
        const float len2 = XMVectorGetX(XMVector3LengthSq(camFwd));
        if (len2 > 1e-6f)
        {
            fwdXZ = XMVector3Normalize(camFwd);
            rightXZ = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), fwdXZ));
        }
    }

    // 入力方向ベクトル（正規化）
    XMVECTOR wish = XMVectorAdd(XMVectorScale(rightXZ, ix), XMVectorScale(fwdXZ, iz));
    if (XMVectorGetX(XMVector3LengthSq(wish)) > 1e-6f) wish = XMVector3Normalize(wish);

    // 通常の歩行加速
    const float scale = grounded ? 1.0f : AIR_CONTROL;
    const float targetVx = XMVectorGetX(wish) * MOVE_SPEED * scale;
    const float targetVz = XMVectorGetZ(wish) * MOVE_SPEED * scale;

    Vector3& v = rb_->velocity;
    const float ax = (targetVx - v.x) * std::clamp(SMOOTH_ACC * dt, 0.0f, 1.0f);
    const float az = (targetVz - v.z) * std::clamp(SMOOTH_ACC * dt, 0.0f, 1.0f);

    v.x += ax;
    v.z += az;


}

void PlayerControll::ApplyJump(bool grounded)
{
    if (!grounded) return;
    if (IsKeyTrigger(VK_SPACE))
    {
        rb_->velocity.y = JUMP_SPEED;
    }
}

void PlayerControll::OnScale()
{
    t->AddScale(Vector3(0.02f, 0.02f, 0.02f));
}

void PlayerControll::OnDestroy()
{
    t = nullptr;
    rb_ = nullptr;
    col_ = nullptr;
    hummer_ = nullptr;
}

void PlayerControll::OnCollisionEnter(const CollisionInfo&)
{
}

// --- “移動中かつ接地時にF押下”でのみ発動、クールダウン中は不可、停止中は不可 ---
void PlayerControll::DashTryStart(bool grounded)
{
    if (!grounded) return;
    if (!IsKeyTrigger('F')) return;
    if (isDash) return;
    if (dashCooldownTimer > 0.0f) return;

    // “移動中”の判定は現在の水平速度で行う
    const Vector3 v = rb_->velocity;
    const float v2 = v.x * v.x + v.z * v.z;
    if (v2 <= (minDashMoveEps * minDashMoveEps)) return; // 停止中→発動しない

    // 水平速度方向へダッシュ
    const float inv = 1.0f / std::sqrt(std::max(v2, 1e-12f));
    dashDir = { v.x * inv, 0.0f, v.z * inv };

    // 発動
    isDash = true;
    dashTime = 0.0f;

    // 初速「5」を付与（水平のみ）
    rb_->velocity.x = dashDir.x * dashStartSpeed;
    rb_->velocity.z = dashDir.z * dashStartSpeed;

    // クールタイム開始
    dashCooldownTimer = dashCooldown;


}

// --- ダッシュ本体：初速「5」→「ダッシュ時間」秒で「2」まで線形減速 ---
void PlayerControll::DashUpdate(float dt)
{
    dashTime += dt;

    const float T = std::max(dashDuration, 1e-6f);
    const float t = std::min(dashTime, T);
    const float ratio = t / T; // 0→1
    const float spd = dashStartSpeed + (dashEndSpeed - dashStartSpeed) * ratio; // 5→2

    // 水平成分を上書き（yは重力維持）
    rb_->velocity.x = dashDir.x * spd;
    rb_->velocity.z = dashDir.z * spd;

    if (dashTime >= T)
    {
        isDash = false;
        dashTime = 0.0f;

        // 終端速度「2」を残す（水平のみ）
        rb_->velocity.x = dashDir.x * dashEndSpeed;
        rb_->velocity.z = dashDir.z * dashEndSpeed;
    }


}

// --- 回転攻撃：クールタイム「3」秒、CD中は起動不可 ---
void PlayerControll::Spinning(float dt)
{
    if (!spinning)
    {
        if (IsKeyTrigger('R') && spinCooldownTimer <= 0.0f)
        {
            spinning = true;
            spinAccumDeg = 0.0f;
            isAttack = true;
        }
    }

    if (spinning)
    {
        const float delta = spinRotSpeed * dt;
        spinAccumDeg += delta;
        t->AddRotate(Vector3(0, delta, 0));

        if (spinAccumDeg >= 360.0f)
        {
            spinning = false;
            isAttack = false;
            spinAccumDeg = 0.0f;
            spinCooldownTimer = spinCooldown; // クールタイム開始
        }
    }


}

void PlayerControll::SetHummerActive(bool)
{
    // TODO: 必要であれば武器ON/OFFを管理
}