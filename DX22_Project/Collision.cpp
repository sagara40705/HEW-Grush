// Collision.cpp
/*
#include "Collision.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include <algorithm>

using namespace DirectX;
static inline float clamp(float v, float mn, float mx) { return v < mn ? mn : (v > mx ? mx : v); }

namespace {

    inline bool AABBOverlap(const Collider::AABB& A, const Collider::AABB& B, Contact* out)
    {
        // 軸ごとの重なり量
        const float ox = std::min(A.max.x, B.max.x) - std::max(A.min.x, B.min.x);
        const float oy = std::min(A.max.y, B.max.y) - std::max(A.min.y, B.min.y);
        const float oz = std::min(A.max.z, B.max.z) - std::max(A.min.z, B.min.z);

        if (ox <= 0 || oy <= 0 || oz <= 0) return false;

        if (out) {
            // 最小侵入軸を選び、A←B 法線を作る
            // 中心位置で符号を決める
            const XMFLOAT3 cA{ (A.min.x + A.max.x) * 0.5f, (A.min.y + A.max.y) * 0.5f, (A.min.z + A.max.z) * 0.5f };
            const XMFLOAT3 cB{ (B.min.x + B.max.x) * 0.5f, (B.min.y + B.max.y) * 0.5f, (B.min.z + B.max.z) * 0.5f };
            Contact c{};

            if (ox <= oy && ox <= oz)
            {
                c.penetration = ox;
                c.normal = { (cA.x < cB.x) ? -1.0f : +1.0f, 0, 0 }; // A←B
            }
            else if (oy <= oz) 
            {
                c.penetration = oy;
                c.normal = { 0, (cA.y < cB.y) ? -1.0f : +1.0f, 0 };
            }
            else 
            {
                c.penetration = oz;
                c.normal = { 0, 0, (cA.z < cB.z) ? -1.0f : +1.0f };
            }
            *out = c;
        }
        return true;
    }

    inline bool SphereSphere(const SphereCollider::Sphere& A, const SphereCollider::Sphere& B, Contact* out)
    {
        const float dx = A.c.x - B.c.x, dy = A.c.y - B.c.y, dz = A.c.z - B.c.z;
        const float r = A.r + B.r;
        const float d2 = dx * dx + dy * dy + dz * dz;
        if (d2 > r * r) return false;
        if (out) {
            const float d = std::max(std::sqrt(d2), 1e-6f);
            out->normal = { dx / d, dy / d, dz / d };   // A←B
            out->penetration = r - d;
        }
        return true;
    }

    inline bool BoxSphere(const Collider::AABB& box, const SphereCollider::Sphere& sph, Contact* out)
    {
        const float cx = clamp(sph.c.x, box.min.x, box.max.x);
        const float cy = clamp(sph.c.y, box.min.y, box.max.y);
        const float cz = clamp(sph.c.z, box.min.z, box.max.z);
        const float dx = sph.c.x - cx, dy = sph.c.y - cy, dz = sph.c.z - cz;
        const float d2 = dx * dx + dy * dy + dz * dz;
        if (d2 > sph.r * sph.r) return false;
        if (out) {
            const float d = std::max(std::sqrt(d2), 1e-6f);
            out->normal = { dx / d, dy / d, dz / d };   // A(Box)←B(Sphere)
            out->penetration = sph.r - d;
        }
        return true;
    }

} // namespace

bool Collision::Test(Collider& a, Collider& b, Contact* out)
{
    if (!a.IsCollidableWith(b)) return false;

    const auto ta = a.GetType(), tb = b.GetType();

    if (ta == ColliderType::Box && tb == ColliderType::Box)
    {
        auto& A = static_cast<BoxCollider&>(a);
        auto& B = static_cast<BoxCollider&>(b);
        return AABBOverlap(A.GetWorldAABB(), B.GetWorldAABB(), out);
    }
    // ここに Sphere/Capsule 等を足していく

    if (ta == ColliderType::Sphere && tb == ColliderType::Sphere)
    {
        auto& A = static_cast<SphereCollider&>(a);
        auto& B = static_cast<SphereCollider&>(b);
        return SphereSphere(A.GetWorldSphere(), B.GetWorldSphere(),out);
    }
        



    return false;
}
*/
// 10/31

// Collision.cpp（Box / Sphere / Capsule 対応）
#include "Collision.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h" // ↓未導入なら後掲の雛形を追加してください
#include <algorithm>
#include <cmath>

using DirectX::XMFLOAT3;

