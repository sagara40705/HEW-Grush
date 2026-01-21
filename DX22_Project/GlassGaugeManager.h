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
	HammerGrade GetHammerGrade();
private:
	//限界突破
	const float kLimitBreakTime = 10.f; // 限界突破するまでの時間
	float limitBreakTimer_ = 0.f;		// 限界突破するまでのタイマー

	const float kLimitBreakDuration = 5.f; // 限界突破の持続時間
	float limitBreakDurationTimer_ = 0.f;  // 限界突破の持続時間タイマー

	bool IsMaxGauge(); //ゲージが最大値か？
	bool isLimitBreak_ = false; //限界突破中か？
public:
	// チュートリアル用フラグ
	bool isTutorial_ = false; // チュートリアル中か？
	bool warkingTask_ = false; // チュートリアルのタスクを進行中か？
	bool endTask_02_ = false; // チュートリアルのタスク2をクリアしたか？
	bool endTask_04_ = false; // チュートリアルのタスク3をクリアしたか？

private:
#ifdef _DEBUG
	void DebugCommand();
#endif // DEBUG	
};