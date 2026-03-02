#pragma once
#include <cstdint>
#include "Component.h"
#include "Time/Timer.h"

class ResultGaugeUI : public MonoBehaviour
{
public:
	ResultGaugeUI() = default;
	~ResultGaugeUI() override = default;
	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;

private:
	UiSpriteRenderer* uiGauge_ = nullptr;
	UiSpriteRenderer* uiFrame_ = nullptr;
	UiSpriteRenderer* uiJob_ = nullptr;
	UiSpriteRenderer* uiJobFlash_ = nullptr;
	UiSpriteRenderer* uiframe_ = nullptr;
	UiSpriteRenderer* uiframeFlash_ = nullptr;
private:
	int currentGauge_ = 0;	// 現在のゲージ値
	int finalGauge_ = 0;	// 最終のゲージ値
	void CalcFinalGauge();
	int CalcUpGaugeValue();
private:
	// ゲージ上昇開始までの待機時間
	static constexpr float WAIT_TIME = 2.0f;
	Timer waitTimer_;
	float jobGlowTimer_ = 0.0f;
	const float JOB_GLOW_DURATION = 1.f;
private:
	bool isGaugeUping_ = false; 	// ゲージ上昇中かどうか
	static constexpr int MAX_GAUGE = 100;
	static constexpr int GAUGE_UP_SPEED = 1; // ゲージ上昇速度
	void UpdateGauge();
	void ChangeJobUI();
	void ChangeFrameUI();

	void SetGaugeBarPercent(float percent);
	
};


