#include "ResultTimerUI.h"
#include "GameObject.h"
#include "UiSpriteRenderer.h"
#include "TextureManager.h"

constexpr Vector2 kNumberUVScale = { 0.1f, 1.0f };

void ResultTimerUI::Start()
{
	constexpr Vector2 kMinutePos[ToIndex(TimerDigits::Count)] = { {500.f, 400.f}, {590.f, 400.f} };
	constexpr Vector2 kColonPos = { 700.f,430.f };
	constexpr Vector2 kSecondPos[ToIndex(TimerDigits::Count)] = { {750.f, 400.f}, {840.f, 400.f} };

	constexpr Vector2 kNumberSize = { 100.f,100.f };
	constexpr Vector2 kColonSize = { 40.f, 40.f };


	GameObject& tenMinute = gameObject->CreateChild();
	uiMinute_[ToIndex(TimerDigits::Ten)] = &tenMinute.AddComponent<UiSpriteRenderer>(TextureTag::TimerNumber, kMinutePos[ToIndex(TimerDigits::Ten)], kNumberSize, Vector2::zero(), kNumberUVScale);

	GameObject& oneMinute = gameObject->CreateChild();
	uiMinute_[ToIndex(TimerDigits::One)] = &oneMinute.AddComponent<UiSpriteRenderer>(TextureTag::TimerNumber, kMinutePos[ToIndex(TimerDigits::One)], kNumberSize, Vector2::zero(), kNumberUVScale);

	GameObject& colon = gameObject->CreateChild();
	uiColon_ = &colon.AddComponent<UiSpriteRenderer>(TextureTag::TimerColon, kColonPos, kColonSize);

	GameObject& tenSecond = gameObject->CreateChild();
	uiSecond_[ToIndex(TimerDigits::Ten)] = &tenSecond.AddComponent<UiSpriteRenderer>(TextureTag::TimerNumber, kSecondPos[ToIndex(TimerDigits::Ten)], kNumberSize, Vector2::zero(), kNumberUVScale);

	GameObject& oneSecond = gameObject->CreateChild();
	uiSecond_[ToIndex(TimerDigits::One)] = &oneSecond.AddComponent<UiSpriteRenderer>(TextureTag::TimerNumber, kSecondPos[ToIndex(TimerDigits::One)], kNumberSize, Vector2::zero(), kNumberUVScale);
}

void ResultTimerUI::Update()
{

	int totalSec = static_cast<int>(timer_);
	if (totalSec > MAX_SECONDS) totalSec = MAX_SECONDS;

	// ---- 2) mm:ss ‚É•ª‰ð ----
	int minutes = totalSec / 60;
	int seconds = totalSec % 60;

	uiMinute_[ToIndex(Timer::Ten)]->SetUVPosX((minutes % 10) * 0.1f);
	uiMinute_[ToIndex(Timer::One)]->SetUVPosX((minutes / 10) % 10 * 0.1f);
	uiSecond_[ToIndex(Timer::Ten)]->SetUVPosX((seconds % 10) * 0.1f);
	uiSecond_[ToIndex(Timer::One)]->SetUVPosX((seconds / 10) % 10 * 0.1f);
}

void ResultTimerUI::OnDestroy()
{
	uiMinute_[ToIndex(Timer::Ten)] = nullptr;
	uiMinute_[ToIndex(Timer::One)] = nullptr;
	uiColon_ = nullptr;
	uiSecond_[ToIndex(Timer::Ten)] = nullptr;
	uiSecond_[ToIndex(Timer::One)] = nullptr;
}
