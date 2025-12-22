#pragma once
#include "CanvasUI.h"
#include "Component.h"
#include "Vector2.h"
#include "Timer.h"

class DeltaTimeUIManager : public MonoBehaviour
{
public:
	enum Digit
	{
		Thousands,
		Hundreds,
		Tens,
		Ones,
		Count
	};

	DeltaTimeUIManager(CanvasUI* d0, CanvasUI* d1, CanvasUI* d2, CanvasUI* d3)
	{
		uiNumbers[Thousands] = d0;
		uiNumbers[Hundreds] = d1;
		uiNumbers[Tens] = d2;
		uiNumbers[Ones] = d3;
	}

	void Start() override;
	void OnDestroy() override;
	void Update() override;

private:
	CanvasUI* uiNumbers[Digit::Count]{};
	int currentValue = 0; // ミリ秒

	// 表示位置・サイズ（左下）
	const Vector2 kNumberPos[Digit::Count] = {
		{ 10.f, 680.f }, { 40.f, 680.f }, { 70.f, 680.f }, { 100.f, 680.f }
	};
	const Vector2 kNumberSize = { 0.3f, 3.0f };
	Vector2 GetNumberUVPos(int n) { return { n * 0.1f, 1.f }; }
	const Vector2 kNumberUVSize = { 0.1f, 1.f };
};