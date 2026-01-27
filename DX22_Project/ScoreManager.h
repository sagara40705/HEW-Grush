#pragma once
#include "Component.h"
#include "Time/Timer.h"

enum class GlassGrade : std::uint8_t;
class TimerUI;

class ScoreManager : public MonoBehaviour
{
private:
	int score_ = 500;
	bool dirty_ = true;
	TimerUI* timer_ = nullptr;
	bool isLock_ = false;
public:
	void Start() override;
	void Update() override;
	void OnDestroy() override;
public:
	int GetScore() { return score_; }
	bool GetDirty() { return dirty_; }
	void UpScore(GlassGrade grade);
	void SetDirty(bool v) { dirty_ = v; }
	void Reset();
private:
	void GoToResultScene();
private:
	//演出用タイマー
	Timer goToResultSceneTimer_;
	static constexpr float kGoToResultSceneTime = 2.0f; //リザルトシーンへ行くまでの時間
};