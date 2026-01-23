#include "StageWall.h"
#include "GameObject.h"
#include "Camera.h"
#include "PhysicsSystem.h"
#include "Raycast.h"
#include "DebugLog.h"
#include "GroundRenderer.h"
#include "TextureManager.h"

void StageWall::Start()
{
	cam_ = Camera::Main();
	render_ = gameObject->GetComponent<GroundRenderer>();
	render_->SetTexture(TextureManager::Get(TextureTag::Game_Ground));
}

void StageWall::Update()
{
	World& w = *gameObject->world;

	Ray ray = cam_->GetForwardRay();
	RaycastHit hit;
	render_->SetEnabled(true);

	if (!Physics::Raycast(w, ray, &hit, 6, ToBit(Layer::Ground))) return;

	if (hit.gameObject == gameObject) 
	{
		DebugLog::OutputValue("kabeniraygaataxtutayo", hit.gameObject->id);
		render_->SetEnabled(false);
	}
}

void StageWall::OnDestroy()
{
}
