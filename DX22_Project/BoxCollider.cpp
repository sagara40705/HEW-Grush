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


//10/31
//BoxCollider::AABB BoxCollider::GetWorldAABB() const
//{
//    auto* tf = GetTransform();
//    assert(tf && "BoxCollider requires Transform");
//
//    const XMFLOAT3 pos = tf->GetPosition();
//    const XMFLOAT3 scl = tf->GetScale();
//
//    // 1) ローカル半サイズ（スケール反映、非等方対応）
//    const XMFLOAT3 he{
//        std::fabs(size.x * scl.x) * 0.5f,
//        std::fabs(size.y * scl.y) * 0.5f,
//        std::fabs(size.z * scl.z) * 0.5f
//    };
//
//    // 2) 回転行列（Transformの回転のみ。スケールは既に反映済み）
//    const XMMATRIX R = XMMatrixRotationQuaternion(tf->GetRotationQuaternionXM());
//
//    // 3) ローカル中心 offset を回してワールド中心へ
//    const XMVECTOR cLocal = XMVectorSet(center.x * scl.x, center.y * scl.y, center.z * scl.z, 0.0f);
//    const XMVECTOR cOffW = XMVector3TransformNormal(cLocal, R);
//
//    XMFLOAT3 off{};
//    XMStoreFloat3(&off, cOffW);
//
//    XMFLOAT3 cW{ pos.x + off.x, pos.y + off.y, pos.z + off.z };
//
//    // 4) |R| * he でワールド軸方向の半径を求める
//    //    XMMATRIX は行ベクトル r[0], r[1], r[2] が x,y,z 行に対応
//    const float m00 = std::fabs(R.r[0].m128_f32[0]);
//    const float m01 = std::fabs(R.r[0].m128_f32[1]);
//    const float m02 = std::fabs(R.r[0].m128_f32[2]);
//
//    const float m10 = std::fabs(R.r[1].m128_f32[0]);
//    const float m11 = std::fabs(R.r[1].m128_f32[1]);
//    const float m12 = std::fabs(R.r[1].m128_f32[2]);
//
//    const float m20 = std::fabs(R.r[2].m128_f32[0]);
//    const float m21 = std::fabs(R.r[2].m128_f32[1]);
//    const float m22 = std::fabs(R.r[2].m128_f32[2]);
//
//    const float ex = m00 * he.x + m01 * he.y + m02 * he.z;
//    const float ey = m10 * he.x + m11 * he.y + m12 * he.z;
//    const float ez = m20 * he.x + m21 * he.y + m22 * he.z;
//
//    return AABB{
//        { cW.x - ex, cW.y - ey, cW.z - ez },
//        { cW.x + ex, cW.y + ey, cW.z + ez }
//    };
//}

//BoxCollider::AABB BoxCollider::GetWorldAABB() const
//{
//    auto* tf = GetTransform();
//    assert(tf && "BoxCollider requires Transform");
//
//    const XMFLOAT3 pos = tf->GetPosition();
//    const XMFLOAT3 scl = tf->GetScale();
//
//    // 非等方スケールを考慮
//    const float hx = std::fabs(size.x * scl.x) * 0.5f;
//    const float hy = std::fabs(size.y * scl.y) * 0.5f;
//    const float hz = std::fabs(size.z * scl.z) * 0.5f;
//
//    // ローカル中心 → ワールド中心（回転は今は未対応）
//    XMFLOAT3 cW{
//        pos.x + center.x * scl.x,
//        pos.y + center.y * scl.y,
//        pos.z + center.z * scl.z
//    };
//
//    return AABB{
//        { cW.x - hx, cW.y - hy, cW.z - hz },
//        { cW.x + hx, cW.y + hy, cW.z + hz }
//    };
//}