// ---------- 小ユーティリティ ----------
static inline float  clamp1(float v, float mn, float mx) { return (v < mn) ? mn : ((v > mx) ? mx : v); }
static inline XMFLOAT3 sub(const XMFLOAT3& a, const XMFLOAT3& b) { return { a.x - b.x, a.y - b.y, a.z - b.z }; }
static inline XMFLOAT3 add(const XMFLOAT3& a, const XMFLOAT3& b) { return { a.x + b.x, a.y + b.y, a.z + b.z }; }
static inline XMFLOAT3 mul(const XMFLOAT3& a, float s) { return { a.x * s, a.y * s, a.z * s }; }
static inline float    dot3(const XMFLOAT3& a, const XMFLOAT3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static inline float    len2(const XMFLOAT3& a) { return dot3(a, a); }
static inline float    len(const XMFLOAT3& a) { return std::sqrt(len2(a)); }

static inline XMFLOAT3 normalizeSafe(const XMFLOAT3& v, const XMFLOAT3& fallback = { 0,1,0 })
{
    const float l2 = len2(v);
    if (l2 <= 1e-12f) return fallback;
    const float inv = 1.0f / std::sqrt(l2);
    return { v.x * inv, v.y * inv, v.z * inv };
}

// 点とAABBの最近接点
static inline XMFLOAT3 closestPtPointAABB(const XMFLOAT3& p, const XMFLOAT3& mn, const XMFLOAT3& mx)
{
    return { clamp1(p.x, mn.x, mx.x), clamp1(p.y, mn.y, mx.y), clamp1(p.z, mn.z, mx.z) };
}

// 点と線分の最近接点（pSeg= A + t*(B-A), t∈[0,1]）
static inline float closestPtPointSegment_t(const XMFLOAT3& p, const XMFLOAT3& a, const XMFLOAT3& b, XMFLOAT3* pSegOut)
{
    const XMFLOAT3 ab = sub(b, a);
    const XMFLOAT3 ap = sub(p, a);
    const float ab2 = len2(ab);
    float t = (ab2 > 1e-12f) ? dot3(ap, ab) / ab2 : 0.0f;
    t = clamp1(t, 0.0f, 1.0f);
    if (pSegOut) *pSegOut = add(a, mul(ab, t));
    return t;
}

// 線分同士の最近接点（Ericsonベース）。pA, pB を返す
static inline void closestPtSegmentSegment(const XMFLOAT3& p1, const XMFLOAT3& q1,
    const XMFLOAT3& p2, const XMFLOAT3& q2,
    XMFLOAT3* pA, XMFLOAT3* pB)
{
    const XMFLOAT3 d1 = sub(q1, p1);
    const XMFLOAT3 d2 = sub(q2, p2);
    const XMFLOAT3 r = sub(p1, p2);
    const float a = dot3(d1, d1); // Squared length of segment S1
    const float e = dot3(d2, d2); // Squared length of segment S2
    const float f = dot3(d2, r);

    float s, t;

    if (a <= 1e-12f && e <= 1e-12f) {
        // 両方点
        s = t = 0.0f;
        if (pA) *pA = p1;
        if (pB) *pB = p2;
        return;
    }
    if (a <= 1e-12f) {
        // S1 は点
        s = 0.0f;
        t = clamp1(f / e, 0.0f, 1.0f);
    }
    else {
        const float c = dot3(d1, r);
        if (e <= 1e-12f) {
            // S2 は点
            t = 0.0f;
            s = clamp1(-c / a, 0.0f, 1.0f);
        }
        else {
            const float b = dot3(d1, d2);
            const float denom = a * e - b * b;
            if (denom != 0.0f)
                s = clamp1((b * f - c * e) / denom, 0.0f, 1.0f);
            else
                s = 0.0f; // 平行

            const float tnom = b * s + f;
            if (tnom < 0.0f) { t = 0.0f; s = clamp1(-c / a, 0.0f, 1.0f); }
            else if (tnom > e) { t = 1.0f; s = clamp1((b - c) / a, 0.0f, 1.0f); }
            else                   t = tnom / e;
        }
    }
    if (pA) *pA = add(p1, mul(d1, s));
    if (pB) *pB = add(p2, mul(d2, t));
}

// 線分とAABBの最近接（簡易近似：端点＋中点の3サンプルから最良を採用）
// ※必要十分の精度。厳密距離が必要なら将来差し替え。
static inline void closestPtSegmentAABB_approx(const XMFLOAT3& a, const XMFLOAT3& b,
    const XMFLOAT3& mn, const XMFLOAT3& mx,
    XMFLOAT3* outSeg, XMFLOAT3* outBox)
{
    const XMFLOAT3 mid{ (a.x + b.x) * 0.5f, (a.y + b.y) * 0.5f, (a.z + b.z) * 0.5f };
    const XMFLOAT3 candP[3] = { a, mid, b };
    XMFLOAT3 bestP = a, bestQ = closestPtPointAABB(a, mn, mx);
    float bestD2 = len2(sub(bestP, bestQ));

    for (int i = 1; i < 3; ++i) {
        XMFLOAT3 q = closestPtPointAABB(candP[i], mn, mx);
        float d2 = len2(sub(candP[i], q));
        if (d2 < bestD2) { bestD2 = d2; bestP = candP[i]; bestQ = q; }
    }
    if (outSeg) *outSeg = bestP;
    if (outBox) *outBox = bestQ;
}

namespace {
    using CT = ColliderType;
    using Fn = bool(*)( Collider&, Collider&, Contact*);
    Fn gTable[3][3];

    struct Init {
        Init() {
            // Box / Sphere 既存
            gTable[(int)CT::Box][(int)CT::Box] = Collision::TestBoxBox;
            gTable[(int)CT::Sphere][(int)CT::Sphere] = Collision::TestSphereSphere;
            gTable[(int)CT::Box][(int)CT::Sphere] = Collision::TestBoxSphere;
            gTable[(int)CT::Sphere][(int)CT::Box] = [](Collider& a, Collider& b, Contact* o) {
                bool hit = Collision::TestBoxSphere(b, a, o);
                if (hit && o) { o->normal.x *= -1.f; o->normal.y *= -1.f; o->normal.z *= -1.f; } // A←B を維持
                return hit;
                };
            // Capsule 追加
            gTable[(int)CT::Capsule][(int)CT::Sphere] = Collision::TestCapsuleSphere;
            gTable[(int)CT::Sphere][(int)CT::Capsule] = [](Collider& a, Collider& b, Contact* o) {
                bool hit = Collision::TestCapsuleSphere(b, a, o);
                if (hit && o) { o->normal.x *= -1.f; o->normal.y *= -1.f; o->normal.z *= -1.f; }
                return hit;
                };
            gTable[(int)CT::Capsule][(int)CT::Box] = Collision::TestCapsuleBox;
            gTable[(int)CT::Box][(int)CT::Capsule] = [](Collider& a, Collider& b, Contact* o) {
                bool hit = Collision::TestCapsuleBox(b, a, o);
                if (hit && o) { o->normal.x *= -1.f; o->normal.y *= -1.f; o->normal.z *= -1.f; }
                return hit;
                };
            gTable[(int)CT::Capsule][(int)CT::Capsule] = Collision::TestCapsuleCapsule;
        }
    } s_init;
} // anon

bool Collision::Test(Collider& a, Collider& b, Contact* out)
{
    // レイヤマスク
    if (!a.IsCollidableWith(b)) return false;

    auto* fn = gTable[(int)a.GetType()][(int)b.GetType()];
    return fn ? fn(a, b, out) : false;
}

// ---------------- 既存（AABB/Sphere） ----------------

bool Collision::TestBoxBox(Collider& a, Collider& b, Contact* out)
{
    const auto& A = static_cast<BoxCollider&>(a).GetWorldAABB();
    const auto& B = static_cast<BoxCollider&>(b).GetWorldAABB();

    const bool ox = (A.min.x <= B.max.x) && (B.min.x <= A.max.x);
    const bool oy = (A.min.y <= B.max.y) && (B.min.y <= A.max.y);
    const bool oz = (A.min.z <= B.max.z) && (B.min.z <= A.max.z);
    if (!(ox && oy && oz)) return false;

    if (out) {
        float dx = std::min(A.max.x - B.min.x, B.max.x - A.min.x);
        float dy = std::min(A.max.y - B.min.y, B.max.y - A.min.y);
        float dz = std::min(A.max.z - B.min.z, B.max.z - A.min.z);
        if (dx < dy && dx < dz) *out = Contact{ { (A.max.x < B.max.x) ? -1.f : 1.f, 0, 0 }, dx };
        else if (dy < dz)       *out = Contact{ { 0, (A.max.y < B.max.y) ? -1.f : 1.f, 0 }, dy };
        else                    *out = Contact{ { 0, 0, (A.max.z < B.max.z) ? -1.f : 1.f }, dz };
    }
    return true;
}

bool Collision::TestSphereSphere(Collider& a, Collider& b, Contact* out)
{
    const auto& SA = static_cast<const SphereCollider&>(a);
    const auto& SB = static_cast<const SphereCollider&>(b);
    const auto A = SA.GetWorldSphere();
    const auto B = SB.GetWorldSphere();

    const float dx = A.c.x - B.c.x;
    const float dy = A.c.y - B.c.y;
    const float dz = A.c.z - B.c.z;
    const float r = A.r + B.r;
    const float d2 = dx * dx + dy * dy + dz * dz;
    if (d2 > r * r) return false;

    if (out) {
        const float d = std::max(std::sqrt(d2), 1e-6f);
        out->normal = { dx / d, dy / d, dz / d }; // A←B
        out->penetration = r - d;
    }
    return true;
}

bool Collision::TestBoxSphere(Collider& box, Collider& sph, Contact* out)
{
    const auto& B = static_cast<BoxCollider&>(box).GetWorldAABB();
    const auto& S = static_cast<SphereCollider&>(sph).GetWorldSphere();

    const float cx = clamp1(S.c.x, B.min.x, B.max.x);
    const float cy = clamp1(S.c.y, B.min.y, B.max.y);
    const float cz = clamp1(S.c.z, B.min.z, B.max.z);
    const float dx = cx - S.c.x;
    const float dy = cy - S.c.y;
    const float dz = cz - S.c.z;
    const float d2 = dx * dx + dy * dy + dz * dz;
    if (d2 > S.r * S.r) return false;

    if (out) 
    {
        const float d = std::max(std::sqrt(d2), 1e-6f);
        out->normal = { dx / d, dy / d, dz / d }; // A=Box, B=Sphere だが法線は「A←B」になるよう dx= S→closest(Box)
        out->penetration = S.r - d;
    }
    return true;
}

// ---------------- Capsule 系 ----------------

bool Collision::TestCapsuleSphere(Collider& cap, Collider& sph, Contact* out)
{
    const auto& C = static_cast<const CapsuleCollider&>(cap).GetWorldCapsule();
    const auto& S = static_cast<const SphereCollider&>(sph).GetWorldSphere();

    XMFLOAT3 pSeg{};
    closestPtPointSegment_t(S.c, C.a, C.b, &pSeg);
    const XMFLOAT3 v = sub(pSeg, S.c);       // B(=Sphere) → A(=Capsule)
    const float    r = C.r + S.r;
    const float    d2 = len2(v);
    if (d2 > r * r) return false;

    if (out) {
        const float d = std::max(std::sqrt(d2), 1e-6f);
        out->normal = { v.x / d, v.y / d, v.z / d }; // A←B（B→A）
        out->penetration = r - d;
    }
    return true;
}

bool Collision::TestCapsuleCapsule(Collider& a, Collider& b, Contact* out)
{
    const auto& A = static_cast<const CapsuleCollider&>(a).GetWorldCapsule();
    const auto& B = static_cast<const CapsuleCollider&>(b).GetWorldCapsule();

    XMFLOAT3 pA{}, pB{};
    closestPtSegmentSegment(A.a, A.b, B.a, B.b, &pA, &pB);
    const XMFLOAT3 v = sub(pA, pB);          // B → A
    const float    r = A.r + B.r;
    const float    d2 = len2(v);
    if (d2 > r * r) return false;

    if (out) {
        const float d = std::max(std::sqrt(d2), 1e-6f);
        out->normal = { v.x / d, v.y / d, v.z / d }; // A←B
        out->penetration = r - d;
    }
    return true;
}

bool Collision::TestCapsuleBox(Collider& cap, Collider& box, Contact* out)
{
    const auto& C = static_cast<CapsuleCollider&>(cap).GetWorldCapsule();
    const auto& B = static_cast<BoxCollider&>(box).GetWorldAABB();

    XMFLOAT3 pSeg{}, pBox{};
    closestPtSegmentAABB_approx(C.a, C.b, B.min, B.max, &pSeg, &pBox);

    XMFLOAT3 v = sub(pSeg, pBox);   // B(=Box) → A(=Capsule)
    float    d2 = len2(v);
    const float r = C.r;

    if (d2 > r * r) return false;

    if (out) {
        float d = std::sqrt(std::max(d2, 0.0f));
        if (d < 1e-6f) {
            // 退避：点が面上/内部に落ちた場合は最小押し出し軸を採用
            const XMFLOAT3 centerBox{
                (B.min.x + B.max.x) * 0.5f,
                (B.min.y + B.max.y) * 0.5f,
                (B.min.z + B.max.z) * 0.5f
            };
            const XMFLOAT3 q = pSeg; // 線分側の点を基準に
            const float dx = std::min(std::fabs(q.x - B.min.x), std::fabs(B.max.x - q.x));
            const float dy = std::min(std::fabs(q.y - B.min.y), std::fabs(B.max.y - q.y));
            const float dz = std::min(std::fabs(q.z - B.min.z), std::fabs(B.max.z - q.z));
            if (dx <= dy && dx <= dz) out->normal = { (q.x > centerBox.x) ? 1.f : -1.f, 0, 0 };
            else if (dy <= dz)        out->normal = { 0, (q.y > centerBox.y) ? 1.f : -1.f, 0 };
            else                      out->normal = { 0, 0, (q.z > centerBox.z) ? 1.f : -1.f };
            out->penetration = r; // 簡易：フレーム反復で解消される前提
        }
        else {
            out->normal = { v.x / d, v.y / d, v.z / d }; // A←B
            out->penetration = r - d;
        }
    }
    return true;
}
