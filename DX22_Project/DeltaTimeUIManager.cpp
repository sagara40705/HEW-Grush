#include "DeltaTimeUIManager.h"
#include "DebugLog.h"
void DeltaTimeUIManager::Start()
{
	// 初期同期
	currentValue = 0;
	for (int i = 0; i < Digit::Count; ++i)
	{
		if (!uiNumbers[i]) continue;
		uiNumbers[i]->SetOffset(kNumberPos[i]);
		uiNumbers[i]->SetSize(kNumberSize);
		uiNumbers[i]->SetUVPos(GetNumberUVPos(0));
		uiNumbers[i]->SetUVScale(kNumberUVSize);
	}
}

void DeltaTimeUIManager::OnDestroy()
{
	for (int i = 0; i < Digit::Count; ++i) uiNumbers[i] = nullptr;
}

void DeltaTimeUIManager::Update()
{
	// Timer は外部で Update() 済みを想定しているが、念のため呼んでも安全
	// Timer::Update(); // 呼ぶ場所が他にあるならここは不要

	float dt = Timer::GetDeltaTime();
	int ms = static_cast<int>(dt * 1000.0f + 0.5f); // ミリ秒に丸め
	if (ms < 0) ms = 0;
	if (ms > 9999) ms = 9999;

	if (ms == currentValue) return; // 変化なしなら更新しない
	currentValue = ms;

	// 桁分解
	int digits[Digit::Count];
	digits[Thousands] = (ms / 1000) % 10;
	digits[Hundreds]  = (ms / 100)  % 10;
	digits[Tens]      = (ms / 10)   % 10;
	digits[Ones]      = ms % 10;

	for (int i = 0; i < Digit::Count; ++i)
	{
		CanvasUI* ui = uiNumbers[i];
		if (!ui) continue;

		ui->SetUVPos(GetNumberUVPos(digits[i]));
		ui->SetUVScale(kNumberUVSize);
		ui->SetSize(kNumberSize);
		ui->SetOffset(kNumberPos[i]);
	}
}