// SphereCollider.cpp
#include "SphereCollider.h"
#include "Transform.h"
#include <cassert>
#include <algorithm>
#include <cmath>
using namespace DirectX;

static inline float AbsMax3(float x, float y, float z) 
{
    return std::max(std::max(std::fabs(x), std::fabs(y)), std::fabs(z));
}

SphereCollider::Sphere SphereCollider::GetWorldSphere() const
{
    auto* tf = GetTransform();
    assert(tf && "SphereCollider requires Transform");

    const XMFLOAT3 pos = tf->GetPosition();
    const XMFLOAT3 scl = tf->GetScale();

    XMFLOAT3 cW{
        pos.x + center.x * scl.x,
        pos.y + center.y * scl.y,
        pos.z + center.z * scl.z
    };
    const float s = AbsMax3(scl.x, scl.y, scl.z); // ”ñ“™•û‚ÍÅ‘å²‚Å•Ûç“I‚É
    const float rW = std::fabs(radius) * s;

    return Sphere{ cW, rW };
}

SphereCollider::AABB SphereCollider::GetWorldAABB()
{
    const auto sp = GetWorldSphere();
    return AABB{
        { sp.c.x - sp.r, sp.c.y - sp.r, sp.c.z - sp.r },
        { sp.c.x + sp.r, sp.c.y + sp.r, sp.c.z + sp.r }
    };
}
