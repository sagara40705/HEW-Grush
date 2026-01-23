#include "ResultRankUI.h"
#include "GameObject.h"
#include "UiSpriteRenderer.h"
#include "TextureManager.h"
#include "GameState.h"
constexpr Vector2 kNumberUVScale = { 1.f, 1.0f };
void ResultRankUI::Start()
{
	constexpr Vector2 kMinutePos[ToIndex(TimerDigits::Count)] = { {000.f, 00.f}, {00.f, 00.f} };
	constexpr Vector2 kColonPos = { 700.f,430.f };
	constexpr Vector2 kSecondPos[ToIndex(TimerDigits::Count)] = { {0.f, 0.f}, {0.f, 0.f} };

	constexpr Vector2 kNumberSize = { 1280.f,780.f };
	constexpr Vector2 kColonSize = { 40.f, 40.f };
	float clearTime = GetRecentClearTimes();
	GameObject& RankSMinute = gameObject->CreateChild();
	GameObject& RankAMinute = gameObject->CreateChild();
	GameObject& RankBMinute = gameObject->CreateChild();
	GameObject& RankCMinute = gameObject->CreateChild();
	if (clearTime<=180)
	{
		if (clearTime <= 120)
		{
			if (clearTime <= 60)
			{
				
				uiMinute_[ToIndex(TimerDigits::Ten)] = &RankSMinute.AddComponent<UiSpriteRenderer>(TextureTag::RANKS, kMinutePos[ToIndex(TimerDigits::Ten)], kNumberSize, Vector2::zero(), kNumberUVScale);
			}
			else
			{
				uiMinute_[ToIndex(TimerDigits::Ten)] = &RankAMinute.AddComponent<UiSpriteRenderer>(TextureTag::ResultA, kMinutePos[ToIndex(TimerDigits::Ten)], kNumberSize, Vector2::zero(), kNumberUVScale);

			}
		}
		else
		{
			uiMinute_[ToIndex(TimerDigits::Ten)] = &RankBMinute.AddComponent<UiSpriteRenderer>(TextureTag::ResultB, kMinutePos[ToIndex(TimerDigits::Ten)], kNumberSize, Vector2::zero(), kNumberUVScale);
		}
	}
	else
	{
		uiMinute_[ToIndex(TimerDigits::Ten)] = &RankCMinute.AddComponent<UiSpriteRenderer>(TextureTag::ResultC, kMinutePos[ToIndex(TimerDigits::Ten)], kNumberSize, Vector2::zero(), kNumberUVScale);
	}
}

void ResultRankUI::Update(float)
{
}

void ResultRankUI::Draw()
{

}
