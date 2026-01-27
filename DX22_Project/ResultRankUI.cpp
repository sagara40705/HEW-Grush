#include "ResultRankUI.h"
#include "GameObject.h"
#include "UiSpriteRenderer.h"
#include "TextureManager.h"
#include "GameState.h"
#include <DirectXMath.h>
using namespace DirectX;

XMFLOAT4 color(1.2f, 1.2f, 1.2f, 1.0f);
constexpr Vector2 kNumberUVScale = { 1.f, 1.0f };
void ResultRankUI::Start()
{
	constexpr Vector2 kMinutePos[ToIndex(TimerDigits::Count)] = { {000.f, 00.f}, {00.f, 00.f} };
//	constexpr Vector2 kColonPos = { 700.f,430.f };
	constexpr Vector2 kSecondPos[ToIndex(TimerDigits::Count)] = { {0.f, 0.f}, {0.f, 0.f} };
	Vector2 pos = kMinutePos[ToIndex(TimerDigits::Ten)];
	
	constexpr Vector2 kNumberSize = { 1280.f,780.f };
	Vector2 size = kNumberSize;
//	constexpr Vector2 kColonSize = { 40.f, 40.f };
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

void ResultRankUI::Update(float deltaTime)
{

	for (int a = 0; a <= 10; a++)
	{
		glowTime_ += deltaTime;

		// 0.7 ～ 1.3 くらいで脈打つ光
		float glow = 1.0f + sinf(glowTime_ * 4.0f) * 0.3f;



		// 今表示されているランクに色を適用
		if (uiMinute_[ToIndex(TimerDigits::Ten)])
		{
			uiMinute_[ToIndex(TimerDigits::Ten)]->SetColor(color);
		}
	}
	

}

void ResultRankUI::Draw()
{

}
