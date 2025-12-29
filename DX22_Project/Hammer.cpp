#include "Hammer.h"
#include "GameObject.h"
#include "Transform.h"
#include "BoxCollider.h"
#include "CameraShake.h"
#include "GlassGaugeManager.h"
#include "ScoreManager.h"
#include "PlayerAnimation.h"

#include "GlassObject.h"
#include "DebugLog.h"

void Hammer::Start()
{ 
	tf_ = gameObject->GetComponent<Transform>();
	bc_ = gameObject->GetComponent<BoxCollider>();

	GameObject* camera = gameObject->FindWithTag(Tag::MainCamera);
	shake_ = camera->GetComponent<CameraShake>();

	GameObject* gauge = gameObject->FindWithTag(Tag::GaugeManager);
	gauge_ = gauge->GetComponent<GlassGaugeManager>();

	GameObject* score = gameObject->FindWithTag(Tag::ScoreManager);
	score_ = score->GetComponent<ScoreManager>();

	GameObject* player = gameObject->FindWithTag(Tag::Player);
	playerAnim_ = player->GetComponent<PlayerAnimation>();

	
	assert(tf_ && bc_ && gauge_ && score_ && "参照のどれかがnullptrです");
	if (!tf_ || !bc_ || !gauge_ || !score_) return;

	bc_->isTrigger = true;
	bc_->isStatic = false;
}

void Hammer::Update()
{
#ifdef _DEBUG
	DebugCommand();
#endif // DEBUG	
}

void Hammer::OnDestroy()
{
	tf_ = nullptr;
	bc_ = nullptr;
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
	constexpr Vector3 Size[ToIndex(HammerGrade::Count)] = { {0.3f,0.3f,0.8f}, {0.5f,0.5f,0.8f}, {0.8f,0.8f,0.8f} };

	//tf_->SetScale(Size[ToIndex(grade_)]);
	DebugLog::OutputValue("Hammer::Scale, Grade", static_cast<int>(grade_) + 1);
}

void Hammer::UpdateNormalAttack(float r)
{
	if (!tf_) return;
	if (r < 0.0f) r = 0.0f;
	if (r > 1.0f) r = 1.0f;

	constexpr Vector3 START = { -2.f, 0.5f, -2.f };
	constexpr Vector3 END =   { 2.f, 0.6f, -2.f };
	constexpr Vector3 ROTATE = { 0.f, -90.f, -45.f };

	const Vector3 OFFSET = Vector3::Lerp(START, END, r);
	tf_->SetPosition(BasePos + OFFSET);
	tf_->SetRotation(ROTATE);

}

void Hammer::ResetPos()
{
	tf_->SetPosition(BasePos);
	tf_->SetRotation(BaseRota);
}

void Hammer::UpdateSpinAttack()
{
	constexpr Vector3 OFFSET = { 0.f, 0.5f, -2.f };
	constexpr Vector3 ROTATE = { 0.f, -90.f, -45.f };
	tf_->SetPosition(BasePos + OFFSET);
	tf_->SetRotation(ROTATE);
}

void Hammer::Attack(GameObject* go)
{
	const float kDamage[ToIndex(HammerGrade::Count)] = { 1.f, 3.f, 5.f };

	GlassObject* glass = go->GetComponent<GlassObject>();
	if (!glass) return;

	DebugLog::OutputValue("Hammer Attack Glass Object, ID", go->id);
	glass->TakeDamage(kDamage[ToIndex(grade_)]);

	if (!shake_) return;
	shake_->Play(ShakeType::LEVEL_1);

	// グレード比較してガラスの方が強ければプレイヤーをノックバックさせる
	if (ToIndex(glass->GetGrade()) > ToIndex(grade_))
	{
		if (playerAnim_)		
			playerAnim_->CheckLeanBackAnime(3.f);
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
}
#endif // DEBUG	