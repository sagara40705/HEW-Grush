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