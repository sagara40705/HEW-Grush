#pragma once
#include "Component.h"
#include "Vector3.h"

class Rigidbody;

class GlassShard : public MonoBehaviour
{
public:
    float lifetime = 3.0f;     // ê∂ë∂ïbêî
    float elapsed = 0.0f;     // åoâﬂéûä‘

    Rigidbody* rb = nullptr;

public:
    void Start() override;
    void OnDestroy() override;
    void Update() override;

};

