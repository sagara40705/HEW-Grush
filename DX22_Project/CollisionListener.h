#pragma once
#include "Component.h"
#include "Vector3.h"

class GameObject;

struct CollisionInfo 
{
    GameObject* other = nullptr;
    DirectX::XMFLOAT3 normal{ 0,0,0 }; // é©ï™Å©ëäéË ÇÃâüÇµñﬂÇµñ@ê¸
    float penetration = 0.0f;
};

// MonoBehaviour îhê∂
class CollisionListener
{
public:
    virtual void OnCollisionEnter(const CollisionInfo& c) {}
    virtual void OnCollisionStay(const CollisionInfo& c) {}
    virtual void OnCollisionExit(GameObject& other) {}
};

struct TriggerInfo
{
    GameObject* other = nullptr; 
};

class TriggerListener
{
public:
    virtual ~TriggerListener() = default;
    virtual void OnTriggerEnter(const TriggerInfo& info) {}
    virtual void OnTriggerStay(const TriggerInfo& info) {}
    virtual void OnTriggerExit(GameObject& other) {}
};