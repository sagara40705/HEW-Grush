#include "Hammer.h"
#include "GameObject.h"
#include "Transform.h"
#include "BoxCollider.h"
#include "GlassObject.h"
#include "DebugLog.h"

void Hammer::Start()
{ 
	tf_ = gameObject->GetComponent<Transform>();
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
	tf_ = nullptr;
	bc_ = nullptr;
}

void Hammer::OnTriggerStay(const TriggerInfo& info)
{
	//DebugLog::OutputValue("Hammer::OnTriggerStay", info.other->id);
	if (info.other->CompareTag(Tag::Glass))
	{
		//DebugLog::Output("Hammer::OnTriggerStay");
		Attack(info.other);
	}
}

void Hammer::GradeUp()
{
	grade_ = NextGrade(grade_);
	SetScale();
	DebugLog::OutputValue("Hammer::GradeUp, Grade", static_cast<int>(grade_) + 1);
}

void Hammer::GradeDown()
{
	grade_ = PrevGrade(grade_);
	SetScale();
	DebugLog::OutputValue("Hammer::GradeDown, Grade", static_cast<int>(grade_) + 1);
}

void Hammer::SetScale()
{
	constexpr Vector3 Size[ToIndex(HammerGrade::Count)] = { {0.3f,0.3f,0.3f}, {0.5f,0.5f,0.5f}, {0.8f,0.8f,0.8f} };

	tf_->SetScale(Size[ToIndex(grade_)]);
	DebugLog::OutputValue("Hammer::Scale, Grade", static_cast<int>(grade_) + 1);
}

void Hammer::Attack(GameObject* go)
{
	const float kDamage[ToIndex(HammerGrade::Count)] = { 1.f, 3.f, 5.f };

	GlassObject* glass = go->GetComponent<GlassObject>();
	if (!glass) return;

	DebugLog::OutputValue("Hammer Attack Glass Object, ID", go->id);
	glass->TakeDamage(0.5f);

	//if (CanBrake(glass->GetGrade()))
	//{
	//	go->Destroy();
	//	DebugLog::Output("Hammer::CanBrake");
	//}
	//else
	//{
	//	//glass->TakeDamage(1);
	//}
}

bool Hammer::CanBrake(GlassGrade glass)
{
	return (ToIndex(glass) <= ToIndex(grade_));
}

