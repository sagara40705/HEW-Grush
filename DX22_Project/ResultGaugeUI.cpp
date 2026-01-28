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
#include "DebugLog.h"

void ResultGaugeUI::Awake()
{
	GameObject& Gauge = gameObject->CreateChild();
	uiGauge_ = &Gauge.AddComponent<UiSpriteRenderer>
		(TextureTag::Result_Gauge_Bar, Vector2::zero(), Vector2(1280.f, 720.f));

	GameObject& Frame = gameObject->CreateChild();
	uiFrame_ = &Frame.AddComponent<UiSpriteRenderer>
		(TextureTag::Result_Gauge_Frame, Vector2::zero(), Vector2(1280.f, 720.f));

	GameObject& job = gameObject->CreateChild();
	uiJob_ = &job.AddComponent<UiSpriteRenderer>(TextureTag::Result_Job_Pikapika, Vector2(650.f, 450.f), Vector2(600.f, 70.f));

	GameObject& jobFlash = gameObject->CreateChild();
	uiJobFlash_ = &jobFlash.AddComponent<UiSpriteRenderer>(TextureTag::Result_Job_Pikapika, Vector2(650.f, 450.f), Vector2(600.f, 70.f));

	// 最初は透明にしておく
	uiJobFlash_->SetColor({ 2.0f, 2.0f, 2.0f, 0.0f });
	uiJobFlash_->SetVisible(false);
}

void ResultGaugeUI::Start()
{
	//SetJobGauge(50.f); // デバッグ用 

	// 現在のジョブゲージ値を取得
	currentGauge_ = GetJobGauge();
	// 最終のゲージ値を計算
	CalcFinalGauge();

	// ゲージバーを初期化
	SetGaugeBarPercent(static_cast<float>(currentGauge_) / static_cast<float>(MAX_GAUGE));

	// ジョブUIを変更
	ChangeJobUI();

	// 待機タイマー開始
	waitTimer_.Start();
}

void ResultGaugeUI::Update()
{
	if (isGaugeUping_)
	{
		UpdateGauge();
	}
	else
	{
		if (waitTimer_.ElapsedRawSeconds() >= WAIT_TIME)
		{
			waitTimer_.Stop();
			isGaugeUping_ = true;
			DebugLog::Output("ResultGaugeUI-StartGaugeUp");
		}
	}

	if (jobGlowTimer_ > 0.0f)
	{
		jobGlowTimer_ -= 1.0f / 60.0f;
		float t = 1.0f - (jobGlowTimer_ / JOB_GLOW_DURATION); // 0.0 -> 1.0

		// sin波で 0 -> 1 -> 0 の動きを作る
		float alpha = std::sin(t * 3.1415f);

		if (uiJobFlash_)
		{
			uiJobFlash_->SetVisible(true);
			// 明るい白（輝度2.0〜3.0）で、透明度だけをふわっと変える
			uiJobFlash_->SetColor({ 4.f, 4.f, 4.f, alpha });
		}
	}
	else
	{
		if (uiJobFlash_) uiJobFlash_->SetVisible(false);
	}

}

void ResultGaugeUI::OnDestroy()
{
	uiFrame_ = nullptr;
	uiGauge_ = nullptr;
	uiJob_ = nullptr;
	uiJobFlash_ = nullptr;
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

void ResultGaugeUI::UpdateGauge()
{
	currentGauge_ += GAUGE_UP_SPEED; // 1フレーム分の上昇値を加算

	// ゲージの最大値を超えたらjobを上げる
	if (currentGauge_ >= MAX_GAUGE)
	{
		// 既に達人なら ゲージは最大値で固定
		if (GetJobType() == JobType::Tastuzin)
		{	
			currentGauge_ = MAX_GAUGE;	// ゲージを最大値で固定
			finalGauge_ = MAX_GAUGE;	// ゲージを最大値で固定
		}
		else
		{
			// ジョブアップ
			JobType job = GetJobType();
			job = static_cast<JobType>(static_cast<std::uint8_t>(job) + 1);
			SetJobType(job);
			ChangeJobUI();

			jobGlowTimer_ = JOB_GLOW_DURATION;

			currentGauge_ = currentGauge_ - MAX_GAUGE;	// 残りのゲージを次のジョブに引き継ぐ
			finalGauge_ = finalGauge_ - MAX_GAUGE;		// 残りのゲージを次のジョブに引き継ぐ
		}
	}

	// ゲージバーの割合を設定
	SetGaugeBarPercent(static_cast<float>(currentGauge_) / static_cast<float>(MAX_GAUGE));

	// ゲージの最終値に到達したら更新終了
	if (currentGauge_ >= finalGauge_)
	{
		currentGauge_ = finalGauge_;
		isGaugeUping_ = false;

		// 最終的なゲージ値を保存
		SetJobGauge(currentGauge_);
	}
}

void ResultGaugeUI::ChangeJobUI()
{
	TextureTag jobText;

	// ジョブに応じたテクスチャを設定
	switch (GetJobType())
	{
	case JobType::Pikapika:
		jobText = TextureTag::Result_Job_Pikapika;
		break;
	case JobType::Sugoude:
		jobText = TextureTag::Result_Job_Sugoude;
		break;
	case JobType::Tastuzin:
		jobText = TextureTag::Result_Job_Tastuzin;
		break;
	default:
		assert(false && "Invalid JobType");
		break;
	}

	// テクスチャを設定
	uiJob_->SetTexture(jobText);
	uiJobFlash_->SetTexture(jobText);
}

void ResultGaugeUI::SetGaugeBarPercent(float percent)
{
	constexpr float OFFSET = 40.f;

	constexpr float FULL_WIDTH = 1280.f;
	constexpr float HALF_WIDTH = FULL_WIDTH / 2.f;
	constexpr float FULL_HEIGHT = 720.f;

	constexpr float BASE_POS_X = HALF_WIDTH + OFFSET;

	// ゲージバーの位置を設定
	uiGauge_->SetOffset(Vector2(BASE_POS_X - (HALF_WIDTH + OFFSET) * percent, 0.f));

	// ゲージバーのサイズを設定
	uiGauge_->SetSize(Vector2(FULL_WIDTH * percent, FULL_HEIGHT));
}


