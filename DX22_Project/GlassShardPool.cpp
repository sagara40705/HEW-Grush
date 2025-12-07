#include "GlassShardPool.h"
#include "GameObject.h"
#include "Prefab.h"
#include "PrefabSet.h"
#include "GlassShard.h"

void GlassShardPool::Start()
{
	World& W = *gameObject->world;

	for (int i = 0; i < MAX; ++i)
	{
		pool_[i] = &Prefab::Instantiate(W, PrefabId::GlassShard);
		pool_[i]->SetActive(false);
	}
}

void GlassShardPool::OnDestroy()
{

}

GameObject* GlassShardPool::Instantiate(const Vector3& pos)
{
	for (int i = 0; i < MAX; ++i)
	{
		if (pool_[i]->IsActiveSelf()) continue;

		pool_[i]->SetActive(true);
		GlassShard* shard = pool_[i]->GetComponent<GlassShard>();
		shard->OnActive(pos);

		return pool_[i];
	}


	return nullptr;
}
