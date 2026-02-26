#include "TimerUI.h"
#include "GameObject.h"
#include "UiSpriteRenderer.h"
#include "TextureManager.h"
#include "GameState.h"


constexpr Vector2 kNumberUVScale = { 0.1f, 1.0f };

void TimerUI::Awake()
{
	constexpr Vector2 kMinutePos[ToIndex(TimerDigits::Count)] = { {1085.f, 47.f}, {1120.f, 47.f} };
	constexpr Vector2 kColonPos = { 1158.f,49.f };
	constexpr Vector2 kSecondPos[ToIndex(TimerDigits::Count)] = { {1175.f, 47.f}, {1210.f, 47.f} };

	constexpr Vector2 kNumberSize = { 35.f, 35.f };
	constexpr Vector2 kColonSize = { 15.f, 30.f };


	GameObject& tenMinute = gameObject->CreateChild();
	uiMinute_[ToIndex(TimerDigits::Ten)] = &tenMinute.AddComponent<UiSpriteRenderer>
		(TextureTag::Game_UI_Timer_Number, kMinutePos[ToIndex(TimerDigits::Ten)], kNumberSize, Vector2::zero(), kNumberUVScale);

	GameObject& oneMinute = gameObject->CreateChild();
	uiMinute_[ToIndex(TimerDigits::One)] = &oneMinute.AddComponent<UiSpriteRenderer>
		(TextureTag::Game_UI_Timer_Number, kMinutePos[ToIndex(TimerDigits::One)], kNumberSize, Vector2::zero(), kNumberUVScale);

	GameObject& colon = gameObject->CreateChild();
	uiColon_ = &colon.AddComponent<UiSpriteRenderer>(TextureTag::Game_UI_Timer_Colon, kColonPos, kColonSize);

	GameObject& tenSecond = gameObject->CreateChild();
	uiSecond_[ToIndex(TimerDigits::Ten)] = &tenSecond.AddComponent<UiSpriteRenderer>
		(TextureTag::Game_UI_Timer_Number, kSecondPos[ToIndex(TimerDigits::Ten)], kNumberSize, Vector2::zero(), kNumberUVScale);

	GameObject& oneSecond = gameObject->CreateChild();
	uiSecond_[ToIndex(TimerDigits::One)] = &oneSecond.AddComponent<UiSpriteRenderer>
		(TextureTag::Game_UI_Timer_Number, kSecondPos[ToIndex(TimerDigits::One)], kNumberSize, Vector2::zero(), kNumberUVScale);
}

void TimerUI::Start()
{
	timer_ = 0.f;

}

void TimerUI::Update()
{
	if (IsGameLocked()) return;

	if (scoreManager_ != nullptr)
	{
		score_ = scoreManager_->GetScore();
	}
	else
	{
		// ScoreManagerを探す
		GameObject* scoreManagerObj = gameObject->world->FindWithTag(Tag::ScoreManager);
		if (scoreManagerObj)
		{
			scoreManager_ = scoreManagerObj->GetComponent<ScoreManager>();
			if (scoreManager_)
			{
				score_ = scoreManager_->GetScore();
			}
		}
	}

	

	// ---- 1) タイマー更新 ----
	if (score_ <= 0) return; // スコアが0以下ならタイマー停止
	timer_ += 1.f / 60.f;
	// 上限 99:59 でクランプ
	int totalSec = static_cast<int>(timer_);
	if (totalSec > MAX_SECONDS) totalSec = MAX_SECONDS;

	// ---- 2) mm:ss に分解 ----
	int minutes = totalSec / 60;
	int seconds = totalSec % 60;

	uiMinute_[ToIndex(TimerDigits::Ten)]->SetUVPosX((minutes % 10) * 0.1f);
	uiMinute_[ToIndex(TimerDigits::One)]->SetUVPosX((minutes / 10) % 10 * 0.1f);
	uiSecond_[ToIndex(TimerDigits::Ten)]->SetUVPosX((seconds % 10) * 0.1f);
	uiSecond_[ToIndex(TimerDigits::One)]->SetUVPosX((seconds / 10) % 10 * 0.1f);
}

void TimerUI::OnDestroy()
{
	SetRecentClearTimes(timer_);

	uiMinute_[ToIndex(TimerDigits::Ten)] = nullptr;
	uiMinute_[ToIndex(TimerDigits::One)] = nullptr;
	uiColon_ = nullptr;
	uiSecond_[ToIndex(TimerDigits::Ten)] = nullptr;
	uiSecond_[ToIndex(TimerDigits::One)] = nullptr;
	scoreManager_ = nullptr;
}
