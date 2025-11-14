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
}

void TimerUIManager::OnDestroy()
{
	uiNumbers[TimerDigit::TensMinute] = nullptr;
	uiNumbers[TimerDigit::OnesMinute] = nullptr;
	uiNumbers[TimerDigit::TensSecond] = nullptr;
	uiNumbers[TimerDigit::OnesSecond] = nullptr;
}

void TimerUIManager::Update()
{

}
