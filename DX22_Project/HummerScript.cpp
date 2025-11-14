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

//void HummerScript::Update()
//{
//	//if (!t) return;
//
//	//rotate += 0.1;
//	//Vector3 pos = owner->GetPosition() + Vector3(owner->GetScale().x * sinf(rotate), 0, owner->GetScale().x * cosf(rotate));
//
//	//t->SetPosition(pos);
//	////t->SetRotation(Vector3(0, rotate * 31, 0));
//}

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
}

float HummerScript::GetGaugeRatio()
{
	return (gauge / kMaxGaugeValue[ToInt(grade)]);
}


