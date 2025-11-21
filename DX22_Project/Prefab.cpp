#include "Prefab.h"
#include "AllComponet.h"
#include "ModelManager.h"
#include "LineManager.h"

std::unordered_map<PrefabId, Prefab::BuilderFunc> Prefab::s_table{};

void Prefab::Register(PrefabId id, const BuilderFunc& builder)
{
	s_table[id] = builder;
}

GameObject& Prefab::Instantiate(World& world, PrefabId id, const Vector3& pos, const Vector3& rot, const Vector3& scale)
{
	// 1) GameObject çÏê¨
	GameObject& go = world.CreateGameObject();

	// 2) ã§í èàóù
	auto& tf = go.AddComponent<Transform>(pos, rot, scale);

	// 3) ìoò^çœÇ›ÇÃ Builder Çåüçı
	auto it = s_table.find(id);
	assert(it != s_table.end() && "Prefab not registered");

	// 4) Builder Ç≈ç\ízÇ∑ÇÈ
	it->second(go);

	return go;
}

void RegisterAllPrefabs()
{
    using P = PrefabId;

    Prefab::Register(PrefabId::Swing, 
        [](GameObject& go) {
            go.SetTag(Tag::Glass);

            auto& mc = go.AddComponent<ModelComponent>(ModelManager::Get(ModelTag::Swing));
            (void)mc;
            auto& mr = go.AddComponent<ModelRenderer>();
            (void)mr;
            auto& bc = go.AddComponent<BoxCollider>(true, DirectX::XMFLOAT3{ 0, 0.7f, 0.2f }, DirectX::XMFLOAT3{ 1.5f, 1.5f , 1.5f });
            (void)bc;
            LineManager::AddBoxCollider(&go);
            auto& gs = go.AddComponent<GlassScript>(GlassGrade::Grade_1);
            (void)gs;
        });
}
