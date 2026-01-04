#include "Gauge.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "AllComponent.h"
#include "TextureManager.h"
#include "Defines.h"
#include "UiSpriteRenderer.h"
#include "TextureManager.h"

constexpr Vector2 kNumberUVScale = { 1.f, 1.0f };

//int ArubaitpLevel = 0;
void GaugeUI::Start()
{
	Update(GaugePoint);
	constexpr Vector2 kMinutePos[ToIndex(TimerDigits::Count)] = { {1100.f, 140.f}, {600.f, 200.f} };
	constexpr Vector2 kColonPos = { 1165.f,160.f };
	constexpr Vector2 kSecondPos[ToIndex(TimerDigits::Count)] = { {1175.f, 140.f}, {1200.f, 140.f} };

     Vector2 kNumGaugeberSize = {GaugePoint, 40.f };
	constexpr Vector2 kNumFrameberSize = { 300.f, 55.f };
	constexpr Vector2 kColonSize = { 10.f, 200.f };
	//テクスチャーロード
	/*TextureManager::Load(TextureTag::Title_BG);
	TextureManager::Load(TextureTag::GaugeEffect);*/

	GameObject& Gauge = gameObject->CreateChild();
	uiMinute_[ToIndex(TimerDigits::Ten)] = &Gauge.AddComponent<UiSpriteRenderer>(TextureTag::Gauge, kMinutePos[ToIndex(TimerDigits::Ten)], kNumGaugeberSize, Vector2::zero(), kNumberUVScale);

	GameObject& Frame = gameObject->CreateChild();
	uiMinute_[ToIndex(TimerDigits::Ten)] = &Frame.AddComponent<UiSpriteRenderer>(TextureTag::Frame, kMinutePos[ToIndex(TimerDigits::Ten)], kNumFrameberSize, Vector2::zero(), kNumberUVScale);

	/*if (GaugePoint >= 300)
	{
		GaugePoint = 0.f;
		ArubaitpLevel++;
	}
	*/


}

void GaugeUI::Draw()
{
	
	
	

}

float GaugeUI::Update(float)
{
	if (timer_ >= 100)
	{
		if (timer_ >= 200)
		{
			if (timer_ >= 300)
			{
				GaugePoint = 150.f;
			}
			else
			{
				GaugePoint = 90.f;
			}
		}
		else
		{
			GaugePoint = 270.f;
		}

	}
	else
	{
		GaugePoint = 300.f;
	}
	return GaugePoint;
	//return 0;

}
