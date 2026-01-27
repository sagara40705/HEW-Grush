#pragma once
#include <cstdint>
#include "Component.h"
#include "TimerUI.h"

class ResultGaugeUI : public MonoBehaviour
{
public:
	ResultGaugeUI() = default;
	~ResultGaugeUI() override = default;
	void Start() override;
	void Draw()override;
	void Update() override;
	float GaugePotint();
	float pointGauge;
	float AllpointGauge();
	float point;
float Allpoint;

private:
	float targetGauge_;    //最終ゲージ量
	float currentGauge_;   //今の表示ゲージ
	float gaugeSpeed_;     //増えるスピード
	//float timer_ = 0.f;
	UiSpriteRenderer* uiGauge_ = nullptr;
	UiSpriteRenderer* uiFrame_ = nullptr;
};

