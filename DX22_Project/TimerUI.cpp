#include "TimerUI.h"
#include "GameObject.h"
#include "UiSpriteRenderer.h"
#include "TextureManager.h"

constexpr Vector2 kNumberUVScale = { 0.2f, 0.5f };

void TimerUI::Start()
{
	constexpr Vector2 kMinutePos[ToIndex(TimerDigits::Count)] = { {1050.f, 0.f}, {1090.f, 0.f} };
	constexpr Vector2 kColonPos = { 1130.f, 0.f };
	constexpr Vector2 kSecondPos[ToIndex(TimerDigits::Count)] = { {1160.f, 0.f}, {1200.f, 0.f} };

	constexpr Vector2 kNumberSize = { 40.f, 60.f };
	constexpr Vector2 kColonSize = { 30.f, 60.f };


	GameObject& tenMinute = gameObject->CreateChild();
	tenMinute.AddComponent<UiSpriteRenderer>(TextureTag::TimerNumber, kMinutePos[ToIndex(TimerDigits::Ten)], kNumberSize, Vector2::zero(), kNumberUVScale);

	GameObject& oneMinute = gameObject->CreateChild();
	oneMinute.AddComponent<UiSpriteRenderer>(TextureTag::TimerNumber, kMinutePos[ToIndex(TimerDigits::One)], kNumberSize, Vector2::zero(), kNumberUVScale);

	GameObject& colon	  = gameObject->CreateChild();
	colon.AddComponent<UiSpriteRenderer>(TextureTag::TimerColon, kColonPos, kColonSize);

	GameObject& tenSecond = gameObject->CreateChild();
	tenSecond.AddComponent<UiSpriteRenderer>(TextureTag::TimerNumber, kSecondPos[ToIndex(TimerDigits::Ten)], kNumberSize, Vector2::zero(), kNumberUVScale);

	GameObject& oneSecond = gameObject->CreateChild();
	oneSecond.AddComponent<UiSpriteRenderer>(TextureTag::TimerNumber, kSecondPos[ToIndex(TimerDigits::One)], kNumberSize, Vector2::zero(), kNumberUVScale);
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
