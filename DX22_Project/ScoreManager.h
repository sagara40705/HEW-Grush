#pragma once
#include "Component.h"

enum class GlassGrade : std::uint8_t;

class ScoreManager : public Component
{
private:
	int score_ = 10;
	bool dirty_ = true;
public:
	int GetScore() { return score_; }
	bool GetDirty() { return dirty_; }
	void UpScore(GlassGrade grade);
	void SetDirty(bool v) { dirty_ = v; }
	void Reset();
};