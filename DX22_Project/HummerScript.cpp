#include "HummerScript.h"
#include "Transform.h"
#include "Collider.h" 
#include "GameObject.h"
#include "Input.h"
#include <algorithm>
#include <cmath>
#include "DebugLog.h"
#include "GlassScript.h"

void HummerScript::Start()
{
	t = Parent->TryGetComponent<Transform>();
	assert(t && "HummerScript requires Transform");
}

void HummerScript::Update()
{
	if (isGaugeDecaying)
	{
		GaugeDecay();
	}
	else 
	{
		// タイマー更新
		// ** 仮実装 **
		timerGaugeDecayDelay += 1.f / 60.f;

		if (timerGaugeDecayDelay >= kGaugeDecayDelay)
		{
			isGaugeDecaying = true;
			timerGaugeDecayDelay = 0.f;
			Debug::Log(" HummerScript::Update - StartGaugeDecay");
		}
	}
}

void HummerScript::OnDestroy()
{
	t = nullptr;
	owner = nullptr;
}

void HummerScript::OnTriggerEnter(const TriggerInfo& c)
{
	if (c.other)
	{
		if (!c.other->CompareTag(Tag::Glass)) return;

		if (CheckGrade(c.other))
		{
			c.other->Destroy();
			//Debug::LogValue(" HummerScript::Destroy id", c.other->id);
			GaugeUp(c.other);
			
			// ゲージ減衰中なら止める & Delayタイマーを初期化
			if (isGaugeDecaying)
			{
				isGaugeDecaying = false;
				timerGaugeDecayDelay = 0.f;
			}
		}
	}
}

bool HummerScript::CheckGrade(GameObject* glass)
{
	GlassScript* gs = glass->TryGetComponent<GlassScript>();
	if (!gs) return false;

	return (ToInt(gs->grade) <= ToInt(grade));
}

void HummerScript::GaugeUp(GameObject* glass)
{
	// 破壊したガラスオブジェクトのGlassScriptを取得
	GlassScript* gs = glass->TryGetComponent<GlassScript>();
	if (!gs) return;

	// ゲージを上昇
	gauge += kGaugeUpValue[ToInt(gs->grade)];
	Debug::LogValue(" HummerScript::gaugeratio", GetGaugeRatio());

	// ゲージの最大値を超えたら
	if (gauge >= kMaxGaugeValue[ToInt(grade)])
	{
		// 1・2 → グレードUP , 3 → 最大値の正規化
		if (grade == HummerGrade::Grade_3) gauge = kMaxGaugeValue[ToInt(HummerGrade::Grade_3)];
		else GradeUp();
	}
}

void HummerScript::GradeUp()
{
	grade = static_cast<HummerGrade>(ToInt(grade) + 1);
	gauge = 0.f;
	Debug::LogValue(" HummerScript::gradelup", ToInt(grade) + 1);
	ScaleByGrade();
}

void HummerScript::GaugeDecay()
{
	// ゲージをマイナス
	// ** 未実装 **

	// ゲージのマイナスを超えたら
	if (gauge < 0.f)
	{
		// 1→ 0に正規化 , 3・2  → グレードDOWN
		if (grade == HummerGrade::Grade_1) gauge = 0.f;
		else GradeDown();
	}
}

void HummerScript::GradeDown()
{
	grade = static_cast<HummerGrade>(ToInt(grade) - 1);
	gauge = kMaxGaugeValue[ToInt(grade)];
	Debug::LogValue(" HummerScript::gradelup", ToInt(grade) + 1);
	ScaleByGrade();
}

void HummerScript::ScaleByGrade()
{
	t->SetScale(kDefaultHummerScale * kHummerScaleRatio[ToInt(grade)]);
}

float HummerScript::GetGaugeRatio()
{
	return (gauge / kMaxGaugeValue[ToInt(grade)]);
}


