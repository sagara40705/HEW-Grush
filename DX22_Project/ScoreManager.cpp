#include "ScoreManager.h"
#include "GameObject.h"
#include "GlassObject.h"
#include "DebugLog.h"
#include "Game.h"
#include "GameState.h"
#include "SceneResult.h"
#include "SceneManager.h"
#include "TimerUI.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "Time/Time.h"

#include <cassert>

void ScoreManager::Start()
{
	constexpr int kMaxScore[ToIndex(StageId::Count)] = { 500, 600, 700, 700, 700 };
	StageId stage = GetCurrentStage();
	assert(stage != StageId::None && "Invalid StageId in ScoreManager::Start");

	score_ = kMaxScore[ToIndex(stage)];

	// TimerUIを検索
	GameObject* timerObj = gameObject->world->FindWithTag(Tag::TimerUI);
	assert(timerObj && "TimerUI not found in ScoreManager::GoToResultScene");
	if (!timerObj) return;
		//TimerUIコンポーネントを取得
	timer_ = timerObj->GetComponent<TimerUI>();
	assert(timer_ && "TimerUI component not found in ScoreManager::GoToResultScene");
	if (!timer_) return;

	GameObject& finishObj = gameObject->CreateChild();
	uiFinish_ = &finishObj.AddComponent<UiSpriteRenderer>
		(TextureTag::Game_UI_Finish, Vector2(130.f, -20.f), Vector2(1024.f, 741.f));
	uiFinish_->SetVisible(false);

	m_CheckFinishSE = false;
}

void ScoreManager::Update()
{
	if (isLock_ && uiFinish_->IsVisible()) // スコアが0になり、演出中
	{
		if(!m_CheckFinishSE)
		{
			SoundManager::SoundPlay(GameFinishSE,1.0f);
			m_CheckFinishSE = true;
		}

		if (m_animationTime < 1.0f)
		{
			m_animationTime += 1.0f / 80.0f; // 約1.3秒
			if (m_animationTime > 1.0f) m_animationTime = 1.0f;

			float bounce = Easing::EaseOutBounce(m_animationTime);
			float yPos = -800.0f + (800.0f * bounce);
			uiFinish_->SetOffset({ 130.0f, yPos });
		}
		else
		{
			// アニメーションが1.0fに達したらシーン切り替え
			GoToResultScene();
			
		}
	}
}

void ScoreManager::UpScore(GlassGrade grade)
{
	constexpr int kUpValue[ToIndex(GlassGrade::Count)] = { 3, 6, 10 };

	if (isLock_) return;

	score_ -= kUpValue[ToIndex(grade)];
	DebugLog::OutputValue("Up Score", score_);

	// スコアが0以下になったら
	if (score_ <= 0.f)
	{
		score_ = 0;
		isLock_ = true;
		Time::SetTimeScale(0.2f);


		m_animationTime = 0.0f;
		uiFinish_->SetVisible(true);
		uiFinish_->SetOffset({ 130.0f, 800.0f });
	}
}

void ScoreManager::Reset()
{
	score_ = 0;
	dirty_ = true;
}

void ScoreManager::GoToResultScene()
{
	// ===== クリア時間をGameStateに保存 =====
	float clearTime = timer_->GetTime();
	SetRecentClearTimes(clearTime);
	Time::SetTimeScale(1.0f); // 初期化
	// ===== シーン切り替え =====
	DebugLog::Output("Clear Stage");
	SceneManager::ChangeSceneAsync<SceneResult>();
	//uiFinish_->SetVisible(false);
}
