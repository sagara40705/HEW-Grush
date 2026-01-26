#include "ResultGaugeUI.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "AllComponent.h"
#include "TextureManager.h"
#include "Defines.h"
#include "UiSpriteRenderer.h"
#include "TextureManager.h"
#include "GameState.h"
#include "ResultRankUI.h"


void ResultGaugeUI::Awake()
{
	GameObject& Gauge = gameObject->CreateChild();
	uiGauge_ = &Gauge.AddComponent<UiSpriteRenderer>
		(TextureTag::Result_Gauge_Bar, Vector2::zero(), Vector2(1280.f, 720.f));

	GameObject& Frame = gameObject->CreateChild();
	uiFrame_ = &Frame.AddComponent<UiSpriteRenderer>
		(TextureTag::Result_Gauge_Frame, Vector2::zero(), Vector2(1280.f, 720.f));
}

void ResultGaugeUI::Start()
{
	// 現在のジョブゲージ値を取得
	currentGauge_ = GetJobGauge();
	// 最終のゲージ値を計算
	CalcFinalGauge();
}

void ResultGaugeUI::Update()
{
}

void ResultGaugeUI::OnDestroy()
{
	uiFrame_ = nullptr;
	uiGauge_ = nullptr;
}

void ResultGaugeUI::CalcFinalGauge()
{
	int upValue = CalcUpGaugeValue();

	finalGauge_ = currentGauge_ + upValue;
}

int ResultGaugeUI::CalcUpGaugeValue()
{
	GameObject* rankUI = gameObject->FindWithTag(Tag::ResultRankUI);
	RankType rank = rankUI->GetComponent<ResultRankUI>()->GetRank();

	int value = 0;

	switch (rank)
	{
	case RankType::Rank_S:
		value = 100;
		break;
	case RankType::Rank_A:
		value = 70;
		break;
	case RankType::Rank_B:
		value = 50;
		break;
	case RankType::Rank_C:
		value = 30;
		break;
	default:
		assert(false && "Invalid RankType");
		break;
	}

	return value;
}


