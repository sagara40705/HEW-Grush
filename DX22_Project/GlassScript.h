#pragma once
#include "Component.h"

enum class GlassGrade : int
{
	Grade_1 = 0,
	Grade_2 = 1,
	Grade_3 = 2,
	Count = 3
};
constexpr int ToInt(GlassGrade g) { return static_cast<int>(g); }

// グレードごとのゲージの上がり値
constexpr float kGaugeUpValue[ToInt(GlassGrade::Count)] = { 1.f, 3.f, 5.f };

class GlassScript : public Component
{
public:
	void Start() override;
	void OnDestroy() override;
	GlassGrade grade = GlassGrade::Grade_1;
};