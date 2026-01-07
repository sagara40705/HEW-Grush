#include "Rank.h"
#include "GameObject.h"
#include "UiSpriteRenderer.h"
#include "TextureManager.h"

constexpr Vector2 kNumberUVScale = { 1.f, 1.0f };
void RankUI::Start()
{
	constexpr Vector2 kMinutePos[ToIndex(TimerDigits::Count)] = { {300.f, 400.f}, {300.f, 200.f} };
	constexpr Vector2 kColonPos = { 700.f,430.f };
	constexpr Vector2 kSecondPos[ToIndex(TimerDigits::Count)] = { {750.f, 400.f}, {840.f, 400.f} };

	constexpr Vector2 kNumberSize = { 200.f,200.f };
	constexpr Vector2 kColonSize = { 40.f, 40.f };


	GameObject& tenMinute = gameObject->CreateChild();
	uiMinute_[ToIndex(TimerDigits::Ten)] = &tenMinute.AddComponent<UiSpriteRenderer>(TextureTag::RANKS, kMinutePos[ToIndex(TimerDigits::Ten)], kNumberSize, Vector2::zero(), kNumberUVScale);
}

void RankUI::Update(float)
{
}

void RankUI::Draw()
{

}
