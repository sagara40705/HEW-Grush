#pragma once
#include "Vector3.h"

class Collider; class GameObject;

struct Ray
{
	Vector3 origin = Vector3::zero();
	Vector3 direction = Vector3::zero();
	static constexpr float kMaxDist = 1.0f;
};

struct RaycastHit
{
	Vector3 point = Vector3::zero();
	Vector3 normal = Vector3::zero();
	float distance = 0.f;
	Collider* collider = nullptr;
	GameObject* gameObject = nullptr;
};
