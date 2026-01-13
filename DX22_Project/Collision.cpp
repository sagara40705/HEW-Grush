// Collision.cpp（Vector3 ベース版：Box / Sphere / Capsule 対応）
#include "Collision.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "Vector3.h"

#include <algorithm>
#include <cmath>

using DirectX::XMFLOAT3;

// ---------- 小ユーティリティ（Vector3 ベース） ----------

static inline float clamp1(float v, float mn, float mx)
{
    return (v < mn) ? mn : ((v > mx) ? mx : v);
}

// XMFLOAT3 → Vector3
static inline Vector3 V(const XMFLOAT3& f) noexcept
{
    return Vector3(f);
}

// 安全正規化
static inline Vector3 normalizeSafe(const Vector3& v, const Vector3& fallback = Vector3::up()) noexcept
{
    const float sq = v.sqrMagnitude();
    if (sq <= 1e-12f) return fallback;
    const float inv = 1.0f / std::sqrt(sq);
    return v * inv;
}

// 点と AABB の最近接点（AABB は XMFLOAT3 を保持しているのでそこだけ変換）
static inline Vector3 closestPtPointAABB(const Vector3& p, const XMFLOAT3& mnF, const XMFLOAT3& mxF) noexcept
{
    const Vector3 mn = V(mnF);
    const Vector3 mx = V(mxF);
    return {
        clamp1(p.x, mn.x, mx.x),
        clamp1(p.y, mn.y, mx.y),
        clamp1(p.z, mn.z, mx.z)
    };
}

// 点と線分の最近接点 pSeg = a + t*(b-a), t∈[0,1] を返す（t も返す）
static inline float closestPtPointSegment_t(const Vector3& p,
    const Vector3& a, const Vector3& b,
    Vector3* pSegOut) noexcept
{
    const Vector3 ab = b - a;
    const Vector3 ap = p - a;
    const float ab2 = ab.sqrMagnitude();

    float t = 0.0f;
    if (ab2 > 1e-12f)
    {
        t = Vector3::Dot(ap, ab) / ab2;
        t = clamp1(t, 0.0f, 1.0f);
    }

    if (pSegOut)
    {
        *pSegOut = a + ab * t;
    }
    return t;
}

// 線分同士の最近接点（Ericson ベース）pA, pB を返す
static inline void closestPtSegmentSegment(
    const Vector3& p1, const Vector3& q1,
    const Vector3& p2, const Vector3& q2,
    Vector3* pA, Vector3* pB) noexcept
{
    const Vector3 d1 = q1 - p1;
    const Vector3 d2 = q2 - p2;
    const Vector3 r = p1 - p2;
    const float a = d1.sqrMagnitude();      // |d1|^2
    const float e = d2.sqrMagnitude();      // |d2|^2
    const float f = Vector3::Dot(d2, r);

    float s, t;
    const float EPS = 1e-12f;

    if (a <= EPS && e <= EPS)
    {
        // 両方点
        s = t = 0.0f;
        if (pA) *pA = p1;
        if (pB) *pB = p2;
        return;
    }

    if (a <= EPS)
    {
        // S1 は点
        s = 0.0f;
        t = clamp1(f / e, 0.0f, 1.0f);
    }
    else
    {
        const float c = Vector3::Dot(d1, r);
        if (e <= EPS)
        {
            // S2 は点
            t = 0.0f;
            s = clamp1(-c / a, 0.0f, 1.0f);
        }
        else
        {
            const float b = Vector3::Dot(d1, d2);
            const float denom = a * e - b * b;

            if (std::fabs(denom) > EPS)
                s = clamp1((b * f - c * e) / denom, 0.0f, 1.0f);
            else
                s = 0.0f; // 平行

            const float tnom = b * s + f;
            if (tnom < 0.0f)
            {
                t = 0.0f;
                s = clamp1(-c / a, 0.0f, 1.0f);
            }
            else if (tnom > e)
            {
                t = 1.0f;
                s = clamp1((b - c) / a, 0.0f, 1.0f);
            }
            else
            {
                t = tnom / e;
            }
        }
    }

    if (pA) *pA = p1 + d1 * s;
    if (pB) *pB = p2 + d2 * t;
}

// 線分と AABB の最近接（端点＋中点の 3 サンプルから最良を採用する簡易近似）
static inline void closestPtSegmentAABB_approx(
    const Vector3& a, const Vector3& b,
    const XMFLOAT3& mnF, const XMFLOAT3& mxF,
    Vector3* outSeg, Vector3* outBox) noexcept
{
    const Vector3 mid{ (a.x + b.x) * 0.5f, (a.y + b.y) * 0.5f, (a.z + b.z) * 0.5f };
    const Vector3 candP[3] = { a, mid, b };

    Vector3 bestP = a;
    Vector3 bestQ = closestPtPointAABB(a, mnF, mxF);
    float bestD2 = (bestP - bestQ).sqrMagnitude();

    for (int i = 1; i < 3; ++i)
    {
        Vector3 q = closestPtPointAABB(candP[i], mnF, mxF);
        float d2 = (candP[i] - q).sqrMagnitude();
        if (d2 < bestD2)
        {
            bestD2 = d2;
            bestP = candP[i];
            bestQ = q;
        }
    }

    if (outSeg) *outSeg = bestP;
    if (outBox) *outBox = bestQ;
}

