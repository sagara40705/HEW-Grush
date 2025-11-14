#pragma once
#include "CanvasUI.h"

// Timer = [m1][m2] : [s1][s2]

class TimerUIManager : public MonoBehaviour
{
public:
	enum TimerDigit
	{
		TensMinute,   // 十の位（分）
		OnesMinute,   // 一の位（分）
		TensSecond,   // 十の位（秒）
		OnesSecond,   // 一の位（秒）
		Count
	};
private:
	CanvasUI* uiNumbers[TimerDigit::Count]{}; // 分分 秒秒の四つ
	float currenTime = 0.f;
public:
	TimerUIManager(CanvasUI* tm, CanvasUI* om, CanvasUI* ts, CanvasUI* os);
	void Start() override;
	void OnDestroy() override;
	void Update() override;
};