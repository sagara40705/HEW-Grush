// Collider.cpp
#include "Collider.h"
#include "Transform.h"
#include "GameObject.h"

const Transform* Collider::GetTransform() const 
{
    return Parent ? Parent->TryGetComponent<Transform>() : nullptr;
}

Transform* Collider::GetTransform() 
{
    return Parent ? Parent->TryGetComponent<Transform>() : nullptr;
}

