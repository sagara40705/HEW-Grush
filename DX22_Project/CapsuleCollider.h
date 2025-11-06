#pragma once
#include "Collider.h"
#include <DirectXMath.h>

class CapsuleCollider : public Collider
{
public:
    // ローカル中心、Y軸カプセル（円柱の中心線はローカルY方向）
    DirectX::XMFLOAT3 center{ 0,0,0 };
    float halfHeight = 0.5f;   // 円柱部分の半分の高さ（両端に半球が付く）
    float radius = 0.25f;  // 半径

    CapsuleCollider() = default;
    CapsuleCollider(float hh = 0.5f, float r = 0.25f, bool st = false)
        : halfHeight(hh), radius(r) {
        isStatic = st;
    }

    ColliderType GetType() const override { return ColliderType::Capsule; }

    struct Capsule
    {
        DirectX::XMFLOAT3 a; // 下端半球の中心（ワールド）
        DirectX::XMFLOAT3 b; // 上端半球の中心（ワールド）
        float r;             // 半径（ワールド）
    };
    Capsule GetWorldCapsule() const;

    // BroadPhase 用のAABB
    AABB GetWorldAABB() override;
};
