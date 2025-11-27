#pragma once
#include "Component.h" // MonoBehaviour
#include "CanvasUI.h"

class PlayerIntroSequence : public MonoBehaviour
{
private:
	Rigidbody* rb_ = nullptr;
	bool isIntro = true;
private:
	const float kIntroTime = 3.0f;
	float introTimer = 0.f;
public:
	PlayerIntroSequence(CanvasUI* inUi);
	void Start() override;
	void Update() override;
	void OnDestroy() override;

	void IntroEffect();
private:
	void FinishIntro();
private:
	//Effect再生用
	CanvasUI* inUi;
	int   efFrame = 0;        // 現在のフレーム番号
	int efCounter = 0;        // カウンタ
	bool  efPlaying = false;  // 再生中フラグ
	
};