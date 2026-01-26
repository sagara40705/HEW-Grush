#pragma once
#include "Component.h"
#include "Vector3.h"

constexpr int MAX = 256;

enum class GlassGrade : std::uint8_t;

class GlassShardPool : public Component
{
private:
	static GlassShardPool* s_instance;
	GameObject* pool_[MAX];
	Transform* camTf_ = nullptr;
	Transform* player_ = nullptr;
public:
	void Start() override;
	void OnDestroy() override;
public:
	static GlassShardPool* GetInstanse() { return s_instance; }
	GameObject* Instantiate(const Vector3& pos, GlassGrade grade, float scale);
private:
	Vector3 CalcBillboard(const Vector3& pos);
	Vector3 MakePlayerBiasedDir(const Vector3& shardPos);
	float MakeImpulsePower();
};