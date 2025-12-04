// === File: PlayerMove.cpp ===
#include "PlayerMove.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "Camera.h"
#include "PlayerAction.h"
#include "Game.h"
#include "GameObject.h"
#include "Input.h"
#include <algorithm>
#include <cmath>
// --- XInput（スティックで移動、接続時は“原則”コントローラ優先）---
#include <Xinput.h>
#pragma comment(lib, "Xinput.lib")
using namespace DirectX;

static inline float Sign(bool pos, bool neg) { return (pos ? 1.0f : 0.0f) - (neg ? 1.0f : 0.0f); }
static inline float Rad2Deg(float r) { return r * (180.0f / 3.14159265358979323846f); }
static inline bool OverlapXZ(const Collider::AABB& a, const Collider::AABB& b)
{
    const bool ox = (a.min.x <= b.max.x) && (b.min.x <= a.max.x);
    const bool oz = (a.min.z <= b.max.z) && (b.min.z <= a.max.z);
    return ox && oz;
}

// XInput ヘルパ（最小）
namespace
{
    constexpr SHORT STICK_DEAD = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE; // 7849
    // 左スティックの入力を -1..1 で返却。戻り値は「コントローラが“実際に入力された”か」
    bool GetControllerAxes(float& outX, float& outZ)
    {
        XINPUT_STATE st{};
        if (XInputGetState(0, &st) != ERROR_SUCCESS) return false; // 未接続
        const SHORT sx = st.Gamepad.sThumbLX;
        const SHORT sy = st.Gamepad.sThumbLY;
        bool active = false;
        if (std::abs(sx) > STICK_DEAD) { outX = static_cast<float>(sx) / 32767.0f; active = true; }
        else outX = 0.0f;
        if (std::abs(sy) > STICK_DEAD) { outZ = static_cast<float>(sy) / 32767.0f; active = true; }
        else outZ = 0.0f;
        if (active) {
            const float len = std::sqrt(outX * outX + outZ * outZ);
            if (len > 1.0f) { outX /= len; outZ /= len; }
        }
        return active;
    }
}

constexpr XMFLOAT3 RespawnPos = { 0.0f, 1.0f, 0.0f };
constexpr float ResetPosY = -20.0f;

void PlayerMove::Start()
{
    t_ = gameObject->TryGetComponent<Transform>();
    rb_ = gameObject->TryGetComponent<Rigidbody>();
    col_ = nullptr;
    for (auto& kv : gameObject->components) {
        if (auto* c = dynamic_cast<Collider*>(kv.second.get())) { col_ = c; break; }
    }
    cam_ = Camera::Main()->gameObject->TryGetComponent<Transform>();
    pa_ = gameObject->TryGetComponent<PlayerAction>();

    assert(t_ && rb_ && col_ && "PlayerMove requires Transform/Rigidbody/Collider");
    facingYawDeg_ = 0.0f; // 初期yaw（最初の入力で正対）
}

void PlayerMove::Update()
{
    constexpr float DT = 1.0f / 60.0f;
    const bool grounded = IsGrounded();
    ApplyHorizontal(DT, grounded);

    if (t_->GetPosition().y < ResetPosY)
    {
        t_->SetPosition(RespawnPos);
        Debug::Log(" PlayerControll::RespawnPos");
    }
}

void PlayerMove::OnDestroy()
{
    t_ = nullptr; cam_ = nullptr; rb_ = nullptr; col_ = nullptr; cam_ = nullptr; pa_ = nullptr;
}

bool PlayerMove::IsGrounded() const
{
    

    const auto me = col_->GetWorldAABB();

    for (auto& o : Game::GetWorld().objects) {
        if (!o->active || o.get() == gameObject) continue;
        for (auto& kv : o->components) {
            auto* other = dynamic_cast<Collider*>(kv.second.get());
            if (!other || !other->isStatic) continue;
            const auto g = other->GetWorldAABB();
            if (!OverlapXZ(me, g)) continue;
            const float dy = me.min.y - g.max.y;
            if (dy >= -GROUND_EPS && dy <= GROUND_EPS) return true;
        }
    }
    return true;
}

