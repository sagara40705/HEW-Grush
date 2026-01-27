#include "GlassGaugeManager.h"
#include "GameObject.h"
#include "PlayerState.h"
#include "Hammer.h"
#include "GlassObject.h"
#include "InputKeyboard.h"
#include "DebugLog.h"
#include "ShaderList.h"
#include "SoundManager.h"

constexpr float kMaxGaugeVale[ToIndex(HammerGrade::Count)] 
				= { 100.f, 200.f, 300.f };

void GlassGaugeManager::Start()
{
	GameObject* player = gameObject->world->FindWithTag(Tag::Player);
	player_ = player->GetComponent<PlayerState>();
	GameObject* hammer = player->FindChildWithTag(Tag::Hammer);
	hammer_ = hammer->GetComponent<Hammer>();

	assert(player_ && hammer_ && "参照のどれかがnullptrです");
	if (!player_ || !hammer_) return;
}

void GlassGaugeManager::Update()
{
	assert(player_ && hammer_ && "参照のどれかがnullptrです");
	if (!player_ || !hammer_) return;

#ifdef _DEBUG
	DebugCommand();
#endif // DEBUG	

	// 限界突破中
	if (isLimitBreak_)
	{
		limitBreakDurationTimer_ += 0.1f / 60.f;
		ShaderList::SetRainbowTime(limitBreakDurationTimer_);

		if (limitBreakDurationTimer_ >= kLimitBreakDuration)
		{
			isLimitBreak_ = false;
			limitBreakDurationTimer_ = 0.f;
			hammer_->LimitBreakEnd();
			DebugLog::Output("限界突破終了");
		}
		return;
	}
	else
	{
		if (!IsMaxGauge())
		{
			limitBreakTimer_ = 0.f;
			//return;
		}

		limitBreakTimer_ += 0.1f / 60.f;
		if (limitBreakTimer_ >= kLimitBreakTime)
		{
			isLimitBreak_ = true;
			limitBreakTimer_ = 0.f;
			hammer_->LimitBreakStart();
			DebugLog::Output("限界突破開始");
		}
	}

	if (isGaugeEmpty) return;

	// ゲージ減衰中
	if (isGaugeDecaying)
	{
		// ゲージを減衰 + 0をした回ったらGradeDown
		dirty_ = true;
		constexpr float kGaugeDecayValue = 0.01f;
		gauge_ -= kGaugeDecayValue;
		if (gauge_ < 0.f)
		{
			GradeDown();
		}
	}
	// 非ゲージ減衰中
	else
	{
		// 減衰までの時間を計る
		constexpr float DT = 0.1f / 60.f;
		constexpr float kGaugeDecayDelayTime = 10.f; //ゲージ減衰までの時間
//		constexpr float kGaugeDecayDelayTime = 1.f; //ゲージ減衰までの時間
		gaugeDecayDelayTimer += DT;
		if (gaugeDecayDelayTimer >=  kGaugeDecayDelayTime)
		{
			isGaugeDecaying = true;
			gaugeDecayDelayTimer = 0.f;
			DebugLog::Output("ゲージ減衰開始");
			SoundManager::SoundPlay(SoundTag::GaugeDownSE, 1.0f);
		}
	}
}

void GlassGaugeManager::OnDestroy()
{
	hammer_ = nullptr;
	player_ = nullptr;
}

void GlassGaugeManager::BrakeGlass(GlassGrade grade)
{
	constexpr float kGaugeUpVale[ToIndex(GlassGrade::Count)] 
					= {10.f, 10.f, 10.f};

	if (isGaugeDecaying)
	{
		SoundManager::StopSound(GaugeDownSE);
	}
	
	//減衰を止めタイマーをリセット
	isGaugeDecaying = false;
	gaugeDecayDelayTimer = 0.f;

	dirty_ = true;
	up_ = true;
	//ゲージを加算
	gauge_ += kGaugeUpVale[ToIndex(grade)];
	isGaugeEmpty = false;
	DebugLog::OutputValue("GlassGaugeManager::BrakeGlass, Gauhe", gauge_);
	
	//ゲージの最大値を超えたらグレードUP
	if (gauge_ > kMaxGaugeVale[ToIndex(hammer_->GetGrade())])
	{
		GradeUp();
	}

	if (isTutorial_ && warkingTask_)
	{
		if (hammer_->GetGrade() == HammerGrade::Grade_1)
		{
			endTask_02_ = true;
		}

		if (hammer_->GetGrade() == HammerGrade::Grade_2)
		{
			endTask_04_ = true;
		}
	}
}

void GlassGaugeManager::GradeUp()
{
	if (hammer_->GetGrade() == HammerGrade::Grade_3)
	{
		gauge_ = kMaxGaugeVale[ToIndex(HammerGrade::Grade_3)];
		return;
	}

	hammer_->GradeUp();
	SoundManager::SoundPlay(SoundTag::GradeUpSE, 1.0f);

	constexpr float kWhenGradeUpMinGauge = 5;
	gauge_ = kWhenGradeUpMinGauge;
}

void GlassGaugeManager::GradeDown()
{
	if (hammer_->GetGrade() == HammerGrade::Grade_1)
	{
		isGaugeEmpty = true;
		gauge_ = 0.f;
		DebugLog::Output("GlassGauge, is Empty");
		SoundManager::SoundPlay(SoundTag::GradeDownSE ,1.0f);
		return;
	}

	hammer_->GradeDown();

	gauge_ = kMaxGaugeVale[ToIndex(hammer_->GetGrade())];
}


float GlassGaugeManager::GetGaugeRatio()
{
	dirty_ = false;
	up_ = false;
	size_t grade = ToIndex(hammer_->GetGrade());
	float ratio = gauge_  / kMaxGaugeVale[grade];

	return  ratio;
}

HammerGrade GlassGaugeManager::GetHammerGrade()
{
	return hammer_->GetGrade();
}

bool GlassGaugeManager::IsMaxGauge()
{
	size_t grade = ToIndex(hammer_->GetGrade());
	if (gauge_ >= kMaxGaugeVale[grade])
	{
		return true;
	}
	return false;
}

#ifdef _DEBUG
void GlassGaugeManager::DebugCommand()
{
	if (InputKeyboard::GetKeyDown('B'))
	{
		BrakeGlass(GlassGrade::Grade_1);
		DebugLog::Output("DebugCommand GaugeUp 1");
	}

	if (InputKeyboard::GetKeyDown('N'))
	{
		isLimitBreak_ = true;
		player_->LimitBreakOn();
		hammer_->LimitBreakStart();
	}
	if (InputKeyboard::GetKeyDown('M'))
	{
		isLimitBreak_ = true;
		player_->LimitBreakOff();
		hammer_->LimitBreakEnd();
	}
}
#endif // DEBUG	
