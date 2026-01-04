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


class ResultTimerUI : public MonoBehaviour
{
private:
	static constexpr int MAX_SECONDS = 99 * 60 + 59;
	
private:
	UiSpriteRenderer* uiMinute_[ToIndex(Timer::Count)]{};
	UiSpriteRenderer* uiColon_ = nullptr;
	UiSpriteRenderer* uiSecond_[ToIndex(Timer::Count)]{};
public:
	float timer_ = 0.f;		//Å©í«â¡
	void Start() override;
	void Update() override;
	void OnDestroy() override;
};

