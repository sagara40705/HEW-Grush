// PhysicsSystem.h
#pragma once
class World;

namespace PhysicsSystem 
{
    // 物理1ステップ
    void Step(const World& W, float dt);
}



//// PhysicsSystem.h
//#pragma once
//#include <set>
//class World;
//
//namespace PhysicsSystem
//{
//
//    // dt 1/60 
//    void Step(World& W, float dt);
//
//    inline constexpr int kSolveIterations = 1;
//}
