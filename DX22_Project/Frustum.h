#pragma once
#include "Vector3.h"

struct Plane
{
    DirectX::XMFLOAT4 eq; // ax + by + cz + d = 0
};

struct Frustum
{
    Plane planes[6]; // 左, 右, 下, 上, ニア, ファー

    void BuildFromMatrix(const DirectX::XMFLOAT4X4& vp);
    bool IntersectsSphere(const DirectX::XMFLOAT3& center, float radius) const;
};
