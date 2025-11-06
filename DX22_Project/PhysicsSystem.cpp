// PhysicsSystem.cpp
#include "PhysicsSystem.h"
#include "World.h"
#include "GameObject.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "Collision.h"
#include "CollisionListener.h"
#include "DebugLog.h"

#include <vector>
#include <set>
#include <map>
#include <utility>
#include <typeindex>
#include <algorithm>

using std::vector;

//========================= 基本構造体・ユーティリティ =========================

struct PairKey {
    std::uint32_t a{}, b{};             // 常に a < b で正規化
    bool operator<(const PairKey& r) const noexcept {
        return (a < r.a) || (a == r.a && b < r.b);
    }
};

static std::set<PairKey> s_lastCollisionPairs;
static std::set<PairKey> s_lastTriggerPairs;

struct Body {
    GameObject* go = nullptr;
    Transform* tf = nullptr;
    Rigidbody* rb = nullptr;   // 無ければ静的扱い
    Collider* col = nullptr;

    bool IsDynamic() const noexcept { return (rb != nullptr || !col->isStatic); }
};

static inline PairKey MakeKey(std::uint32_t ia, std::uint32_t ib) noexcept {
    return (ia < ib) ? PairKey{ ia, ib } : PairKey{ ib, ia };
}

static inline DirectX::XMFLOAT3 Neg(const DirectX::XMFLOAT3& v) { return { -v.x,-v.y,-v.z }; }
static inline float Dot(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static inline DirectX::XMFLOAT3 Scale(const DirectX::XMFLOAT3& v, float s) { return { v.x * s, v.y * s, v.z * s }; }

//========================= イベントディスパッチ =========================

static void DispatchCollisionEnter(GameObject& self, GameObject& other, const Contact& c) {
    for (auto& kv : self.components) if (auto* lis = dynamic_cast<CollisionListener*>(kv.second.get())) {
        CollisionInfo info{ &other, c.normal, c.penetration };
        lis->OnCollisionEnter(info);
    }
}
static void DispatchCollisionStay(GameObject& self, GameObject& other, const Contact& c) {
    for (auto& kv : self.components) if (auto* lis = dynamic_cast<CollisionListener*>(kv.second.get())) {
        CollisionInfo info{ &other, c.normal, c.penetration };
        lis->OnCollisionStay(info);
    }
}
static void DispatchCollisionExit(GameObject& self, GameObject& other) {
    for (auto& kv : self.components) if (auto* lis = dynamic_cast<CollisionListener*>(kv.second.get())) {
        lis->OnCollisionExit(other);
    }
}

static void DispatchTriggerEnter(GameObject& self, GameObject& other) {
    for (auto& kv : self.components) if (auto* lis = dynamic_cast<TriggerListener*>(kv.second.get())) {
        TriggerInfo info{ &other };
        lis->OnTriggerEnter(info);
    }
}
static void DispatchTriggerStay(GameObject& self, GameObject& other) {
    for (auto& kv : self.components) if (auto* lis = dynamic_cast<TriggerListener*>(kv.second.get())) {
        TriggerInfo info{ &other };
        lis->OnTriggerStay(info);
    }
}
static void DispatchTriggerExit(GameObject& self, GameObject& other) {
    for (auto& kv : self.components) if (auto* lis = dynamic_cast<TriggerListener*>(kv.second.get())) {
        lis->OnTriggerExit(other);
    }
}

//========================= 速度・位置解決 =========================

static void RemoveVelocityAlongNormal(Rigidbody* rb, const DirectX::XMFLOAT3& n) {
    if (!rb) return;
    const float vn = Dot(rb->velocity, n);
    rb->velocity.x -= vn * n.x;
    rb->velocity.y -= vn * n.y;
    rb->velocity.z -= vn * n.z;
}

static void SolveDynamicStatic(Body& dyn, const Contact& cA) {
    dyn.tf->AddPosition(Scale(cA.normal, cA.penetration));
    RemoveVelocityAlongNormal(dyn.rb, cA.normal);
}

static void SolveDynamicDynamic(Body& A, Body& B, const Contact& cA) {
    const float half = 0.5f * cA.penetration;
    A.tf->AddPosition(Scale(cA.normal, +half));
    B.tf->AddPosition(Scale(cA.normal, -half));
    RemoveVelocityAlongNormal(A.rb, cA.normal);
    RemoveVelocityAlongNormal(B.rb, Neg(cA.normal));
}

//========================= 関数分割：収集・判定・通知 =========================

// 1) World から Body 群を収集（各 Collider ごとに1 Body）
static void CollectBodies(const World& W, vector<Body>& dynamics, vector<Body>& statics) {
    dynamics.clear(); statics.clear();

    for (auto& up : W.objects) {
        if (!up) continue;
        GameObject& go = *up;
        if (!go.active) continue;

        // 全 Collider を列挙
        vector<Collider*> cols;
        cols.reserve(go.components.size());
        for (auto& kv : go.components) {
            if (auto* c = dynamic_cast<Collider*>(kv.second.get())) cols.push_back(c);
        }
        if (cols.empty()) continue;

        auto* tf = go.TryGetComponent<Transform>();
        if (!tf) continue;

        auto* rb = go.TryGetComponent<Rigidbody>(); // nullなら静的

        for (auto* c : cols) {
            if (!c->IsEnabled()) continue;
            Body b;
            b.go = &go; b.tf = tf; b.rb = rb; b.col = c;
            (b.IsDynamic() ? dynamics : statics).emplace_back(b);
        }
    }
}

// 2) 1フレーム分の接触を集計し、必要なら解決も行う
struct PairData { Contact c; bool isTrigger; };

static void CollideAndSolve(const vector<Body>& dynamics, const vector<Body>& statics,
    std::map<PairKey, PairData>& outPairs)
{
    auto addPair = [&](const Body& A, const Body& B, const Contact& c, bool isTrigger) {
        outPairs.emplace(MakeKey(A.go->id, B.go->id), PairData{ c, isTrigger });
        };
    auto canCollide = [](const Body& A, const Body& B) {
        return A.col->IsCollidableWith(*B.col);
        };

    // 動×静
    for (auto& d : dynamics) {
        for (auto& s : statics) {
            if (!canCollide(d, s)) continue;
            Contact c{};
            if (!Collision::Test(*d.col, *s.col, &c)) continue;

            const bool trigger = (d.col->isTrigger || s.col->isTrigger);
            addPair(d, s, c, trigger);

            if (!trigger) {
                // d は動的、sは静的
                SolveDynamicStatic(const_cast<Body&>(d), c);
            }
        }
    }

    // 動×動
    for (size_t i = 0; i < dynamics.size(); ++i) {
        for (size_t j = i + 1; j < dynamics.size(); ++j) {
            const Body& A = dynamics[i];
            const Body& B = dynamics[j];
            if (!canCollide(A, B)) continue;

            Contact c{};
            if (!Collision::Test(*A.col, *B.col, &c)) continue;

            const bool trigger = (A.col->isTrigger || B.col->isTrigger);
            addPair(A, B, c, trigger);

            if (!trigger) {
                // 解決は半分ずつ
                SolveDynamicDynamic(const_cast<Body&>(dynamics[i]), const_cast<Body&>(dynamics[j]), c);
            }
        }
    }
}

// 3) Enter/Stay/Exit 通知（衝突とトリガーで分離管理）
static void DispatchEvents(const World& W, const std::map<PairKey, PairData>& pairs)
{
    std::set<PairKey> curCollisionPairs, curTriggerPairs;

    auto findGO = [&](std::uint32_t id)->GameObject* {
        for (auto& up : W.objects) if (up && up->id == id) return up.get();
        return nullptr;
        };

    for (auto& [k, pd] : pairs) {
        GameObject* A = findGO(k.a);
        GameObject* B = findGO(k.b);
        if (!A || !B) continue;

        if (pd.isTrigger) {
            curTriggerPairs.insert(k);
            const bool was = (s_lastTriggerPairs.count(k) != 0);
            if (was) {
                DispatchTriggerStay(*A, *B);
                DispatchTriggerStay(*B, *A);
            }
            else {
                DispatchTriggerEnter(*A, *B);
                DispatchTriggerEnter(*B, *A);
            }
        }
        else {
            curCollisionPairs.insert(k);
            const bool was = (s_lastCollisionPairs.count(k) != 0);

            const Contact& cAB = pd.c;
            Contact cBA = pd.c; cBA.normal = Neg(pd.c.normal);

            if (was) {
                DispatchCollisionStay(*A, *B, cAB);
                DispatchCollisionStay(*B, *A, cBA);
            }
            else {
                DispatchCollisionEnter(*A, *B, cAB);
                DispatchCollisionEnter(*B, *A, cBA);
            }
        }
    }

    // Exit
    for (auto& k : s_lastTriggerPairs) {
        if (curTriggerPairs.count(k)) continue;
        if (auto* A = findGO(k.a)) if (auto* B = findGO(k.b)) {
            DispatchTriggerExit(*A, *B);
            DispatchTriggerExit(*B, *A);
        }
    }
    for (auto& k : s_lastCollisionPairs) {
        if (curCollisionPairs.count(k)) continue;
        if (auto* A = findGO(k.a)) if (auto* B = findGO(k.b)) {
            DispatchCollisionExit(*A, *B);
            DispatchCollisionExit(*B, *A);
        }
    }

    s_lastTriggerPairs = std::move(curTriggerPairs);
    s_lastCollisionPairs = std::move(curCollisionPairs);
}

//========================= 公開API =========================

void PhysicsSystem::Step(const World& W, float /*dt*/)
{
    vector<Body> dynamics, statics;
    CollectBodies(W, dynamics, statics);

    std::map<PairKey, PairData> pairs;
    CollideAndSolve(dynamics, statics, pairs);

    DispatchEvents(W, pairs);
}



/*
#include "PhysicsSystem.h"
#include "World.h"
#include "GameObject.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "Collision.h"
#include "CollisionListener.h"

#include <vector>
#include <set>
#include <map>
#include <utility>
#include <typeindex>

using std::vector;

namespace 
{
    struct Body 
    {
        GameObject* go;
        Transform* tf;
        Rigidbody* rb;  // null なら静的
        Collider* col;
        bool isDynamic() const { return rb != nullptr && !col->isStatic; }
    };

    struct PairKey
    {
        ObjectId a, b; // a<b に正規化
        bool operator<(const PairKey& o) const noexcept {
            return (a < o.a) || (a == o.a && b < o.b);
        }
    };

    // 前フレーム衝突ペア
    static std::set<PairKey> s_lastPairs;
    static std::set<PairKey> s_lastCollisionPairs;
    static std::set<PairKey> s_lastTriggerPairs;
    struct PairData 
    {
        Contact contact;   // 接触情報（法線/貫通量）
        bool    isTrigger; // ← どちらかがトリガー
    };
    std::map<PairKey, PairData> pairMap;
}

// 衝突コールバック（片側ずつ）
static void DispatchEnter(GameObject& self, GameObject& other, const Contact& c) 
{
    CollisionInfo info{ &other, c.normal, c.penetration };
    for (auto& kv : self.components) 
    {
        if (auto* lis = dynamic_cast<CollisionListener*>(kv.second.get())) 
        {
            lis->OnCollisionEnter(info);
        }
    }
}
static void DispatchStay(GameObject& self, GameObject& other, const Contact& c) 
{
    CollisionInfo info{ &other, c.normal, c.penetration };
    for (auto& kv : self.components) 
    {
        if (auto* lis = dynamic_cast<CollisionListener*>(kv.second.get()))
        {
            lis->OnCollisionStay(info);
        }
    }
}
static void DispatchExit(GameObject& self, GameObject& other)
{
    for (auto& kv : self.components) 
    {
        if (auto* lis = dynamic_cast<CollisionListener*>(kv.second.get()))
        {
            lis->OnCollisionExit(other);
        }
    }
}

void PhysicsSystem::Step(const World& W, float dt)
{
    vector<Body> dynamics;
    vector<Body> statics;

    // ワールドの全コライダー収集＆動静仕分け
    for (auto& up : W.objects) 
    {
        if (!up) continue;
        auto& o = *up;
        if (!o.active) continue;

        vector<Collider*> cols;
        cols.reserve(o.components.size());
        for (auto& kv : o.components) 
        {
            if (auto* c = dynamic_cast<Collider*>(kv.second.get())) cols.push_back(c);
        }
        if (cols.empty()) continue;

        auto* tf = o.TryGetComponent<Transform>();
        if (!tf) continue;

        auto* rb = o.TryGetComponent<Rigidbody>();

        for (auto* c : cols) 
        {
            Body b{ &o, tf, rb, c };
            (b.isDynamic() ? dynamics : statics).push_back(b);
        }
    }

    // 今フレーム衝突ペアと接触情報（オブジェクト単位で一回だけ通知するため集約）
    std::set<PairKey> curPairs;
    std::map<PairKey, Contact> pairContact; // 最初に見つけた接触でOK（単純化）

    auto addPair = [&](GameObject& A, GameObject& B, const Contact& c) 
    {
        PairKey k{ std::min(A.id,B.id), std::max(A.id,B.id) };
        curPairs.insert(k);
        if (pairContact.find(k) == pairContact.end()) pairContact.emplace(k, c);
    };

    
     //：動的 × 静的
    for (auto& d : dynamics)
    {
        for (auto& s : statics)
        {
            //
            Contact contact{};
            if (!Collision::Test(*d.col, *s.col, &contact)) continue;

            // 位置補正
            d.tf->AddPosition(DirectX::XMFLOAT3{
                contact.normal.x * contact.penetration,
                contact.normal.y * contact.penetration,
                contact.normal.z * contact.penetration
                });

            // 速度の法線成分を除去
            auto& v = d.rb->velocity; // XMFLOAT3 前提
            const float dot =
                v.x * contact.normal.x +
                v.y * contact.normal.y +
                v.z * contact.normal.z;

            if (dot < 0.0f)
            {
                v.x -= dot * contact.normal.x;
                v.y -= dot * contact.normal.y;
                v.z -= dot * contact.normal.z;
            }

            addPair(*d.go, *s.go, contact);
        }

   
    }

    // 2) 動的×動的
    for (size_t i = 0; i < dynamics.size(); ++i)
    {
        for (size_t j = i + 1; j < dynamics.size(); ++j)
        {
            auto& A = dynamics[i];
            auto& B = dynamics[j];

            Contact c{};
            if (!Collision::Test(*A.col, *B.col, &c)) continue;

            // 双方に半分ずつ押し出し
            const float half = c.penetration * 0.5f;
            A.tf->AddPosition(DirectX::XMFLOAT3{ c.normal.x * half, c.normal.y * half, c.normal.z * half });
            B.tf->AddPosition(DirectX::XMFLOAT3{ -c.normal.x * half, -c.normal.y * half, -c.normal.z * half });

            if (A.rb) A.rb->velocity = DirectX::XMFLOAT3{ 0,0,0 };
            if (B.rb) B.rb->velocity = DirectX::XMFLOAT3{ 0,0,0 };

            addPair(*A.go, *B.go, c);
        }
    }

    // 3) コールバック（Enter/Stay/Exit）
    // Enter or Stay
    for (auto& k : curPairs) 
    {
        const bool was = (s_lastPairs.find(k) != s_lastPairs.end());
        const Contact& c = pairContact[k];

        // k は正規化(a<b)なので、実際の法線向きを両者に合わせて渡す
        GameObject* A = nullptr; GameObject* B = nullptr;
        for (auto& up : W.objects) 
        {
            if (!up) continue;
            auto& o = *up;
            if (o.id == k.a) A = &o;
            if (o.id == k.b) B = &o;
        }
        if (!A || !B) continue;

        // A側（A←B 法線 = c.normal）
        if (was) { DispatchStay(*A, *B, c); }
        else { DispatchEnter(*A, *B, c); }
        // B側（B←A 法線 = -c.normal）
        Contact cBA = c; cBA.normal.x *= -1; cBA.normal.y *= -1; cBA.normal.z *= -1;
        if (was) { DispatchStay(*B, *A, cBA); }
        else { DispatchEnter(*B, *A, cBA); }
    }
    // Exit
    for (auto& k : s_lastPairs) 
    {
        if (curPairs.find(k) != curPairs.end()) continue;
        GameObject* A = nullptr; GameObject* B = nullptr;
        for (auto& up : W.objects) 
        {
            if (!up) continue;
            auto& o = *up;
            if (o.id == k.a) A = &o;
            if (o.id == k.b) B = &o;
        }
        if (A && B) { DispatchExit(*A, *B); DispatchExit(*B, *A); }
    }

    s_lastPairs = std::move(curPairs);
}
*/