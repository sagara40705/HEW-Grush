#pragma once
#include "Component.h"
#include <cstdint>

enum class TimerDigits : std::uint8_t
{
	One,
	Ten,
	Count
};
constexpr size_t ToIndex(TimerDigits d) noexcept { return static_cast<size_t>(d); }

class TimerUI : public MonoBehaviour
{
private:
	float timer_ = 0.f;
	static constexpr int MAX_SECONDS = 99 * 60 + 59;
private:
	UiSpriteRenderer* uiMinute_[ToIndex(TimerDigits::Count)]{};
	UiSpriteRenderer* uiColon_ = nullptr;
	UiSpriteRenderer* uiSecond_[ToIndex(TimerDigits::Count)]{};
public:
	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;
public:
	float GetTime() const { return timer_; }
};