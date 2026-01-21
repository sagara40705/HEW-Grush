#include "ResultRankUI.h"
#include "GameObject.h"
#include "UiSpriteRenderer.h"
#include "TextureManager.h"

constexpr Vector2 kNumberUVScale = { 1.f, 1.0f };

void ResultRankUI::Awake()
{
	constexpr Vector2 kMinutePos[ToIndex(TimerDigits::Count)] = { {000.f, 00.f}, {00.f, 00.f} };
	constexpr Vector2 kColonPos = { 700.f,430.f };
	constexpr Vector2 kSecondPos[ToIndex(TimerDigits::Count)] = { {0.f, 0.f}, {0.f, 0.f} };

	constexpr Vector2 kNumberSize = { 1280.f,780.f };
	constexpr Vector2 kColonSize = { 40.f, 40.f };


	GameObject& tenMinute = gameObject->CreateChild();
	uiMinute_[ToIndex(TimerDigits::Ten)] = &tenMinute.AddComponent<UiSpriteRenderer>
		(TextureTag::RANKS, kMinutePos[ToIndex(TimerDigits::Ten)], kNumberSize, Vector2::zero(), kNumberUVScale);
}
void ResultRankUI::Start()
{

}

void ResultRankUI::Update(float)
{
}

void ResultRankUI::Draw()
{

}
