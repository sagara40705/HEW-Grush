#include "Game.h"
#include "GameState.h"

std::unique_ptr<GameState> Game::s_state;

void Game::OriginalInit()
{
	s_state = std::make_unique<GameState>();
	s_state->Reset();
}
void Game::OriginalUninit()
{
	s_state.reset();
}

bool HasPlayedIntroMovie()
{
	return Game::GetState()->hasPlayedIntroMovie;
}

void PlayedIntroMovie()
{
	Game::GetState()->hasPlayedIntroMovie = true;
}

// 現在のステージを取得
StageId GetCurrentStage()
{
	return Game::GetState()->stage_;
}
void SetSelectedStage(StageId id)
{
	Game::GetState()->stage_ = id;
}


// 直近クリアしたステージのタイムを取得・設定
float GetRecentClearTimes()
{
	return Game::GetState()->recentClearTimes_;
}
void SetRecentClearTimes(float time)
{
	Game::GetState()->recentClearTimes_ = time;
}

// チュートリアル終了フラグの取得・設定
void EndTutorial()
{
	Game::GetState()->endTutorial_ = true;
}
bool IsEndTutorial()
{
	return Game::GetState()->endTutorial_;
}

// ジョブタイプの取得・設定
JobType GetJobType()
{
	return Game::GetState()->job_;
}
void SetJobType(JobType job)
{
	Game::GetState()->job_ = job;
}

// ジョブゲージの取得・設定
int GetJobGauge()
{
	return Game::GetState()->jobGauge_;
}
void SetJobGauge(int gauge)
{
	Game::GetState()->jobGauge_ = gauge;
}

// スタートUI用(他を動かさないようにするため)
bool IsGameLocked()
{
	return Game::GetState()->isLock_;
}
void SetGameLock(bool lock)
{
	Game::GetState()->isLock_ = lock;
}