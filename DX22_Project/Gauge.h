#pragma once
#include <cstdint>
#include "Component.h"
#include "TimerUI.h"
enum class Gauge : std::uint8_t
{
	One,
	Ten,
	Count
};
constexpr size_t ToIndex(Gauge d) noexcept { return static_cast<size_t>(d); }

class GaugeUI : public TimerUI
{
public:
	GaugeUI() = default;
	~GaugeUI() override = default;
	float GaugePoint = 0.f;
    void Start() override;
	float Update(float);
	void Draw()override;
	float GetGaugePoint() { return GaugePoint; }
private:
	UiSpriteRenderer* uiMinute_[ToIndex(Gauge::Count)]{};
	UiSpriteRenderer* uiColon_ = nullptr;
	UiSpriteRenderer* uiSecond_[ToIndex(Gauge::Count)]{};


};

