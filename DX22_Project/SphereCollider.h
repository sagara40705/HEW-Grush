#pragma once
#include "Collider.h"
#include <DirectXMath.h>

class SphereCollider : public Collider
{
public:
    DirectX::XMFLOAT3 center{ 0,0,0 }; // ローカル中心
    float radius = 0.5f;             // ローカル半径

    SphereCollider() = default;
    SphereCollider(float r, bool st = false, const DirectX::XMFLOAT3& c = { 0,0,0 }) : center(c), radius(r) { isStatic = st; }

    ColliderType GetType() const override { return ColliderType::Sphere; }

    // 球（中心と半径）
    struct Sphere { DirectX::XMFLOAT3 c; float r; };
    Sphere GetWorldSphere() const;

    // Collider::AABB を満たす
    AABB GetWorldAABB() override;
};