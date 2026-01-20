#include "ResultTimerUI.h"
#include "GameObject.h"
#include "UiSpriteRenderer.h"
#include "TextureManager.h"
#include "GameState.h"

constexpr Vector2 kNumberUVScale = { 0.2f, 0.5f };
constexpr Vector2 kNumberUVPos[10] =
{
	{0.0f,0.0f},{0.2f,0.0f},{0.4f,0.0f},{0.6f,0.0f},{0.8f,0.0f},
	{0.0f,0.5f},{0.2f,0.5f},{0.4f,0.5f},{0.6f,0.5f},{0.8f,0.5f},
};

void ResultTimerUI::Start()
{

	constexpr Vector2 kMinutePos[ToIndex(TimerDigits::Count)] = {{790.f, 500.f}, {700.f, 500.f} };
	constexpr Vector2 kColonPos = { 900.f,530.f };
	constexpr Vector2 kSecondPos[ToIndex(TimerDigits::Count)] = {{1040.f, 500.f}, {950.f, 500.f} };

	constexpr Vector2 kNumberSize = { 100.f,100.f };
	constexpr Vector2 kColonSize = { 40.f, 40.f };


	GameObject& tenMinute = gameObject->CreateChild();
	uiMinute_[ToIndex(TimerDigits::Ten)] = &tenMinute.AddComponent<UiSpriteRenderer>(TextureTag::ResultTimer, kMinutePos[ToIndex(TimerDigits::Ten)], kNumberSize, Vector2::zero(), kNumberUVScale);

	GameObject& oneMinute = gameObject->CreateChild();
	uiMinute_[ToIndex(TimerDigits::One)] = &oneMinute.AddComponent<UiSpriteRenderer>(TextureTag::ResultTimer, kMinutePos[ToIndex(TimerDigits::One)], kNumberSize, Vector2::zero(), kNumberUVScale);

	GameObject& colon = gameObject->CreateChild();
	uiColon_ = &colon.AddComponent<UiSpriteRenderer>(TextureTag::TimerColon, kColonPos, kColonSize);

	GameObject& tenSecond = gameObject->CreateChild();
	uiSecond_[ToIndex(TimerDigits::Ten)] = &tenSecond.AddComponent<UiSpriteRenderer>(TextureTag::ResultTimer, kSecondPos[ToIndex(TimerDigits::Ten)], kNumberSize, Vector2::zero(), kNumberUVScale);

	GameObject& oneSecond = gameObject->CreateChild();
	uiSecond_[ToIndex(TimerDigits::One)] = &oneSecond.AddComponent<UiSpriteRenderer>(TextureTag::ResultTimer, kSecondPos[ToIndex(TimerDigits::One)], kNumberSize, Vector2::zero(), kNumberUVScale);

}

void ResultTimerUI::Update()
{
	float timer = GetRecentClearTimes();
	int totalSec = static_cast<int>(timer);
	if (totalSec > MAX_SECONDS) totalSec = MAX_SECONDS;

	// ---- 2) mm:ss ‚É•ª‰ð ----
	int minutes = totalSec / 60;
	int seconds = totalSec % 60;

	uiMinute_[ToIndex(TimerDigits::Ten)]->SetUVPos(kNumberUVPos[minutes / 10]);
	uiMinute_[ToIndex(TimerDigits::One)]->SetUVPos(kNumberUVPos[minutes % 10]);
	uiSecond_[ToIndex(TimerDigits::Ten)]->SetUVPos(kNumberUVPos[seconds / 10]);
	uiSecond_[ToIndex(TimerDigits::One)]->SetUVPos(kNumberUVPos[seconds % 10]);
}

void ResultTimerUI::OnDestroy()
{
	uiMinute_[ToIndex(TimerDigits::Ten)] = nullptr;
	uiMinute_[ToIndex(TimerDigits::One)] = nullptr;
	uiColon_ = nullptr;
	uiSecond_[ToIndex(TimerDigits::Ten)] = nullptr;
	uiSecond_[ToIndex(TimerDigits::One)] = nullptr;
}
