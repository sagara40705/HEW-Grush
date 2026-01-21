#pragma once
#include "Component.h"
#include <cstdint>

class PlayerState; class Hammer;
enum class GlassGrade : std::uint8_t;
enum class HammerGrade : std::uint8_t;

class GlassGaugeManager : public MonoBehaviour
{
private:
	float gauge_ = 0.0f;
	bool isGaugeEmpty = true;
	bool isGaugeDecaying = false;   //ƒQ[ƒWŒ¸Š’†‚©H
	float gaugeDecayDelayTimer = 0.f; //ƒQ[ƒWŒ¸Š‚Ü‚Å‚Ìƒ^ƒCƒ}[
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
public:
	bool dirty_ = true;
	bool up_ = false;
	float GetGaugeRatio();
	HammerGrade GetHammerGrade();
private:
	//ŒÀŠE“Ë”j
	const float kLimitBreakTime = 10.f; // ŒÀŠE“Ë”j‚·‚é‚Ü‚Å‚ÌŠÔ
	float limitBreakTimer_ = 0.f;		// ŒÀŠE“Ë”j‚·‚é‚Ü‚Å‚Ìƒ^ƒCƒ}[

	const float kLimitBreakDuration = 5.f; // ŒÀŠE“Ë”j‚Ì‘±ŠÔ
	float limitBreakDurationTimer_ = 0.f;  // ŒÀŠE“Ë”j‚Ì‘±ŠÔƒ^ƒCƒ}[

	bool IsMaxGauge(); //ƒQ[ƒW‚ªÅ‘å’l‚©H
	bool isLimitBreak_ = false; //ŒÀŠE“Ë”j’†‚©H
private:
#ifdef _DEBUG
	void DebugCommand();
#endif // DEBUG	
};