#pragma once
#include "Collider.h"
#include "Vector3.h"

class BoxCollider : public Collider
{
public:
    // ローカル座標系の中心・サイズ
    Vector3 center{ 0, 0, 0 };
    Vector3 size{ 1, 1, 1 };

public:
    //BoxCollider() = default;
    void Start() override;
    BoxCollider(bool st = true, Vector3 c = { 0, 0, 0 }, Vector3 s = { 1, 1, 1 }) : center(c), size(s){ isStatic = st; }

    ColliderType GetType() const override { return ColliderType::Box; }

    // ワールド空間のAABB（Transform反映）
    AABB GetWorldAABB() override;
};
