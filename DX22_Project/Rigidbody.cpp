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
