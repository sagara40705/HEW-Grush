#include "BoxCollider.h"
#include "Transform.h"
#include <cassert>
#include <algorithm>
#include <cmath>
using namespace DirectX;


static inline XMVECTOR LoadF3(const XMFLOAT3& v) { return XMLoadFloat3(&v); }
static inline XMVECTOR LoadV3(const Vector3& v) { return XMVectorSet(v.x, v.y, v.z, 1.0f); }

BoxCollider::AABB BoxCollider::GetWorldAABB()
{
    auto* tf = GetTransform();
    assert(tf && "BoxCollider requires Transform");

    const XMFLOAT4X4 M4 = tf->GetMatrix(); // S*R*T
    const XMMATRIX   M = XMLoadFloat4x4(&M4);

    const XMFLOAT3 c = center;
    const XMFLOAT3 e = { size.x * 0.5f, size.y * 0.5f, size.z * 0.5f };

    const XMFLOAT3 localCorners[8] =
    {
        {c.x - e.x, c.y - e.y, c.z - e.z},
        {c.x + e.x, c.y - e.y, c.z - e.z},
        {c.x - e.x, c.y + e.y, c.z - e.z},
        {c.x + e.x, c.y + e.y, c.z - e.z},
        {c.x - e.x, c.y - e.y, c.z + e.z},
        {c.x + e.x, c.y - e.y, c.z + e.z},
        {c.x - e.x, c.y + e.y, c.z + e.z},
        {c.x + e.x, c.y + e.y, c.z + e.z},
    };

    XMFLOAT3 mn{ +FLT_MAX, +FLT_MAX, +FLT_MAX };
    XMFLOAT3 mx{ -FLT_MAX, -FLT_MAX, -FLT_MAX };

    for (auto& p : localCorners)
    {
        XMVECTOR v = XMVector3Transform(LoadF3(p), M);
        XMFLOAT3 w; XMStoreFloat3(&w, v);
        mn.x = std::min(mn.x, w.x); mn.y = std::min(mn.y, w.y); mn.z = std::min(mn.z, w.z);
        mx.x = std::max(mx.x, w.x); mx.y = std::max(mx.y, w.y); mx.z = std::max(mx.z, w.z);
    }
    return { mn, mx };
}
