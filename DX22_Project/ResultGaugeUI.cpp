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


//int ArubaitpLevel = 0;
void ResultGaugeUI::Start()
{
	//Update(GaugePoint);
	constexpr Vector2 kMinutePos[ToIndex(TimerDigits::Count)] = { {520.f, 1000.f}, {0.f, 0.f} };
	//=======
	pointGauge =GaugePotint();

	constexpr Vector2 kColonPos = { 1165.f,160.f };
	constexpr Vector2 kSecondPos[ToIndex(TimerDigits::Count)] = { {700.f, 240.f}, {680.f, 540.f} };

	Vector2 kNumGaugeberSize = { pointGauge, 195.f };
	constexpr Vector2 kNumFrameberSize = { 1280, 780.f };
//	constexpr Vector2 kColonSize = { 10.f, 200.f };
	//テクスチャーロード
	/*TextureManager::Load(TextureTag::Title_BG);
	TextureManager::Load(TextureTag::GaugeEffect);*/

	GameObject& Gauge = gameObject->CreateChild();
	uiGauge_ = &Gauge.AddComponent<UiSpriteRenderer>(TextureTag::Gauge, kSecondPos[ToIndex(TimerDigits::Ten)], kNumGaugeberSize, Vector2::zero(), kNumberUVScale);

	GameObject& Frame = gameObject->CreateChild();
	uiFrame_ = &Frame.AddComponent<UiSpriteRenderer>(TextureTag::Frame, kMinutePos[ToIndex(TimerDigits::Ten)], kNumFrameberSize, Vector2::zero(), kNumberUVScale);

	/*if (GaugePoint >= 300)
	{
		GaugePoint = 0.f;
		ArubaitpLevel++;
	}
	*/


}

void ResultGaugeUI::Draw()
{




}

float ResultGaugeUI::GaugePotint()
{
	//GameSteteからTimerのクリアタイムを取得
	float Gaugepoint = GetRecentClearTimes();
	//クリアタイムの点数換算
	if (Gaugepoint >= 100)
	{
		if (Gaugepoint >= 200)
		{
			if (Gaugepoint >= 300)
			{
				point = 250.f;
			}
			else
			{
				point = 300.f;
			}
		}
		else
		{
			point = 450.f;

		}
	}
	else
	{
		point = 600.f;
	}
	//アルバイトレベルの算出
	Allpoint = point;
	return point;

}

float ResultGaugeUI::AllpointGauge()
{
	
	if (Allpoint >= 600)
	{
		if (Allpoint >= 1200)
		{

			if (Allpoint >= 1800)
			{
				arubaitpLevel = 3;
			}
			else
			{
				arubaitpLevel = 2;
			}
		}
		else
		{
			arubaitpLevel = 1;

		}
	}
	else
	{
		arubaitpLevel = 1;
	}
 	return arubaitpLevel;
	//return 0;
}

int ResultGaugeUI::GetArubaitoLevel()
{
	return arubaitpLevel;
}


	