// ---------- ディスパッチテーブル ----------

namespace
{
    using CT = ColliderType;
    using Fn = bool(*)(Collider&, Collider&, Contact*);
    Fn gTable[3][3];

    struct Init
    {
        Init()
        {
            // Box / Sphere
            gTable[(int)CT::Box][(int)CT::Box] = Collision::TestBoxBox;
            gTable[(int)CT::Sphere][(int)CT::Sphere] = Collision::TestSphereSphere;

            gTable[(int)CT::Box][(int)CT::Sphere] = Collision::TestBoxSphere;
            gTable[(int)CT::Sphere][(int)CT::Box] = [](Collider& a, Collider& b, Contact* o)
                {
                    bool hit = Collision::TestBoxSphere(b, a, o);
                    if (hit && o)
                    {
                        o->normal.x *= -1.f;
                        o->normal.y *= -1.f;
                        o->normal.z *= -1.f; // A←B を維持
                    }
                    return hit;
                };

            // Capsule 系
            gTable[(int)CT::Capsule][(int)CT::Sphere] = Collision::TestCapsuleSphere;
            gTable[(int)CT::Sphere][(int)CT::Capsule] = [](Collider& a, Collider& b, Contact* o)
                {
                    bool hit = Collision::TestCapsuleSphere(b, a, o);
                    if (hit && o)
                    {
                        o->normal.x *= -1.f;
                        o->normal.y *= -1.f;
                        o->normal.z *= -1.f;
                    }
                    return hit;
                };

            gTable[(int)CT::Capsule][(int)CT::Box] = Collision::TestCapsuleBox;
            gTable[(int)CT::Box][(int)CT::Capsule] = [](Collider& a, Collider& b, Contact* o)
                {
                    bool hit = Collision::TestCapsuleBox(b, a, o);
                    if (hit && o)
                    {
                        o->normal.x *= -1.f;
                        o->normal.y *= -1.f;
                        o->normal.z *= -1.f;
                    }
                    return hit;
                };

            gTable[(int)CT::Capsule][(int)CT::Capsule] = Collision::TestCapsuleCapsule;
        }
    } s_init;
} // anonymous namespace

// =====================================================
// Public API
// =====================================================

bool Collision::Test(Collider& a, Collider& b, Contact* out)
{
    // レイヤマスク
    if (!a.IsCollidableWith(b)) return false;

    auto* fn = gTable[(int)a.GetType()][(int)b.GetType()];
    return fn ? fn(a, b, out) : false;
}

// =====================================================
// Box / Sphere
// =====================================================

bool Collision::TestBoxBox(Collider& a, Collider& b, Contact* out)
{
    const auto& A = static_cast<BoxCollider&>(a).GetWorldAABB();
    const auto& B = static_cast<BoxCollider&>(b).GetWorldAABB();

    const bool ox = (A.min.x <= B.max.x) && (B.min.x <= A.max.x);
    const bool oy = (A.min.y <= B.max.y) && (B.min.y <= A.max.y);
    const bool oz = (A.min.z <= B.max.z) && (B.min.z <= A.max.z);
    if (!(ox && oy && oz)) return false;

    if (out)
    {
        const float dx = std::min(A.max.x - B.min.x, B.max.x - A.min.x);
        const float dy = std::min(A.max.y - B.min.y, B.max.y - A.min.y);
        const float dz = std::min(A.max.z - B.min.z, B.max.z - A.min.z);

        if (dx < dy && dx < dz)
        {
            out->normal = { (A.max.x < B.max.x) ? -1.f : 1.f, 0, 0 };
            out->penetration = dx;
        }
        else if (dy < dz)
        {
            out->normal = { 0, (A.max.y < B.max.y) ? -1.f : 1.f, 0 };
            out->penetration = dy;
        }
        else
        {
            out->normal = { 0, 0, (A.max.z < B.max.z) ? -1.f : 1.f };
            out->penetration = dz;
        }
    }
    return true;
}

bool Collision::TestSphereSphere(Collider& a, Collider& b, Contact* out)
{
    const auto& SA = static_cast<const SphereCollider&>(a);
    const auto& SB = static_cast<const SphereCollider&>(b);
    const auto A = SA.GetWorldSphere();
    const auto B = SB.GetWorldSphere();

    const Vector3 cA = V(A.c);
    const Vector3 cB = V(B.c);
    const Vector3 diff = cA - cB;
    const float r = A.r + B.r;
    const float d2 = diff.sqrMagnitude();

    if (d2 > r * r) return false;

    if (out)
    {
        const float d = std::max(std::sqrt(d2), 1e-6f);
        const Vector3 n = diff * (1.0f / d); // A←B
        out->normal = n;                     // XMFLOAT3 へ暗黙変換
        out->penetration = r - d;
    }
    return true;
}

