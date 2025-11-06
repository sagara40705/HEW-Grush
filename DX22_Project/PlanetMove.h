// PlanetMover.h
#pragma once
#include "Component.h"
class Transform;

class PlanetMove : public MonoBehaviour {
public:
    float radius = 50.0f;          // 球半径
    float moveSpeed = 5.0f;        // プレイヤーの見かけ速度 m/s
    Transform* t = nullptr;        // 自分(=PlanetRoot)のTransform
    Transform* playerT = nullptr;  // プレイヤーTransform
    void Start() override;
    void Update() override;
};
