#include "Frustum.h"
using namespace DirectX;

static inline void NormalizePlane(Plane& p)
{
    XMVECTOR v = XMLoadFloat4(&p.eq);
    v = XMPlaneNormalize(v);
    XMStoreFloat4(&p.eq, v);
}

void Frustum::BuildFromMatrix(const XMFLOAT4X4& vp)
{
    XMMATRIX M = XMLoadFloat4x4(&vp);

    XMVECTOR row0 = M.r[0];
    XMVECTOR row1 = M.r[1];
    XMVECTOR row2 = M.r[2];
    XMVECTOR row3 = M.r[3];

    // 左
    XMStoreFloat4(&planes[0].eq, row3 + row0);
    // 右
    XMStoreFloat4(&planes[1].eq, row3 - row0);
    // 下
    XMStoreFloat4(&planes[2].eq, row3 + row1);
    // 上
    XMStoreFloat4(&planes[3].eq, row3 - row1);
    // ニア
    XMStoreFloat4(&planes[4].eq, row3 + row2);
    // ファー
    XMStoreFloat4(&planes[5].eq, row3 - row2);

    for (int i = 0; i < 6; i++)
        NormalizePlane(planes[i]);
}

bool Frustum::IntersectsSphere(const XMFLOAT3& center, float radius) const
{
    using namespace DirectX;
    XMVECTOR c = XMLoadFloat3(&center);

    for (int i = 0; i < 6; i++)
    {
        XMVECTOR p = XMLoadFloat4(&planes[i].eq);
        float dist = XMVectorGetX(XMPlaneDotCoord(p, c));

        if (dist < -radius)
        {
            // 完全に外
            return false;
        }
    }
    return true;
}
