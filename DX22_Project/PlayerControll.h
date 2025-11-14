#pragma once
#include "CollisionListener.h"

class Transform;
class Rigidbody;
class Collider;   // ★ BoxCollider 固定から基盤Colliderへ

class PlayerControll final : public MonoBehaviour, public CollisionListener
{
public:
    // ====== 定数 ======
    static constexpr float MOVE_SPEED = 5.0f;  // m/s
    static constexpr float AIR_CONTROL = 0.5f;  // 空中制御 0..1
    static constexpr float JUMP_SPEED = 5.0f;  // 上向き初速
    static constexpr float GROUND_EPS = 0.10f; // 接地許容
    static constexpr float SMOOTH_ACC = 20.0f; // XZ 追従加速

public:
    void Start() override;
    void Update() override;
    void OnDestroy() override;
    void OnCollisionEnter(const CollisionInfo& c) override;
    //void OnCollisionStay(const CollisionInfo& c) override;

private:
    Transform* t = nullptr;
    Transform* camera_ = nullptr;
    Rigidbody* rb_ = nullptr;
    Collider* col_ = nullptr; // ★ 基盤Colliderポインタ
    Collider* hummer_ = nullptr;

    bool IsGrounded() const;
    void ApplyHorizontal(float dt, bool grounded);
    void ApplyJump(bool grounded);
private:
    void OnScale();
    void Spinning();
    void Dash();
    void SetHummerActive(bool activ);
private:
    bool isAttack = false;
    bool isDash = false;
    Vector3 moveDir = Vector3::zero();
};
