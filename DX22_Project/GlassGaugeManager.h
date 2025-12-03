#pragma once
#include "Component.h"
#include <cstdint>

class PlayerState; class Hammer;
enum class GlassGrade : std::uint8_t;

class GlassGaugeManager : public MonoBehaviour
{
private:
	float gaude_ = 0.0f;
	bool isGaugeEmpty = true;
	bool isGaugeDecaying = false;   //ゲージ減衰中か？
	float gaugeDecayDelayTimer = 0.f; //ゲージ減衰までのタイマー
	const float kGaugeDecayDelayTime = 5.f; //ゲージ減衰までの時間
private:
	PlayerState* player_ = nullptr;
	Hammer* hammer_ = nullptr;
public:
	void Start() override;
	void Update() override;
	void OnDestroy() override;
public:
	void BrakeGlass(GlassGrade grade);
	void GradeUp();
	void GradeDown();
	void OnGlassBrake();
private:
	void DebugCommand();
};