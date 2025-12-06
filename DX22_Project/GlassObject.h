#pragma once
#include <cstdint>
#include "Component.h"

class GlassGaugeManager; class ScoreManager;

enum class GlassGrade : std::uint8_t
{
	Grade_1,
	Grade_2,
	Grade_3,
	Count
};
constexpr size_t ToIndex(GlassGrade g) noexcept {
	return static_cast<size_t>(g);
}

class GlassObject : public Component
{
private:
	bool isShard = false;
	GlassGaugeManager* gaude_ = nullptr;
	ScoreManager* score_ = nullptr;
	GlassGrade grade_ = GlassGrade::Grade_1;
	float hp_ = 0;
	Transform* tf_ = nullptr;
public:
	GlassObject(GlassGrade g = GlassGrade::Grade_1) : grade_(g) {}
	void Start() override;
	void OnDestroy() override;
public:
	void SetIsShard(bool v) { isShard = v; }
	bool GetIsShard() { return isShard; }
	GlassGrade GetGrade() { return grade_; }
	void TakeDamage(float damage);
	void SpawnShards();
	void SetGlassParamPS();
};