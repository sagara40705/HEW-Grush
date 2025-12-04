#pragma once
#include "CanvasUI.h"

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
	CanvasUI* uiColon;                        // コロン
	float currentTimer = 0.f;
	static constexpr int MAX_SECONDS = 99 * 60 + 59; // 99:59 で打ち止め
public:
	TimerUIManager(CanvasUI* tm, CanvasUI* om, CanvasUI* ts, CanvasUI* os, CanvasUI* colon);
	void Start() override;
	void OnDestroy() override;
	void Update() override;
private:
	const float scaleFactor = 1.f / 6.5f; // 
	const Vector2 kNumberPos[TimerDigit::Count] = { {950.f, 20.f},{1020.f, 20.f}, {1130.f, 20.f}, {1200.f, 20.f} };	//位置(四つ分)
	const Vector2 kNumberSize = { 0.5f * scaleFactor, 5.0f * scaleFactor };										//縮小率
	Vector2 GetNumberUVPos(int n) { return { n * 0.1f, 1.f }; }							//UVPos
	const Vector2 kNumberUVSize = { 0.1f, 1.f };										//UVSize
public:
	void TimerStart();

};