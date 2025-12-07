#include "GlassShardPool.h"
#include "GameObject.h"
#include "Prefab.h"
#include "PrefabSet.h"

void GlassShardPool::Start()
{
	World& W = *gameObject->world;

	for (int i = 0; i < MAX; ++i)
	{
		pool_[i] = &Prefab::Instantiate(W, PrefabId::GlassShard);
	}
}

void GlassShardPool::OnDestroy()
{

}
