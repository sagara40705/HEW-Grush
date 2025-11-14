#include "TimerUIManager.h"
#include "GameObject.h"
#include "Timer.h"

TimerUIManager::TimerUIManager(CanvasUI* tm, CanvasUI* om, CanvasUI* ts, CanvasUI* os)
{
	if (!tm || !om || !ts || !os) return;

	uiNumbers[TimerDigit::TensMinute] = tm;
	uiNumbers[TimerDigit::OnesMinute] = om;
	uiNumbers[TimerDigit::TensSecond] = ts;
	uiNumbers[TimerDigit::OnesSecond] = os;
}

void TimerUIManager::Start()
{
	currentTimer = 0.f;

	for (int i = 0; i < TimerDigit::Count; ++i)
	{
		if (!uiNumbers[i]) continue;

		// 位置・サイズ設定
		uiNumbers[i]->SetOffset(kNumberPos[i]);
		uiNumbers[i]->SetSize(kNumberSize);

		// 数字 0 の UV にしておく
		uiNumbers[i]->SetUVPos(GetNumberUVPos(0));
		uiNumbers[i]->SetUVScale(kNumberUVSize);
	}
}

void TimerUIManager::OnDestroy()
{
	for (int i = 0; i < TimerDigit::Count; ++i)
	{
		uiNumbers[i] = nullptr;
	}
}

void TimerUIManager::Update()
{
	// ---- 1) 経過時間を増やす ----
	currentTimer += 1.f / 60.f;

	// 上限 99:59 でクランプ
	int totalSec = static_cast<int>(currentTimer);
	if (totalSec > MAX_SECONDS) totalSec = MAX_SECONDS;

	// ---- 2) mm:ss に分解 ----
	int minutes = totalSec / 60;
	int seconds = totalSec % 60;

	int digits[TimerDigit::Count];
	digits[TensMinute] = (minutes / 10) % 10;
	digits[OnesMinute] = minutes % 10;
	digits[TensSecond] = (seconds / 10) % 10;
	digits[OnesSecond] = seconds % 10;

	// ---- 3) 各 CanvasUI の UV を更新 ----
	for (int i = 0; i < TimerDigit::Count; ++i)
	{
		CanvasUI* ui = uiNumbers[i];
		if (!ui) continue;

		const int d = digits[i];          // 0〜9
		ui->SetUVPos(GetNumberUVPos(d));  // 「d 桁目」の左上に UV を移動
	}
}
