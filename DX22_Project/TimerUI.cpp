#include "TimerUI.h"
#include "GameObject.h"
#include "UiSpriteRenderer.h"
#include "TextureManager.h"

constexpr Vector2 kNumberUVScale = { 0.1f, 1.0f };

void TimerUI::Start()
{
	constexpr Vector2 kMinutePos[ToIndex(TimerDigits::Count)] = { {1085.f, 17.f}, {1110.f, 17.f} };
	constexpr Vector2 kColonPos = { 1140.f,19.f };
	constexpr Vector2 kSecondPos[ToIndex(TimerDigits::Count)] = { {1155.f, 17.f}, {1180.f, 17.f} };

	constexpr Vector2 kNumberSize = { 25.f, 25.f };
	constexpr Vector2 kColonSize = { 10.f, 20.f };


	GameObject& tenMinute = gameObject->CreateChild();
	uiMinute_[ToIndex(TimerDigits::Ten)] = &tenMinute.AddComponent<UiSpriteRenderer>(TextureTag::TimerNumber, kMinutePos[ToIndex(TimerDigits::Ten)], kNumberSize, Vector2::zero(), kNumberUVScale);

	GameObject& oneMinute = gameObject->CreateChild();
	uiMinute_[ToIndex(TimerDigits::One)] = &oneMinute.AddComponent<UiSpriteRenderer>(TextureTag::TimerNumber, kMinutePos[ToIndex(TimerDigits::One)], kNumberSize, Vector2::zero(), kNumberUVScale);

	GameObject& colon	  = gameObject->CreateChild();
	uiColon_ = &colon.AddComponent<UiSpriteRenderer>(TextureTag::TimerColon, kColonPos, kColonSize);

	GameObject& tenSecond = gameObject->CreateChild();
	uiSecond_[ToIndex(TimerDigits::Ten)] = &tenSecond.AddComponent<UiSpriteRenderer>(TextureTag::TimerNumber, kSecondPos[ToIndex(TimerDigits::Ten)], kNumberSize, Vector2::zero(), kNumberUVScale);

	GameObject& oneSecond = gameObject->CreateChild();
	uiSecond_[ToIndex(TimerDigits::One)] = &oneSecond.AddComponent<UiSpriteRenderer>(TextureTag::TimerNumber, kSecondPos[ToIndex(TimerDigits::One)], kNumberSize, Vector2::zero(), kNumberUVScale);
}

void TimerUI::Update()
{
	timer_ += 1.f / 60.f;

	// 上限 99:59 でクランプ
	int totalSec = static_cast<int>(timer_);
	if (totalSec > MAX_SECONDS) totalSec = MAX_SECONDS;

	// ---- 2) mm:ss に分解 ----
	int minutes = totalSec / 60;
	int seconds = totalSec % 60;

	uiMinute_[ToIndex(TimerDigits::Ten)]->SetUVPosX((minutes % 10) * 0.1f);
	uiMinute_[ToIndex(TimerDigits::One)]->SetUVPosX((minutes / 10) % 10 * 0.1f);
	uiSecond_[ToIndex(TimerDigits::Ten)]->SetUVPosX((seconds % 10) * 0.1f);
	uiSecond_[ToIndex(TimerDigits::One)]->SetUVPosX((seconds / 10) % 10 * 0.1f);
}

void TimerUI::OnDestroy()
{
	uiMinute_[ToIndex(TimerDigits::Ten)] = nullptr;
	uiMinute_[ToIndex(TimerDigits::One)] = nullptr;
	uiColon_ = nullptr;
	uiSecond_[ToIndex(TimerDigits::Ten)] = nullptr;
	uiSecond_[ToIndex(TimerDigits::One)] = nullptr;
}
