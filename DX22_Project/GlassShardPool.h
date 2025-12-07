#pragma once
#include "Component.h"

constexpr int MAX = 256;

class GlassShardPool : public Component
{
public:
	GameObject* pool_[MAX];
public:
	void Start() override;
	void OnDestroy() override;
};