bool Collision::TestBoxSphere(Collider& box, Collider& sph, Contact* out)
{
    const auto& B = static_cast<BoxCollider&>(box).GetWorldAABB();
    const auto& S = static_cast<SphereCollider&>(sph).GetWorldSphere();

    const Vector3 cS = V(S.c);
    const Vector3 closest{
        clamp1(cS.x, B.min.x, B.max.x),
        clamp1(cS.y, B.min.y, B.max.y),
        clamp1(cS.z, B.min.z, B.max.z)
    };

    const Vector3 v = closest - cS;              // B(=Box) ← B(=Sphere) の方向（Sphere→Box）
    const float d2 = v.sqrMagnitude();

    if (d2 > S.r * S.r) return false;

    if (out)
    {
        const float d = std::max(std::sqrt(d2), 1e-6f);
        const Vector3 n = v * (1.0f / d);        // A(Box) ← B(Sphere)
        out->normal = n;
        out->penetration = S.r - d;
    }
    return true;
}

// =====================================================
// Capsule 系
// =====================================================

bool Collision::TestCapsuleSphere(Collider& cap, Collider& sph, Contact* out)
{
    const auto& C = static_cast<const CapsuleCollider&>(cap).GetWorldCapsule();
    const auto& S = static_cast<const SphereCollider&>(sph).GetWorldSphere();

    const Vector3 a = V(C.a);
    const Vector3 b = V(C.b);
    const Vector3 cS = V(S.c);

    Vector3 pSeg{};
    closestPtPointSegment_t(cS, a, b, &pSeg);

    const Vector3 v = pSeg - cS;       // B(=Sphere) → A(=Capsule)
    const float   r = C.r + S.r;
    const float   d2 = v.sqrMagnitude();

    if (d2 > r * r) return false;

    if (out)
    {
        const float d = std::max(std::sqrt(d2), 1e-6f);
        const Vector3 n = v * (1.0f / d); // A←B
        out->normal = n;
        out->penetration = r - d;
    }
    return true;
}

bool Collision::TestCapsuleCapsule(Collider& a, Collider& b, Contact* out)
{
    const auto& A = static_cast<const CapsuleCollider&>(a).GetWorldCapsule();
    const auto& B = static_cast<const CapsuleCollider&>(b).GetWorldCapsule();

    const Vector3 a0 = V(A.a);
    const Vector3 a1 = V(A.b);
    const Vector3 b0 = V(B.a);
    const Vector3 b1 = V(B.b);

    Vector3 pA{}, pB{};
    closestPtSegmentSegment(a0, a1, b0, b1, &pA, &pB);

    const Vector3 v = pA - pB;         // B → A
    const float   r = A.r + B.r;
    const float   d2 = v.sqrMagnitude();

    if (d2 > r * r) return false;

    if (out)
    {
        const float d = std::max(std::sqrt(d2), 1e-6f);
        const Vector3 n = v * (1.0f / d); // A←B
        out->normal = n;
        out->penetration = r - d;
    }
    return true;
}

bool Collision::TestCapsuleBox(Collider& cap, Collider& box, Contact* out)
{
    const auto& C = static_cast<CapsuleCollider&>(cap).GetWorldCapsule();
    const auto& B = static_cast<BoxCollider&>(box).GetWorldAABB();

    const Vector3 a = V(C.a);
    const Vector3 b = V(C.b);

    Vector3 pSeg{}, pBox{};
    closestPtSegmentAABB_approx(a, b, B.min, B.max, &pSeg, &pBox);

    Vector3 v = pSeg - pBox;      // B(=Box) → A(=Capsule)
    float   d2 = v.sqrMagnitude();
    const float r = C.r;

    if (d2 > r * r) return false;

    if (out)
    {
        float d = std::sqrt(std::max(d2, 0.0f));
        if (d < 1e-6f)
        {
            // 退避：点が面上/内部に落ちた場合は最小押し出し軸を採用
            const Vector3 centerBox{
                (B.min.x + B.max.x) * 0.5f,
                (B.min.y + B.max.y) * 0.5f,
                (B.min.z + B.max.z) * 0.5f
            };
            const Vector3 q = pSeg;

            const float dx = std::min(std::fabs(q.x - B.min.x), std::fabs(B.max.x - q.x));
            const float dy = std::min(std::fabs(q.y - B.min.y), std::fabs(B.max.y - q.y));
            const float dz = std::min(std::fabs(q.z - B.min.z), std::fabs(B.max.z - q.z));

            if (dx <= dy && dx <= dz)
            {
                out->normal = { (q.x > centerBox.x) ? 1.f : -1.f, 0, 0 };
            }
            else if (dy <= dz)
            {
                out->normal = { 0, (q.y > centerBox.y) ? 1.f : -1.f, 0 };
            }
            else
            {
                out->normal = { 0, 0, (q.z > centerBox.z) ? 1.f : -1.f };
            }
            out->penetration = r; // 簡易：フレーム反復で解消される前提
        }
        else
        {
            const Vector3 n = v * (1.0f / d); // A←B
            out->normal = n;
            out->penetration = r - d;
        }
    }
    return true;
}
