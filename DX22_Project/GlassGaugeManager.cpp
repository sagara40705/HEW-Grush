#include "GlassGaugeManager.h"
#include "GameObject.h"
#include "PlayerState.h"
#include "Hammer.h"
#include "GlassObject.h"

constexpr float kMaxGaugeVale[ToIndex(HummerGrade::Count)] 
				= { 10.f, 20.f, 30.f };

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

	// ゲージ減衰中
	if (isGaugeDecaying)
	{
		// ゲージを減衰 + 0をした回ったらGradeDown
		constexpr float kGaugeDecayValue = 0.1;
		if (gaude_ -= kGaugeDecayValue  < 0.f)
		{
			GradeDown();
		}
	}
	// 非ゲージ減衰中
	else
	{
		// 減衰までの時間を計る
		constexpr float DT = 0.1 / 60.f;
		if (gaugeDecayDelayTimer += DT > kGaugeDecayDelayTime)
		{
			isGaugeDecaying = true;
			gaugeDecayDelayTimer = 0.f;
		}
	}
}

void GlassGaugeManager::OnDestroy()
{
	Hammer* hammer_ = nullptr;
	PlayerState* player_ = nullptr;
}

void GlassGaugeManager::BrakeGlass(GlassGrade grade)
{
	constexpr float kGaugeUpVale[ToIndex(GlassGrade::Count)] = {2.f, 5.f, 10.f};
	
	//減衰を止めタイマーをリセット
	isGaugeDecaying = false;
	gaugeDecayDelayTimer = 0.f;
	
	//ゲージの最大値を超えたらグレードUP
	if (gaude_ += kGaugeUpVale[ToIndex(grade)] > 
		kMaxGaugeVale[ToIndex(hammer_->GetGrade())])
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
	hammer_->GradeDown();

	gaude_ = kMaxGaugeVale[ToIndex(hammer_->GetGrade())];
}

void GlassGaugeManager::OnGlassBrake()
{

}
