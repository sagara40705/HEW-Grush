#include "ScoreManager.h"
#include "GlassObject.h"
#include "DebugLog.h"
#include "Game.h"
#include "GameState.h"
#include "SceneTitle.h"

void ScoreManager::UpScore(GlassGrade grade)
{
	constexpr int kUpValue[ToIndex(GlassGrade::Count)] = { 3, 6, 10 };

	score_ -= kUpValue[ToIndex(grade)];
	DebugLog::OutputValue("Up Score", score_);

	if (score_ <= 0.f)
	{
		DebugLog::Output("Score Max");
		SceneManager::ChangeScene<SceneTitle>();
	}
}

void ScoreManager::Reset()
{
	score_ = 0;
	dirty_ = true;
}
