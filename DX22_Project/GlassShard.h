#pragma once
#include "Component.h"
#include "GlassObject.h"

class GlassShard : public MonoBehaviour
{
private:
	float lifeTimer = 0.f;
	bool isAlive = false;
	GlassGrade grade_ = GlassGrade::Grade_1;
public:
	void OnEnable() override;
	void OnDisable() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;
public:
	void SetGlassParamPS();
};