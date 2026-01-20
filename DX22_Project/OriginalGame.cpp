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


// 直近クリアしたステージのタイムを取得・設定
float GetRecentClearTimes()
{
	return Game::GetState()->recentClearTimes_;
}
void SetRecentClearTimes(float time)
{
	Game::GetState()->recentClearTimes_ = time;
}

void EndTutorial()
{
	Game::GetState()->endTutorial_ = true;
}

bool IsEndTutorial()
{
	return Game::GetState()->endTutorial_;
}