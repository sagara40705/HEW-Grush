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
constexpr float kGaugeUpValue[ToInt(GlassGrade::Count)] = { 2.f, 5.f, 10.f };
// グレードごとのKg(スコア変動値)
constexpr int kWeightKg[ToInt(GlassGrade::Count)] = { 1, 3, 5 };

class GlassScript : public Component
{
public:
	GlassScript(GlassGrade g = GlassGrade::Grade_1) : grade(g) {}
	void Start() override;
	void OnWillDestroy() override;
	void OnDestroy() override;
	GlassGrade grade = GlassGrade::Grade_1;
};