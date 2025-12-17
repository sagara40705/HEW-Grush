#pragma once
#include "Component.h"
#include <cstdint>

class PlayerState; class Hammer;
enum class GlassGrade : std::uint8_t;
enum class HammerGrade : std::uint8_t;

class GlassGaugeManager : public MonoBehaviour
{
private:
	float gauge_ = 0.0f;			  //現在のゲージ量
	bool isGaugeEmpty = true;	  //ゲージが空か？
	bool isGaugeDecaying = false;   //ゲージ減衰中か？
	float gaugeDecayDelayTimer = 0.f; //ゲージ減衰までのタイマー
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
	float GetGaugeDecayTimer() { return gaugeDecayDelayTimer; }
	bool  IsGaugeDecaying()  { return isGaugeDecaying; }
	HammerGrade GetHammerGrade();
private:
#ifdef _DEBUG
	void DebugCommand();
#endif // DEBUG	
};