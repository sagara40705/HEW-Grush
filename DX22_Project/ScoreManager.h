#pragma once
#include "Component.h"
#include "UiSpriteRenderer.h"

enum class GlassGrade : std::uint8_t;
class TimerUI;

class ScoreManager : public MonoBehaviour
{
private:
	int score_ = 500;
	bool dirty_ = true;
	TimerUI* timer_ = nullptr;
	bool isLock_ = false;
	UiSpriteRenderer* uiFinish_ = nullptr;

	float m_animationTime = 0.0f; // アニメーションの経過時間


public:
	void Start() override;
	void Update() override;
public:
	int GetScore() { return score_; }
	bool GetDirty() { return dirty_; }
	void UpScore(GlassGrade grade);
	void SetDirty(bool v) { dirty_ = v; }
	void Reset();
private:
	void GoToResultScene();
    // イージング関数（バウンス・アウト）
	float EaseOutBounce(float x);

	bool m_CheckFinishSE;
};