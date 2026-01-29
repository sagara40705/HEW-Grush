#include "Hammer.h"
#include "GameObject.h"
#include "Transform.h"
#include "BoxCollider.h"
#include "ModelManager.h"
#include "MeshFilter.h"
#include "MeshRenderer.h"
#include "CameraShake.h"
#include "GlassGaugeManager.h"
#include "ScoreManager.h"
#include "PlayerAnimation.h"
#include "HammerEffects.h"
#include "GlassObject.h"
#include "DebugLog.h"

void Hammer::Start()
{ 
	tf_ = gameObject->GetComponent<Transform>();
	bc_ = gameObject->GetComponent<BoxCollider>();
	mf_ = gameObject->GetComponent<MeshFilter>();
	renderer_ = gameObject->GetComponent<MeshRenderer>();
	playerAnim_ = gameObject->GetComponent<PlayerAnimation>();
	effects_ = gameObject->GetComponent<HammerEffects>();

	GameObject* camera = gameObject->FindWithTag(Tag::MainCamera);
	shake_ = camera->GetComponent<CameraShake>();

	GameObject* gauge = gameObject->FindWithTag(Tag::GaugeManager);
	gauge_ = gauge->GetComponent<GlassGaugeManager>();

	GameObject* score = gameObject->FindWithTag(Tag::ScoreManager);
	score_ = score->GetComponent<ScoreManager>();

	assert(tf_ && bc_ && gauge_ && score_ && "ŽQÆ‚Ì‚Ç‚ê‚©‚ªnullptr‚Å‚·");
	if (!tf_ || !bc_ || !gauge_ || !score_) return;

	bc_->isTrigger = true;
	bc_->isStatic = false;

	GameObject* pivot = gameObject->FindWithTag(Tag::HammerPivot);
	pivotTf_ = pivot->GetComponent<Transform>();

	SetModel();
}

void Hammer::Update()
{
#ifdef _DEBUG
	DebugCommand();
#endif // DEBUG	
}

void Hammer::OnDestroy()
{
	pivotTf_ = nullptr;
	tf_ = nullptr;
	bc_ = nullptr;
	mf_ = nullptr;
	renderer_ = nullptr;
	shake_ = nullptr;
	gauge_ = nullptr;
	score_ = nullptr;
}

void Hammer::OnTriggerEnter(const TriggerInfo& info)
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
	SetModel();
	effects_->Play(HammerEffects::GradeUp, tf_->GetWorldPosition());
	DebugLog::OutputValue("Hammer::GradeUp, Grade", static_cast<int>(grade_) + 1);
}

void Hammer::GradeDown()
{
	grade_ = PrevGrade(grade_);
	//SetScale();
	SetModel();
	DebugLog::OutputValue("Hammer::GradeDown, Grade", static_cast<int>(grade_) + 1);
}

void Hammer::SetScale()
{
	constexpr Vector3 Size[ToIndex(HammerGrade::Count)] = { {0.3f,0.3f,0.8f}, {0.5f,0.5f,0.8f}, {0.8f,0.8f,0.8f} };

	tf_->SetScale(Size[ToIndex(grade_)]);
	DebugLog::OutputValue("Hammer::Scale, Grade", static_cast<int>(grade_) + 1);
}

void Hammer::SetModel()
{
	constexpr ModelTag models[ToIndex(HammerGrade::Count)] = { ModelTag::Hammer_1, ModelTag::Hammer_2, ModelTag::Hammer_3 };

	mf_->SetModel(models[ToIndex(grade_)]);
}

void Hammer::UpdateNormalAttack(float r)
{
	if (!tf_) return;
	if (r < 0.0f) r = 0.0f;
	if (r > 1.0f) r = 1.0f;

	constexpr Vector3 POS_START = { -1.2f, 0.0f, -1.0f };
	constexpr Vector3 POS_END =   { 0.6f, 0.0f, -1.8f };

	constexpr Vector3 ROTATE_START = { 0.f, -45.f, -45.f };
	constexpr Vector3 ROTATE_END = { 0.f, -135.f, -45.f };

	const Vector3 POS = Vector3::Lerp(POS_START, POS_END, r);
	const Vector3 ROTATE = Vector3::Lerp(ROTATE_START, ROTATE_END, r);


	tf_->SetPosition(POS);
	tf_->SetRotation(ROTATE);

}

void Hammer::ResetPos()
{
	tf_->SetPosition(BasePos);
	tf_->SetRotation(BaseRota);

	pivotTf_->SetPosition(Vector3::zero());
	pivotTf_->SetRotation(Vector3::zero());
}

void Hammer::UpdateSpinAttack()
{
	constexpr Vector3 POS = { -0.5f, 0.0f, -1.5f };
	constexpr Vector3 ROTATE = { 0.f, -90.f, -45.f };
	constexpr Vector3 DELTA_ROTATE = { 0.f, -15.f, 0.f };

	tf_->SetPosition(POS);
	tf_->SetRotation(ROTATE);
	pivotTf_->AddRotate(DELTA_ROTATE);
}

void Hammer::UpdateDashAttack(float r)
{

}

void Hammer::Attack(GameObject* go)
{
	const float kDamage[ToIndex(HammerGrade::Count)] = { 1.f, 5.f, 25.f };

	GlassObject* glass = go->GetComponent<GlassObject>();
	if (!glass) return;

	DebugLog::OutputValue("Hammer Attack Glass Object, ID", go->id);
	glass->TakeDamage(kDamage[ToIndex(grade_)]);
	effects_->Play(HammerEffects::Hit, tf_->GetWorldPosition());

	if (!shake_) return;
	shake_->Play(ShakeType::LEVEL_1);

	if (ToIndex(glass->GetGrade()) > ToIndex(grade_))
	{
		if (playerAnim_)
			playerAnim_->CheckNockBackAnime(0.5f);
	}

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

void Hammer::LimitBreakStart()
{
	renderer_->psKind = ShaderList::PS_RAINBOW;
}

void Hammer::LimitBreakEnd()
{
	renderer_->psKind = ShaderList::PS_LAMBERT;
}

#ifdef _DEBUG
#include "InputKeyboard.h"
void Hammer::DebugCommand()
{
	if (InputKeyboard::GetKeyDown('B'))
	{
		if (!shake_) return;
		shake_->Play(ShakeType::LEVEL_1);
		DebugLog::Output("DebugCommand Camera Shake");
	}

	if (InputKeyboard::GetKeyDown('U') && InputKeyboard::GetKey('G'))
	{
		if (!effects_) return;
		effects_->Play(HammerEffects::GradeUp, tf_->GetWorldPosition());
		DebugLog::Output("DebugCommand PlayHammerEffects::GradeUp");
	}


}
#endif // DEBUG	