#pragma once
#include <cstdint>
#include "Component.h"

class GlassGaugeManager;

enum class GlassGrade : std::uint8_t
{
	Grade_1,
	Grade_2,
	Grade_3,
	Count
};
constexpr int ToIndex(GlassGrade g) noexcept {
	return static_cast<size_t>(g);
}

class GlassObject : public Component
{
private:
	GlassGaugeManager* ggm_ = nullptr;
public:
	GlassObject(GlassGrade g = GlassGrade::Grade_1) : grade(g) {}
	void Start() override;
	void OnDestroy() override;
	GlassGrade grade = GlassGrade::Grade_1;
public:
	void SpawnShards();
};