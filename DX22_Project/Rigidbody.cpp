// Rigidbody.cpp（イメージ）
#include "Rigidbody.h"
#include "Transform.h"
#include "GameObject.h"
#include "Time/Time.h"

void Rigidbody::Start()
{
    tf = gameObject->GetComponent<Transform>();
}


void Rigidbody::FixedUpdate()
{
    // Δt
    const float dt = Time::DeltaTime();

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
    Vector3 dp = velocity * Time::DeltaTime();
    tf->AddPosition(dp);
}

void Rigidbody::AddForce(const Vector3& impulse)
{
    if (mass <= 0.0f) return;
    velocity += impulse / mass;
}
