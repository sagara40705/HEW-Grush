#pragma once
#include "Component.h"
<<<<<<< HEAD
#include "Time/Timer.h"
=======
#include "UiSpriteRenderer.h"
#include "Easing.h"
>>>>>>> a414c1e51b6f6c88c93b09de5092ab71a7793e28

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
<<<<<<< HEAD
private:
	//演出用タイマー
	Timer goToResultSceneTimer_;
	static constexpr float kGoToResultSceneTime = 2.0f; //リザルトシーンへ行くまでの時間
=======
	bool m_CheckFinishSE;
>>>>>>> a414c1e51b6f6c88c93b09de5092ab71a7793e28
};