#pragma once
#include "Component.h"

class CameraControl : public MonoBehaviour
{
private:
	Transform* playerTf_ = nullptr;
	Transform* selfTf_   = nullptr;
	PlayerAction* playerAc_ = nullptr;
	float angleDeg = 0.0f;			   // Œ»ÝŠp“x[deg]
	const float kYawSpeedDeg = 120.0f; // Šp‘¬“x[deg / s]
public:
	void Start() override;
	void OnDestroy() override;
	void Update() override;
private:
	void FollowPlayer();
	void ApplyInput_();
	void WrapAngle_() noexcept
	{
		while (angleDeg >= 180.f) angleDeg -= 360.f;
		while (angleDeg < -180.f) angleDeg += 360.f;
	}
};