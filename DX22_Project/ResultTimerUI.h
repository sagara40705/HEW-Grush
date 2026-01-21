#pragma once

#include <cstdint>
#include "Component.h"
#include "TimerUI.h"
//enum class TimerDigitsR : std::uint8_t
//{
//	One,
//	Ten,
//	Count
//};
//constexpr size_t ToIndex(TimerDigitsR d) noexcept { return static_cast<size_t>(d); }

//<<<<<<< HEAD
//=======
//
//>>>>>>> e348d9a321fd84f4a3ac8ea23cc62e58cc620fd9
class ResultTimerUI : public MonoBehaviour
{
private:
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
};
