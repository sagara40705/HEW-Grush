#pragma once
#include "Component.h"

class GlassShard : public MonoBehaviour
{
private:
	float lifeTimer = 0.f;
	bool isAlive = false;
public:
	void OnEnable() override;
	void OnDisable() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;
};