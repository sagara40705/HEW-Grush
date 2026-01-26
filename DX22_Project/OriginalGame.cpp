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

void EndTutorial()
{
	Game::GetState()->endTutorial_ = true;
}

bool IsEndTutorial()
{
	return Game::GetState()->endTutorial_;
}

JobType GetSelectedJob()
{
	return Game::GetState()->job_;
}

void UpJobGauge(float amount)
{
	Game::GetState()->jobGauge_ += amount;	// ゲージアップ

	// ジョブUP判定
	if (Game::GetState()->jobGauge_ > 100.f)
	{
		// 最大ジョブ到達済みならゲージを100で固定
		if (Game::GetState()->job_ == JobType::Tastuzin)
		{
			Game::GetState()->jobGauge_ = 100.f;
			return;
		}

		// ジョブアップ
		Game::GetState()->job_ = static_cast<JobType>(static_cast<int>(Game::GetState()->job_) + 1);
		Game::GetState()->jobGauge_ = 0.f;
		return;
	}
}