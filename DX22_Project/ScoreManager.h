#pragma once
#include "Component.h"

class ScoreManager : public Component
{
private:
	int score_ = 0;
	bool dirty_ = true;
public:
	int GetScore() { return score_; }
	bool GetDirty() { return dirty_; }
	void UpScore(int v);
	void SetDirty(bool v) { dirty_ = v; }
	void Reset();
};