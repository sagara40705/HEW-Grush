#include "ScoreManager.h"

void ScoreManager::UpScore(int v)
{
	score_ += v;
}

void ScoreManager::Reset()
{
	score_ = 0;
	dirty_ = true;
}
