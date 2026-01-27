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



void ResultGaugeUI::Start()
{
	
	constexpr Vector2 kMinutePos[ToIndex(TimerDigits::Count)] = { {520.f, 1000.f}, {0.f, 0.f} };
	//クリアタイムからゲージポイントを算出
	targetGauge_ =GaugePotint();
	currentGauge_ = 0.f;         //ゲージの初期化
	gaugeSpeed_ = 5.0f;          //ゲージが増えるスピード
	constexpr Vector2 kColonPos = { 1165.f,160.f };
	constexpr Vector2 kSecondPos[ToIndex(TimerDigits::Count)] = { {0.f, 0.f}, {685.f, 555.f} };
	
	Vector2 kNumGaugeberSize = { 1280.f,125.f };
	constexpr Vector2 kNumFrameberSize = { 1280.f, 760.f };

	GameObject& Gauge = gameObject->CreateChild();
	uiGauge_ = &Gauge.AddComponent<UiSpriteRenderer>(TextureTag::Result_Gauge_Bar, kSecondPos[ToIndex(TimerDigits::Ten)], kNumGaugeberSize, Vector2::zero(), kNumberUVScale);

	GameObject& Frame = gameObject->CreateChild();
	uiFrame_ = &Frame.AddComponent<UiSpriteRenderer>(TextureTag::Result_Gauge_Frame, kMinutePos[ToIndex(TimerDigits::Ten)], kNumFrameberSize, Vector2::zero(), kNumberUVScale);
	//Draw();
}

void ResultGaugeUI::Draw()
{
}

void ResultGaugeUI::Update()
{
	// ゲージが目標値に達していないなら
	if (currentGauge_ < targetGauge_)
	{
		currentGauge_ += gaugeSpeed_;

		if (currentGauge_ > targetGauge_)
		{
			currentGauge_ = targetGauge_;
		}

		// サイズ更新（横に伸ばす）
		Vector2 size = uiGauge_->GetSize();
		size.x = currentGauge_;
		uiGauge_->SetSize(size);
	}
}

float ResultGaugeUI::GaugePotint()
{
	//GameSteteからTimerのクリアタイムを取得
	float Gaugepoint = GetRecentClearTimes();
	//クリアタイムの点数換算
	if (Gaugepoint >= 60)
	{
		if (Gaugepoint >= 120)
		{
			if (Gaugepoint >= 180)
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
	
	return point;

}




	


