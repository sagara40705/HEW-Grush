#include "PhysicsSystem.h"
#include "World.h"
#include "Collider.h"
#include <vector>


// 内部ヘルパー：Ray vs AABB
bool RaycastAABB(const Ray& ray, const Collider::AABB& box, float maxDistance, float& outT, Vector3& outNormal)
{
    const float eps = 1e-6f;

    const float ox = ray.origin.x;
    const float oy = ray.origin.y;
    const float oz = ray.origin.z;

    const float dx = ray.direction.x;
    const float dy = ray.direction.y;
    const float dz = ray.direction.z;

    const float bmin[3] = { box.min.x, box.min.y, box.min.z };
    const float bmax[3] = { box.max.x, box.max.y, box.max.z };
    const float o[3] = { ox, oy, oz };
    const float d[3] = { dx, dy, dz };

    float tMin = 0.0f;
    float tMax = maxDistance;
    Vector3 hitNormal = Vector3::zero();

    for (int i = 0; i < 3; ++i)
    {
        if (std::fabs(d[i]) < eps)
        {
            // レイがこの軸に平行：外側なら当たらない
            if (o[i] < bmin[i] || o[i] > bmax[i])
                return false;
            continue;
        }

        float invD = 1.0f / d[i];
        float t1 = (bmin[i] - o[i]) * invD;
        float t2 = (bmax[i] - o[i]) * invD;
        Vector3 axisNormal = Vector3::zero();

        // t1 <= t2 にする・その軸の法線も覚えておく
        if (t1 > t2)
        {
            std::swap(t1, t2);
            // レイは + → - 方向に入るので正規化軸は +1
            if (i == 0) axisNormal = Vector3(1, 0, 0);
            if (i == 1) axisNormal = Vector3(0, 1, 0);
            if (i == 2) axisNormal = Vector3(0, 0, 1);
        }
        else
        {
            // レイは - → + 方向に入るので正規化軸は -1
            if (i == 0) axisNormal = Vector3(-1, 0, 0);
            if (i == 1) axisNormal = Vector3(0, -1, 0);
            if (i == 2) axisNormal = Vector3(0, 0, -1);
        }

        // t1 で「入り」、t2 で「出る」
        if (t1 > tMin)
        {
            tMin = t1;
            hitNormal = axisNormal;
        }
        tMax = std::min(tMax, t2);

        if (tMin > tMax)
            return false;
    }

    if (tMin < 0.0f || tMin > maxDistance)
        return false;

    outT = tMin;
    outNormal = hitNormal;
    return true;
}


bool Physics::Raycast(const World& world, const Ray& ray, RaycastHit* outHit, float maxDistance, LayerMask layer, bool queryTrigger)
{
    if (maxDistance <= 0.0f)
        return false;

    // 出力の初期化
    if (outHit)
        *outHit = RaycastHit{};

    Collider* bestCol = nullptr;
    float     bestT = maxDistance;
    Vector3   bestN = Vector3::zero();

    for (Collider* col : world.GetColliders())
    {
        if (!col || !col->IsEnabled())
            continue;

        // レイヤフィルタ
        LayerMask objBit = ToBit(col->layer);
        if ((layer & objBit) == 0u)
            continue;

        // トリガー判定（Collider に isTrigger を追加して使う前提）
        if (!queryTrigger)
        {
            // 例：Collider に bool isTrigger を追加した場合
            // if (col->isTrigger) continue;
        }

        Collider::AABB aabb = col->GetWorldAABB();
        float t;
        Vector3 n;

        if (!RaycastAABB(ray, aabb, maxDistance, t, n))
            continue;

        if (t < bestT)
        {
            bestT = t;
            bestCol = col;
            bestN = n;
        }
    }

    if (!bestCol)
        return false;

    if (outHit)
    {
        outHit->distance = bestT;
        outHit->normal = bestN;
        outHit->point = ray.origin + ray.direction * bestT;
        outHit->collider = bestCol;
        outHit->gameObject = bestCol->gameObject; // Component::Parent が GameObject*
    }

    return true;
}
