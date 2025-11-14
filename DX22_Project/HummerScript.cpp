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

		if (CheckLevel(c.other))
		{
			c.other->Destroy();
			//Debug::LogValue(" HummerScript::Destroy id", c.other->id);
			GaugeUp(c.other);
			LevelUp();
		}
	}
}

bool HummerScript::CheckLevel(GameObject* glass)
{
	GlassScript* gs = glass->TryGetComponent<GlassScript>();
	if (!gs) return false;

	return (gs->level <= level);
}

void HummerScript::GaugeUp(GameObject* glass)
{
	GlassScript* gs = glass->TryGetComponent<GlassScript>();
	if (!gs) return;

	gauge += kGaugeUpValue[gs->level - 1];
	Debug::LogValue(" HummerScript::gauge", gauge);
}

void HummerScript::LevelUp()
{
	if (gauge >= kMaxGaugeValue[level - 1] && level < 3)
	{
		level++;
		gauge = 0;
		Debug::LogValue(" HummerScript::levelup", level);

	}
}


