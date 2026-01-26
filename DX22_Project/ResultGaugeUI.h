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
private:
	int currentGauge_ = 0;	// 現在のゲージ値
	int finalGauge_ = 0;	// 最終のゲージ値
	void CalcFinalGauge();
	int CalcUpGaugeValue();
private:
	// ゲージ上昇開始までの待機時間
	static constexpr float WAIT_TIME = 1.0f;
	Timer waitTimer_;
private:
	// ゲージ上昇中かどうか
	bool isGaugeUping_ = false;

};


