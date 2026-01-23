#include "ResultGaugeUI.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "AllComponent.h"
#include "TextureManager.h"
#include "Defines.h"
#include "UiSpriteRenderer.h"
#include "TextureManager.h"
#include "GameState.h"
constexpr Vector2 kNumberUVScale = { 1.f, 1.0f };
float point = 50.f;

void ResultGaugeUI::Awake()
{
	//Update(GaugePoint);
	constexpr Vector2 kMinutePos[ToIndex(TimerDigits::Count)] = { {1300.f, 240.f}, {800.f, 300.f} };
	//=======
	//UpdateGaugePoint(GaugePoint);

	constexpr Vector2 kColonPos = { 1165.f,160.f };
	constexpr Vector2 kSecondPos[ToIndex(TimerDigits::Count)] = { {1375.f, 240.f}, {1500.f, 140.f} };

	Vector2 kNumGaugeberSize = { point, 40.f };
	constexpr Vector2 kNumFrameberSize = { 300.f, 55.f };
	constexpr Vector2 kColonSize = { 10.f, 200.f };
	//テクスチャーロード
	/*TextureManager::Load(TextureTag::Title_BG);
	TextureManager::Load(TextureTag::GaugeEffect);*/

	GameObject& Gauge = gameObject->CreateChild();
	uiGauge_ = &Gauge.AddComponent<UiSpriteRenderer>(TextureTag::Result_Gauge_Bar, kMinutePos[ToIndex(TimerDigits::Ten)], kNumGaugeberSize, Vector2::zero(), kNumberUVScale);

	GameObject& Frame = gameObject->CreateChild();
	uiFrame_ = &Frame.AddComponent<UiSpriteRenderer>(TextureTag::Result_Gauge_Frame, kMinutePos[ToIndex(TimerDigits::Ten)], kNumFrameberSize, Vector2::zero(), kNumberUVScale);
}
//int ArubaitpLevel = 0;
void ResultGaugeUI::Start()
{
}

void ResultGaugeUI::Draw()
{
}

float ResultGaugeUI::GaugePotint()
{
	float Allpoint;
	
	float Gaugepoint = GetRecentClearTimes();

	if (Gaugepoint >= 100)
	{
		if (Gaugepoint >= 200)
		{
			if (Gaugepoint >= 300)
			{
				point = 90.f;
			}
			else
			{
				point = 150.f;
			}
		}
		else
		{
			point = 270.f;

		}
	}
	else
	{
		point = 300.f;
	}
	Allpoint = point;
	if (Allpoint >= 300)
	{
		if (Allpoint >= 600)
		{

			if (Allpoint >= 900)
			{

			}
		}
	}
	return point;

}

