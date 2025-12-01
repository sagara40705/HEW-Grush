#include "CameraContorol.h"
#include "GameObject.h"
#include "Transform.h"

void CameraControl::Start()
{
	GameObject* palyer = gameObject->FindWithTag(Tag::Player);

	playerTf_ = palyer->transform;
	selfTf_ = gameObject->transform;
}

void CameraControl::OnDestroy()
{
	playerTf_ = nullptr;
	selfTf_ = nullptr;
}

void CameraControl::Update()
{
	FollowPlayer();
}

void CameraControl::FollowPlayer()
{
	selfTf_->SetPosition(playerTf_->GetWorldPosition());
}
