#include "TitlePlyerContoroll.h"
#include "GameObject.h"
#include "Transform.h"
#include "Animator.h"
#include "InputKeyboard.h"
#include "InputGamepad.h"
#include "ModelManager.h"

void TitlePlyerContoroll::Start()
{
	GameObject* pivotObj = gameObject->FindWithTag(Tag::HammerPivot);
	if (pivotObj)
	{
		pivotTf_ = pivotObj->GetComponent<Transform>();
	}

	GameObject* hammerObj = gameObject->FindWithTag(Tag::Hammer);
	if (hammerObj)
	{
		hammerTf_ = hammerObj->GetComponent<Transform>();
	}

	anime_ = gameObject->GetComponent<Animator>();
	anime_->Start();
	AddAnimation();
	anime_->Play(idleAnimeNo_, false, 1.0f);
	anime_->SetTimeSeconds(0.0f);
	isAnimation_ = false;
}

void TitlePlyerContoroll::Update()
{	
	if (!hammerTf_ || !anime_ || !pivotTf_) return;

	if (isAnimation_)
	{
		UpdateAnimation();
	}
	else
	{
		if (anime_->GetCurrentAnimeTime() >= 4.0f)
		{
			anime_->SetTimeSeconds(0.1f);
		}
		UpdateInput();
	}
}

void TitlePlyerContoroll::OnDestroy()
{
	hammerTf_ = nullptr;
	anime_ = nullptr;
}

void TitlePlyerContoroll::AddAnimation()
{
	Model* model = ModelManager::Get(ModelTag::Player_3);
	if (!model) return;

	idleAnimeNo_ = model->AddAnimation("Assets/Player/Animation/playerchara_hammer_taiki.fbx");
	spinAnimeNo_ = model->AddAnimation("Assets/Player/Animation/playerchara_hammer_rotateattack.fbx");
}

void TitlePlyerContoroll::UpdateAnimation()
{
	if (!animationTimer_.IsRunning()) return;

	if (animationTimer_.ElapsedRawSeconds() >= kAnimationTime)
	{
		animationTimer_.Stop();
		anime_->Stop();
		return;
	}
	else
	{
		if (animationTimer_.ElapsedRawSeconds() >= 0.4f)
		{
			constexpr Vector3 DELTA_ROTATE = { 0.f, -15.5f, 0.f };
			pivotTf_->AddRotate(DELTA_ROTATE);
		}
		else
		{
			constexpr Vector3 DELTA_ROTATE = { 0.f, -15.5f, 0.f };
			pivotTf_->AddRotate(DELTA_ROTATE);
		}
		
		return;
	}
}

void TitlePlyerContoroll::UpdateInput()
{
	if (InputKeyboard::GetKeyDown(VK_SPACE) || InputGamepad::GetButtonDown(XINPUT_GAMEPAD_X))
	{
		isAnimation_ = true;
		animationTimer_.Start();
		anime_->Play(spinAnimeNo_, false, 1.5f);
		anime_->SetTimeSeconds(0.0f);

		constexpr Vector3 POS = { -0.5f, 0.0f, -1.5f };
		constexpr Vector3 ROTATE = { 0.f, -90.f, -45.f };
		hammerTf_->SetPosition(POS);
		hammerTf_->SetRotation(ROTATE);
	}
}
