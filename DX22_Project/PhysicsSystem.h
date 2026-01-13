// PhysicsSystem.h
#pragma once
#include "Raycast.h"
#include "Layer.h"
class World;

namespace PhysicsSystem
{
    // 物理1ステップ
    void Step(const World& W, float dt);
}
namespace Physics
{
    bool Raycast(const World& world, const Ray& ray, RaycastHit* outHit, float maxDistance = Ray::kMaxDist, LayerMask layer = MaskAll, bool queryTrigger = true);
}