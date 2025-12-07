#pragma once
#include "Component.h"
#include "Vector3.h"

constexpr int MAX = 256;

class GlassShardPool : public Component
{
public:
	GameObject* pool_[MAX];
public:
	void Start() override;
	void OnDestroy() override;
public:
	GameObject* Instantiate(const Vector3& pos);
};