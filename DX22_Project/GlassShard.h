#pragma once
#include "Component.h"
#include "GlassObject.h"
#include "Time/Timer.h"

class GlassShard : public MonoBehaviour
{
private:
	Timer timer_;
	GlassGrade grade_ = GlassGrade::Grade_1;
	Vector3 dir_;
	DirectX::XMFLOAT4 randParam_;
private:
	Transform* tf_ = nullptr;
	Rigidbody* rb_ = nullptr;
public:
	void OnEnable() override;
	void OnDisable() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;
public:
	void OnActive(const Vector3& pos);
	void AddForce(const Vector3& dir, float impulsePower);
	void SetGlassGrade(GlassGrade g);
	void SetGlassScale(float s);
	void SetShardParamVS();
	void SetGlassParamPS();
};