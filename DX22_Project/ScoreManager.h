#pragma once
#include "Component.h"

enum class GlassGrade : std::uint8_t;
class TimerUI;

class ScoreManager : public Component
{
private:
	int score_ = 500;
	bool dirty_ = true;
	TimerUI* timer_ = nullptr;
	bool isLock_ = false;


public:
	void Start() override;
public:
	int GetScore() { return score_; }
	bool GetDirty() { return dirty_; }
	void UpScore(GlassGrade grade);
	void SetDirty(bool v) { dirty_ = v; }
	void Reset();
private:
	void GoToResultScene();

};