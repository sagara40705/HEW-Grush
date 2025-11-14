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
	float currenTime = 0.f;
public:
	TimerUIManager(CanvasUI* tm, CanvasUI* om, CanvasUI* ts, CanvasUI* os);
	void Start() override;
	void OnDestroy() override;
	void Update() override;
private:
	const Vector2 kNumberPos[TimerDigit::Count] = { {0, 0},{0, 0}, {0, 0}, {0, 0} };	//位置(四つ分)
	const Vector2 kNumberSize = { 1.f, 1.f };											//縮小率
	Vector2 GetNumberUVPos(int n) { return { n * 0.1f, 1.f }; }							//UVPos
	const Vector2 kNumberUVSize = { 0.1f, 1.f };										//UVSize

};