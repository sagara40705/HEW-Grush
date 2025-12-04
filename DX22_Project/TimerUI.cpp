#include "TimerUI.h"
#include "GameObject.h"
#include "UiSpriteRenderer.h"
#include "TextureManager.h"

constexpr Vector2 kNumberUVScale = { 0.f, 0.f };

void TimerUI::Start()
{
	constexpr Vector2 kMinutePos[ToIndex(TimerDigits::Count)] = { {0.f, 0.f}, {0.f, 0.f} };
	constexpr Vector2 kColonPos = { 0.f, 0.f };
	constexpr Vector2 kSecondPos[ToIndex(TimerDigits::Count)] = { {0.f, 0.f}, {0.f, 0.f} };

	constexpr Vector2 kNumberSize = { 0.f, 0.f };
	constexpr Vector2 kColonSize = { 0.f, 0.f };


	GameObject& tenMinute = gameObject->CreateChild();
	tenMinute.AddComponent<UiSpriteRenderer>(TextureTag::TimerNumber, kMinutePos[ToIndex(TimerDigits::Ten)], kNumberSize);

	GameObject& oneMinute = gameObject->CreateChild();
	tenMinute.AddComponent<UiSpriteRenderer>(TextureTag::TimerNumber, kMinutePos[ToIndex(TimerDigits::One)], kNumberSize);

	GameObject& colon	  = gameObject->CreateChild();
	colon.AddComponent<UiSpriteRenderer>(TextureTag::TimerColon, kColonPos, kColonSize);

	GameObject& tenSecond = gameObject->CreateChild();
	tenSecond.AddComponent<UiSpriteRenderer>(TextureTag::TimerNumber, kSecondPos[ToIndex(TimerDigits::Ten)], kNumberSize);

	GameObject& oneSecond = gameObject->CreateChild();
	oneSecond.AddComponent<UiSpriteRenderer>(TextureTag::TimerNumber, kSecondPos[ToIndex(TimerDigits::One)], kNumberSize);
}

void TimerUI::Update()
{

}

void TimerUI::OnDestroy()
{
	uiMinute_[ToIndex(TimerDigits::Ten)] = nullptr;
	uiMinute_[ToIndex(TimerDigits::One)] = nullptr;
	uiColon_ = nullptr;
	uiSecond_[ToIndex(TimerDigits::Ten)] = nullptr;
	uiSecond_[ToIndex(TimerDigits::One)] = nullptr;
}
