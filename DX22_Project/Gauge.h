#pragma once
#include <cstdint>
#include "Component.h"
#include "TimerUI.h"

class GaugeUI : public MonoBehaviour
{
public:
	GaugeUI() = default;
	~GaugeUI() override = default;
	float GaugePoint = 0.f;
    void Start() override;
	float UpdateGaugePoint(float);
	void Draw()override;
	float GetGaugePoint() { return GaugePoint; }
private:
	float timer_ = 0.f;
	UiSpriteRenderer* uiGauge_ = nullptr;
	UiSpriteRenderer* uiFrame_ = nullptr;
};

