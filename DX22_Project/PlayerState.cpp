#include "PlayerState.h"
#include "GameObject.h"
#include "MeshRenderer.h"


void PlayerState::Start()
{
	renderer_ = gameObject->GetComponent<MeshRenderer>();
}

void PlayerState::OnDestroy()
{
	renderer_ = nullptr;
}

void PlayerState::LimitBreakOn()
{
	renderer_->psKind = ShaderList::PS_RAINBOW;
}

void PlayerState::LimitBreakOff()
{
	renderer_->psKind = ShaderList::PS_LAMBERT;
}
