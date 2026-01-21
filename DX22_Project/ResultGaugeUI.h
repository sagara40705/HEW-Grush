#pragma once
#include <cstdint>
#include "Component.h"
#include "TimerUI.h"

class ResultGaugeUI : public MonoBehaviour
{
public:
	ResultGaugeUI() = default;
	~ResultGaugeUI() override = default;
	float GaugePoint = 0.f;
	void Awake() override;
	void Start() override;
	//float UpdateGaugePoint(float);
	void Draw()override;
	//float GetGaugePoint() { return GaugePoint; }
//	float timer_ = 0.f;
	float GaugePotint();
private:
	//float timer_ = 0.f;
	UiSpriteRenderer* uiGauge_ = nullptr;
	UiSpriteRenderer* uiFrame_ = nullptr;
};


