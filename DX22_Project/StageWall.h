#pragma once
#include "Component.h"

class GroundRenderer;

class StageWall : public MonoBehaviour
{
private:
	Camera* cam_ = nullptr;
	GroundRenderer* render_ = nullptr;

public:
	void Start() override;
	void Update() override;
	void OnDestroy() override;
};