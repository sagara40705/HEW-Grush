// Collision.h
#pragma once
#include "Collider.h"

namespace Collision
{
    // true=ヒット。out が非nullなら Contact を返す（A←B法線/貫通量）
    bool Test(Collider& a, Collider& b, Contact* out = nullptr);

    // 実装（内部用）
    bool TestBoxBox(Collider& a, Collider& b, Contact* out);
    bool TestSphereSphere(Collider& a, Collider& b, Contact* out);
    bool TestBoxSphere(Collider& box, Collider& sph, Contact* out);

    // 追加（Capsule系）
    bool TestCapsuleSphere(Collider& cap, Collider& sph, Contact* out);
    bool TestCapsuleCapsule(Collider& a, Collider& b, Contact* out);
    bool TestCapsuleBox(Collider& cap, Collider& box, Contact* out);
}
