// Prefab.cpp （基盤側）

#include "Prefab.h"
#include "World.h"
#include "GameObject.h"
#include "Transform.h"
#include <cassert>

std::unordered_map<PrefabId, Prefab::BuilderFunc> Prefab::s_table{};

void Prefab::Register(PrefabId id, const BuilderFunc& builder)
{
    s_table[id] = builder;
}

GameObject& Prefab::Instantiate(World& world, PrefabId id, const Vector3& pos, const Vector3& rot, const Vector3& scale)
{
    // World 側で Transform を必ず追加する仕様に合わせて、
    // Prefab 側では Transform を追加しない
    GameObject& go = world.CreateGameObject(pos, rot, scale);

    auto it = s_table.find(id);
    assert(it != s_table.end() && "Prefab not registered");

    it->second(go); // Builder が各種 Component を Add

    return go;
}