float PlayerMove::HorizontalSpeed() const
{
    const Vector3& v = rb_->velocity;
    return std::sqrt(v.x * v.x + v.z * v.z);
}
float PlayerMove::HorizontalSpeedSq() const
{
    const Vector3& v = rb_->velocity;
    return (v.x * v.x + v.z * v.z);
}
const Vector3& PlayerMove::VelocityRef() const { return rb_->velocity; }

void PlayerMove::SetMoveOverride(bool on) { moveOverride_ = on; }
void PlayerMove::SetRotateLock(bool on) { rotateLock_ = on; }

void PlayerMove::AddExternalYaw(float deg)
{
    facingYawDeg_ += deg;
    if (facingYawDeg_ > 180.0f) facingYawDeg_ -= 360.0f;
    if (facingYawDeg_ < -180.0f) facingYawDeg_ += 360.0f;
}

void PlayerMove::ApplyHorizontal(float dt, bool grounded)
{
    // --- 入力（コントローラ優先：接続かつ入力があるときに採用。なければWASD） ---
    float ix = 0.0f, iz = 0.0f;
    const bool controllerActive = GetControllerAxes(ix, iz);
    if (!controllerActive) 
    {
        ix = Sign(IsKeyPress('D'), IsKeyPress('A'));
        iz = Sign(IsKeyPress('W'), IsKeyPress('S'));
    }

    // カメラ基底（XZ）
    XMVECTOR fwdXZ = XMVectorSet(0.f, 0.f, 1.f, 0.f);
    XMVECTOR rightXZ = XMVectorSet(1.f, 0.f, 0.f, 0.f);
    if (cam_) {
        const auto p = t_->GetPosition();
        const auto c = cam_->GetWorldPosition();
        XMVECTOR camFwd = XMVectorSet(p.x - c.x, p.y - c.y, p.z - c.z, 0.f);
        camFwd = XMVectorSet(XMVectorGetX(camFwd), 0.f, XMVectorGetZ(camFwd), 0.f);
        if (XMVectorGetX(XMVector3LengthSq(camFwd)) > 1e-6f) {
            fwdXZ = XMVector3Normalize(camFwd);
            rightXZ = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), fwdXZ));
        }
    }

    // 入力方向
    XMVECTOR wishv = XMVectorAdd(XMVectorScale(rightXZ, ix), XMVectorScale(fwdXZ, iz));
    Vector3  wish = Vector3::zero();
    const float wishLen2 = XMVectorGetX(XMVector3LengthSq(wishv));
    if (wishLen2 > 1e-6f) {
        wishv = XMVector3Normalize(wishv);
        wish.x = XMVectorGetX(wishv);
        wish.z = XMVectorGetZ(wishv);
    }

    // 向き（回転ロックされていなければ即時スナップ）
    if (!pa_->IsAction() && rotateToMove && !rotateLock_ && wishLen2 > 1e-6f)
    {
        FaceToWish(wish);
    }

    // 速度更新：外部上書き（ダッシュ等）中は書き換えない
    if (!moveOverride_) {
        const float scale = grounded ? 1.0f : AIR_CONTROL;
        const float targetVx = wish.x * MOVE_SPEED * scale;
        const float targetVz = wish.z * MOVE_SPEED * scale;

        Vector3& v = rb_->velocity;
        const float a = std::clamp(SMOOTH_ACC * dt, 0.0f, 1.0f);
        v.x += (targetVx - v.x) * a;
        v.z += (targetVz - v.z) * a;
    }


}

void PlayerMove::FaceToWish(const Vector3& wishDir)
{
    const float targetYaw = Rad2Deg(std::atan2(wishDir.x, wishDir.z)); // +Z前を0°
    float delta = targetYaw - facingYawDeg_;
    while (delta > 180.0f) delta -= 360.0f;
    while (delta < -180.0f) delta += 360.0f;
    if (std::fabs(delta) > 0.001f) {
        t_->AddRotate(Vector3(0, delta, 0));
        facingYawDeg_ += delta;
    }
}