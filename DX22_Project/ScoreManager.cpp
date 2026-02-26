#include "ScoreManager.h"
#include "GameObject.h"
#include "GlassObject.h"
#include "DebugLog.h"
#include "Game.h"
#include "GameState.h"
#include "SceneResult.h"
#include "SceneManager.h"
#include "TimerUI.h"
#include <cassert>
#include "Time/Time.h"

void ScoreManager::Start()
{
	constexpr int kMaxScore[ToIndex(StageId::Count)] = { 50, 600, 700 };
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
}

void ScoreManager::Update()
{
	if (goToResultSceneTimer_.IsRunning())
	{
		if (goToResultSceneTimer_.ElapsedRawSeconds() >= kGoToResultSceneTime)
		{
<<<<<<< HEAD
			goToResultSceneTimer_.Stop();
=======
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
>>>>>>> a414c1e51b6f6c88c93b09de5092ab71a7793e28
			GoToResultScene();
		}
	}
}

void ScoreManager::OnDestroy()
{
	timer_ = nullptr;
}

void ScoreManager::UpScore(GlassGrade grade)
{
	constexpr int kUpValue[ToIndex(GlassGrade::Count)] = { 3, 6, 10 };

	if (isLock_) return;

	score_ -= kUpValue[ToIndex(grade)];
	DebugLog::OutputValue("Up Score", score_);
<<<<<<< HEAD
=======

	// スコアが0以下になったら
>>>>>>> a414c1e51b6f6c88c93b09de5092ab71a7793e28
	if (score_ <= 0.f)
	{
		score_ = 0;
		isLock_ = true;	
		goToResultSceneTimer_.Start();
		Time::SetTimeScale(0.2f); //スローモーション
		GoToResultScene();

<<<<<<< HEAD
=======

		m_animationTime = 0.0f;
		uiFinish_->SetVisible(true);
		uiFinish_->SetOffset({ 130.0f, 800.0f });
>>>>>>> a414c1e51b6f6c88c93b09de5092ab71a7793e28
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
<<<<<<< HEAD

	// ===== シーン切り替え =====
	DebugLog::Output("Clear Stage");
	SceneManager::ChangeSceneAsync<SceneResult>();
=======
	Time::SetTimeScale(1.0f); // 初期化
	// ===== シーン切り替え =====
	DebugLog::Output("Clear Stage");
	SceneManager::ChangeSceneAsync<SceneResult>();
	//uiFinish_->SetVisible(false);
>>>>>>> a414c1e51b6f6c88c93b09de5092ab71a7793e28
}
