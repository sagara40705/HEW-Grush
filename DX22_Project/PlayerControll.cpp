#include "PlayerControll.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Collider.h"   // ★ 追加
#include "Camera.h"
#include "SphereCollider.h"
#include "GameObject.h"
#include "World.h"
#include "Input.h"
#include <algorithm>
#include <cmath>
#include "DebugLog.h"

// Scene 内の World は既存と同様に外部参照
extern World W;

// 便利用
static inline float Clamp01(float v) { return std::clamp(v, 0.0f, 1.0f); }
static inline float Sign(bool pos, bool neg) { return (pos ? 1.0f : 0.0f) - (neg ? 1.0f : 0.0f); }

constexpr DirectX::XMFLOAT3 RespawnPos = { 0.0f, 1.0f, 0.0f };
constexpr float ResetPosY = -20.0f;

// 2D（XZ）オーバーラップ（Collider::AABB 共通）
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

    // ★ 自分の GameObject から「最初に見つかった Collider」を拾う
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

bool PlayerControll::IsGrounded() const
{
    // 自身AABB（Collider 共通）
    const auto me = col_->GetWorldAABB();
    
    // すべての静的 Collider と照合（XZ重なり かつ 足元が近接）
    for (auto& o : W.objects)
    {
        if (!o->active) continue;
        if (o.get() == Parent) continue;
    
        // ★ オブジェクト内のすべての Collider をチェック
        for (auto& kv : o->components)
        {
            auto* other = dynamic_cast<Collider*>(kv.second.get());
            if (!other) continue;
            if (!other->isStatic) continue;   // 地形など
            //if (other->isTrigger) continue;   // トリガーは除外
    
            const auto g = other->GetWorldAABB();
            if (!OverlapXZ(me, g)) continue;
    
            const float dy = me.min.y - g.max.y; // 足元 - 地面上端
            if (dy >= -GROUND_EPS && dy <= GROUND_EPS) return true;
        }
    }
    return false;
}

void PlayerControll::ApplyHorizontal(float dt, bool grounded)
{
    using namespace DirectX;

    // 入力取得
    const float ix = Sign(IsKeyPress('D'), IsKeyPress('A')); // 右(+1)/左(-1)
    const float iz = Sign(IsKeyPress('W'), IsKeyPress('S')); // 前(+1)/後(-1)

    // デフォルトは XZ ワールド基底（カメラ未設定時の後方互換）
    XMVECTOR fwdXZ = XMVectorSet(0.f, 0.f, 1.f, 0.f);  // +Z
    XMVECTOR rightXZ = XMVectorSet(1.f, 0.f, 0.f, 0.f); // +X

    // ★ カメラがあれば「カメラ前方（XZ投影）」を前進方向に採用
    if (camera_)
    {
        const XMFLOAT3 p = t->GetPosition();
        const XMFLOAT3 c = camera_->GetPosition();

        // カメラの視線は「カメラ→プレイヤー」方向と一致（TPS前提）
        XMVECTOR camFwd = XMVectorSet(p.x - c.x, p.y - c.y, p.z - c.z, 0.f);

        // 地面（XZ）へ投影して正規化
        camFwd = XMVectorSet(XMVectorGetX(camFwd), 0.f, XMVectorGetZ(camFwd), 0.f);
        const float len2 = XMVectorGetX(XMVector3LengthSq(camFwd));
        if (len2 > 1e-6f)
        {
            fwdXZ = XMVector3Normalize(camFwd);
            // 右ベクトル = up(0,1,0) × fwd
            rightXZ = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), fwdXZ);
            rightXZ = XMVector3Normalize(rightXZ);
        }
    }

    // 入力を基底に合成（W/S は fwd、A/D は right）
    XMVECTOR wish = XMVectorAdd(XMVectorScale(rightXZ, ix), XMVectorScale(fwdXZ, iz));
    const float wishLen2 = XMVectorGetX(XMVector3LengthSq(wish));
    if (wishLen2 > 1e-6f) wish = XMVector3Normalize(wish);

    const float scale = grounded ? 1.0f : AIR_CONTROL;
    const float targetVx = XMVectorGetX(wish) * MOVE_SPEED * scale;
    const float targetVz = XMVectorGetZ(wish) * MOVE_SPEED * scale;

    // 既存の加速度追従ロジックを流用
    Vector3& v = rb_->velocity;
    const float ax = (targetVx - v.x) * std::clamp(SMOOTH_ACC * dt, 0.0f, 1.0f);
    const float az = (targetVz - v.z) * std::clamp(SMOOTH_ACC * dt, 0.0f, 1.0f);

    if (IsKeyPress('F'))
    {
        moveDir = Vector3(targetVx, 0, targetVz);
        isDash = true;
        v.x += ax * 2;
        v.z += az * 2;
        return;
    }

    v.x += ax;
    v.z += az;

    // 斜めクリップ
    const float vlen2 = v.x * v.x + v.z * v.z;
    const float maxH = MOVE_SPEED * (grounded ? 1.0f : AIR_CONTROL);
    const float max2 = maxH * maxH;
    if (vlen2 > max2 && (ix != 0.0f || iz != 0.0f))
    {
        const float l = std::sqrt(vlen2);
        v.x = v.x / l * maxH;
        v.z = v.z / l * maxH;
    }
}

void PlayerControll::ApplyJump(bool grounded)
{
    if (!grounded) return;
    if (IsKeyTrigger(VK_SPACE))
    {
        // velocity に即時上向き初速を与える
        rb_->velocity.y = JUMP_SPEED;
    }
}

void PlayerControll::OnScale()
{
    t->AddScale(Vector3(0.02f,0.02f,0.02f));
}

void PlayerControll::Update()
{
    // Rigidbody の固定Δtは 1/60
    constexpr float DT = 1.0f / 60.0f;

    if (isDash) Dash();
    else
    {
        const bool grounded = IsGrounded();
        ApplyHorizontal(DT, grounded);
        ApplyJump(grounded);
        Spinning();
    }

    if (t->GetPosition().y < ResetPosY)
    {
        t->SetPosition(RespawnPos);
        Debug::Log(" PlayerControll::RespawnPos");
    }
}

void PlayerControll::OnDestroy()
{
    t = nullptr;
    rb_ = nullptr;
    col_ = nullptr;
}

void PlayerControll::OnCollisionEnter(const CollisionInfo& c)
{
}

void PlayerControll::OnTriggerEnter(const TriggerInfo& t)
{
    if (!t.other) return;

    if (!t.other->CompareTag(Tag::Glass)) return;
    Debug::LogValue(" PlayerControll::Destroy id", t.other->id);
    t.other->Destroy();
    OnScale();
}

void PlayerControll::Spinning()
{
    constexpr float DT = 1.0f / 60.0f;
    // --- ★ Rキーで360度回転 ---
    static float rotProgress = 0.0f;
    static bool rotating = false;

    if (IsKeyTrigger('R')) {
        rotating = true;
        rotProgress = 0.0f;

    }

    if (rotating) {
        constexpr float ROT_SPEED = 720.0f; // deg/s
        float delta = ROT_SPEED * DT;
        rotProgress += delta;
        t->AddRotate(Vector3(0, delta, 0)); // Y軸回転

        if (rotProgress >= 360.0f) {
            rotating = false;
            rotProgress = 0.0f;
        }
    }
}

void PlayerControll::Dash()
{
    constexpr float DT = 1.0f / 60.0f;
    static float dashTimer = 0.0f;

    dashTimer += DT;

    if (dashTimer > 1.0f)
    {
        isDash = false;
        dashTimer = 0.0f;
    }
}
