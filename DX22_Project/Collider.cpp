// Collider.cpp
#include "Collider.h"
#include "Transform.h"
#include "GameObject.h"

const Transform* Collider::GetTransform() const
{
    return gameObject ? gameObject->GetComponent<Transform>() : nullptr;
}

Transform* Collider::GetTransform()
{
    return gameObject ? gameObject->GetComponent<Transform>() : nullptr;
}
