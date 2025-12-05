#include "GlassObject.h"
#include "GameObject.h"
#include "Transform.h"
#include "GlassGaugeManager.h"
#include "DebugLog.h"
#include "MeshFilter.h"
#include "MeshRenderer.h"

void GlassObject::Start()
{
	GameObject* gaugeMgr = gameObject->world->FindWithTag(Tag::GaugeManager);
	gaude_ = gaugeMgr->GetComponent<GlassGaugeManager>();


	constexpr float HP[ToIndex(GlassGrade::Count)] = { 1.f, 3.f, 5.f };
	hp_ = HP[ToIndex(grade_)];

	tf_ = gameObject->GetComponent<Transform>();
}

void GlassObject::OnDestroy()
{
	gaude_ = nullptr;
	tf_ = nullptr;
}

void GlassObject::TakeDamage(float damage)
{
	hp_ -= damage;
	if (hp_ <= 0.f)
	{
		gameObject->Destroy();
		gaude_->BrakeGlass(grade_);
		SpawnShards();
		DebugLog::OutputValue("GlassObject Destory, ID", gameObject->id);
	}
}

void GlassObject::SpawnShards()
{
	GameObject& shard = gameObject->world->CreateGameObject(tf_->GetPosition());
	shard.AddComponent<MeshFilter>(PrimitiveMeshTag::Cube);
	shard.AddComponent<MeshRenderer>();
	shard.Destroy(1.0f);
	DebugLog::OutputValue("SpawnShards, ID", shard.id);
}
