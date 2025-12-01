#include "CapsuleCollider.h"
#include "Transform.h"
#include <cassert>
#include <algorithm>
using namespace DirectX;

static inline float max3(float a, float b, float c) { return std::max(a, std::max(b, c)); }

CapsuleCollider::Capsule CapsuleCollider::GetWorldCapsule() const
{
    auto* tf = GetTransform();
    assert(tf && "CapsuleCollider requires Transform");

    const XMFLOAT3 pos = tf->GetPosition();
    const XMFLOAT3 scl = tf->GetScale();

    // 今はY軸専用。半径は最大スケールで拡大、長さはYスケールで拡大
    const float rW = std::abs(radius) * max3(std::abs(scl.x), std::abs(scl.y), std::abs(scl.z));
    const float hhW = std::abs(halfHeight) * std::abs(scl.y);

    Capsule c;
    c.a = { pos.x + center.x, pos.y + center.y - hhW, pos.z + center.z }; // 下端
    c.b = { pos.x + center.x, pos.y + center.y + hhW, pos.z + center.z }; // 上端
    c.r = rW;
    return c;
}

CapsuleCollider::AABB CapsuleCollider::GetWorldAABB()
{
    const Capsule c = GetWorldCapsule();

    XMFLOAT3 mn{
        std::min(c.a.x, c.b.x) - c.r,
        std::min(c.a.y, c.b.y) - c.r,
        std::min(c.a.z, c.b.z) - c.r
    };
    XMFLOAT3 mx{
        std::max(c.a.x, c.b.x) + c.r,
        std::max(c.a.y, c.b.y) + c.r,
        std::max(c.a.z, c.b.z) + c.r
    };
    return { mn, mx };
}
