#include "GlassShardPool.h"
#include "GameObject.h"
#include "Prefab.h"
#include "PrefabSet.h"
#include "GlassShard.h"
#include <cassert>
#include "DebugLog.h"

GlassShardPool* GlassShardPool::s_instance = nullptr;

void GlassShardPool::Start()
{
	assert(!s_instance && "GlassShardPool‚ª‚·‚Å‚É’è‹`Ï‚Ý");
	s_instance = this;

	World& W = *gameObject->world;

	for (int i = 0; i < MAX; ++i)
	{
		pool_[i] = &Prefab::Instantiate(W, PrefabId::GlassShard);
		pool_[i]->SetActive(false);
	}
}

void GlassShardPool::OnDestroy()
{
	s_instance = nullptr;
}

GameObject* GlassShardPool::Instantiate(const Vector3& pos, GlassGrade grade)
{
	for (int i = 0; i < MAX; ++i)
	{
		if (pool_[i]->IsActiveSelf()) continue;

		pool_[i]->SetActive(true);
		GlassShard* shard = pool_[i]->GetComponent<GlassShard>();
		assert(shard && "GlassShard‚ªNullptr‚Å‚·");
		shard->OnActive(pos);
		shard->SetGlassGrade(grade);
		DebugLog::OutputValue("GlassShardPool::Instantiate", i);
		return pool_[i];
	}

	return nullptr;
}
