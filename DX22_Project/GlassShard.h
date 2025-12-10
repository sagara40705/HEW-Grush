#pragma once
#include "Component.h"
#include "GlassObject.h"

class GlassShard : public MonoBehaviour
{
private:
	float lifeTimer = 0.f;
	GlassGrade grade_ = GlassGrade::Grade_1;
	Vector3 dir_;
	DirectX::XMFLOAT4 randParam_;
private:
	//Transform* tf_ = nullptr;
	Rigidbody* rb_ = nullptr;
public:
	void OnEnable() override;
	void OnDisable() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;
public:
	void OnActive(const Vector3& pos);
	void SetGlassGrade(GlassGrade g) { grade_ = g; }
	void SetShardParamVS();
	void SetGlassParamPS();
};