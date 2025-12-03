#include "GlassGaugeManager.h"
#include "GameObject.h"
#include "PlayerState.h"
#include "Hammer.h"
#include "GlassObject.h"
#include "InputKeyboard.h"
#include "DebugLog.h"

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

	if (isGaugeEmpty) return;

	// ゲージ減衰中
	if (isGaugeDecaying)
	{
		// ゲージを減衰 + 0をした回ったらGradeDown
		constexpr float kGaugeDecayValue = 0.1f;
		gaude_ -= kGaugeDecayValue;
		if (gaude_ < 0.f)
		{
			GradeDown();
		}
	}
	// 非ゲージ減衰中
	else
	{
		// 減衰までの時間を計る
		constexpr float DT = 0.1f / 60.f;
		gaugeDecayDelayTimer += DT;
		if (gaugeDecayDelayTimer > kGaugeDecayDelayTime)
		{
			isGaugeDecaying = true;
			gaugeDecayDelayTimer = 0.f;
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
	
	//減衰を止めタイマーをリセット
	isGaugeDecaying = false;
	gaugeDecayDelayTimer = 0.f;

	//ゲージを加算
	gaude_ += kGaugeUpVale[ToIndex(grade)];
	isGaugeEmpty = false;
	
	//ゲージの最大値を超えたらグレードUP
	if (gaude_ >kMaxGaugeVale[ToIndex(hammer_->GetGrade())])
	{
		GradeUp();
	}
}

void GlassGaugeManager::GradeUp()
{
	hammer_->GradeUp();

	constexpr float kWhenGradeUpMinGauge = 5;
	gaude_ = kWhenGradeUpMinGauge;
}

void GlassGaugeManager::GradeDown()
{
	if (hammer_->GetGrade() == HammerGrade::Grade_1)
	{
		isGaugeEmpty = true;
		gaude_ = 0.f;
		return;
	}

	hammer_->GradeDown();

	gaude_ = kMaxGaugeVale[ToIndex(hammer_->GetGrade())];
}

void GlassGaugeManager::OnGlassBrake()
{

}

#ifdef _DEBUG
void GlassGaugeManager::DebugCommand()
{
	if (InputKeyboard::GetKeyDown('B'))
	{
		//BrakeGlass(GlassGrade::Grade_1);
		DebugLog::Output("DebugCommand GaugeUp 1");
	}
}
#endif // DEBUG	
