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
			goToResultSceneTimer_.Stop();
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

	if (score_ <= 0.f)
	{
		score_ = 0;
		isLock_ = true;	
		goToResultSceneTimer_.Start();
		Time::SetTimeScale(0.2f); //スローモーション
		GoToResultScene();

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

	// ===== シーン切り替え =====
	DebugLog::Output("Clear Stage");
	SceneManager::ChangeSceneAsync<SceneResult>();
}
