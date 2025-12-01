#pragma once
#pragma once
#include "Component.h"
#include "Vector3.h"

class Rigidbody : public Component, public IFixedUpdate
{
private:
    Transform* tf = nullptr;
public:
    Vector3 velocity = Vector3::zero();
    bool useGravity = true;
    const float gravity = -9.8f;;
    float   mass = 1.0f;          // 質量
    float   drag = 0.0f;          // 空気抵抗
    float   friction = 0.5f;      // 摩擦


    void Start() override;
    void FixedUpdate() override;  // ここが「Integrate フェーズ」
    void AddForce(const Vector3& impulse);
};
