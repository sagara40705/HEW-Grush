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


//10/31
//namespace Collision
//{
//    // true=ヒット。out が非 null なら接触情報を返す（最小限）
//    bool Test(const Collider& a, const Collider& b, Contact* out = nullptr);
//
//    // 実装（内部用）
//    bool TestBoxBox(const Collider& a, const Collider& b, Contact* out);
//    bool TestSphereSphere(const Collider& a, const Collider& b, Contact* out);
//    bool TestBoxSphere(const Collider& box, const Collider& sph, Contact* out);
//
//    // 追加（Capsule系）
//    bool TestCapsuleSphere(const Collider& cap, const Collider& sph, Contact* out);
//    bool TestCapsuleCapsule(const Collider& a, const Collider& b, Contact* out);
//    bool TestCapsuleBox(const Collider& cap, const Collider& box, Contact* out);
//}
