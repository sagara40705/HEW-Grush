// Rigidbody.cpp（イメージ）
#include "Rigidbody.h"
#include "Transform.h"
#include "GameObject.h"

void Rigidbody::Start()
{
    tf = gameObject->TryGetComponent<Transform>();
}

void Rigidbody::Update()
{
    
    //if (!tf || isKinematic) return;

    // 固定Δt想定（あなたのプロジェクトに合わせて）
    constexpr float DT = 1.f / 60.f;

    // 重力
    velocity.y += gravity * DT;

    // 積分
    Vector3 dp = velocity *  DT;

    tf->AddPosition(dp);

    // ※ 衝突は PhysicsSystem::Step() で解決され、必要なら速度の法線成分もそこで修正されます
}

void Rigidbody::FixedUpdate()
{
    auto * go = gameObject;
    if (!go) return;

    auto* tf = go->TryGetComponent<Transform>();
    if (!tf) return;

    // 固定Δt
    constexpr float dt = 1.0f / 60.0f;

    // 1) 重力
    if (useGravity)
    {
        velocity.y += gravity * dt;
    }

    // 2) 空気抵抗（drag）：速度を徐々に減衰
    if (drag > 0.0f)
    {
        float k = std::max(0.0f, 1.0f - drag * dt);
        velocity *= k;
    }

    // 3) 積分（位置更新）
    Vector3 dp = velocity * dt;
    tf->AddPosition(dp);

}

void Rigidbody::AddForce(const Vector3& impulse)
{
    if (mass <= 0.0f) return;
    velocity += impulse / mass;
}
