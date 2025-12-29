#pragma once

#include <cstdint>
#include "Component.h"
#include "TimerUI.h"
enum class Timer : std::uint8_t
{
	One,
	Ten,
	Count
};
constexpr size_t ToIndex(Timer d) noexcept { return static_cast<size_t>(d); }

class ResultTimerUI : public TimerUI
{
private:
	static constexpr int MAX_SECONDS = 99 * 60 + 59;
	
private:
	UiSpriteRenderer* uiMinute_[ToIndex(Timer::Count)]{};
	UiSpriteRenderer* uiColon_ = nullptr;
	UiSpriteRenderer* uiSecond_[ToIndex(Timer::Count)]{};
public:
	void Start() override;
	void Update() override;
	void OnDestroy() override;
	GameObject* gameObject = nullptr;
};

