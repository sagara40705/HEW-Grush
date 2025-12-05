#include "Hammer.h"
#include "GameObject.h"
#include "BoxCollider.h"
#include "GlassObject.h"
#include "DebugLog.h"

void Hammer::Start()
{
	bc_ = gameObject->GetComponent<BoxCollider>();

	assert(bc_ && "ŽQÆ‚Ì‚Ç‚ê‚©‚ªnullptr‚Å‚·");
	if (!bc_) return;

	bc_->isTrigger = true;
	bc_->isStatic = false;
}

void Hammer::Update()
{
}

void Hammer::OnDestroy()
{
}

void Hammer::OnTriggerStay(const TriggerInfo& info)
{
	if (info.other->CompareTag(Tag::Glass))
	{
		DebugLog::Output("Hammer::OnTriggerStay");
		Attack(info.other);
		//info.other->Destroy();
	}
}

void Hammer::GradeUp()
{
}

void Hammer::GradeDown()
{
}

void Hammer::SetScale()
{
}

void Hammer::Attack(GameObject* go)
{
	const int kDamage[ToIndex(HammerGrade::Count)] = { 1,3,5 };

	GlassObject* glass = go->GetComponent<GlassObject>();
	if (!glass) return;

	if (CanBrake(glass->GetGrade()))
	{
		go->Destroy();
		DebugLog::Output("Hammer::CanBrake");
	}
	else
	{
		//glass->TakeDamage(1);
	}
}

bool Hammer::CanBrake(GlassGrade glass)
{
	return (ToIndex(glass) <= ToIndex(grade_));
}

