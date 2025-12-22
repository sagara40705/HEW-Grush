#include "Prefab.h"
#include "World.h"
#include "AllComponet.h"
#include "ModelManager.h"
#include "LineManager.h"
#include "DebugLog.h"

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

	Debug::LogValue("Instantiate PrefabId", static_cast<int>(id));
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
    Prefab::Register(PrefabId::Benti,
        [](GameObject& go) {
            go.SetTag(Tag::Glass);
            auto& mc = go.AddComponent<ModelComponent>(ModelManager::Get(ModelTag::Benti));
            (void)mc;
            auto& mr = go.AddComponent<ModelRenderer>();
            (void)mr;
            auto& bc = go.AddComponent<BoxCollider>(true, DirectX::XMFLOAT3{ 0,1,0 }, DirectX::XMFLOAT3{ 4,2 , 2 });
            (void)bc;
            LineManager::AddBoxCollider(&go);
            auto& gs = go.AddComponent<GlassScript>(GlassGrade::Grade_2);
            (void)gs;

        });
    Prefab::Register(PrefabId::Ufo,
        [](GameObject& go) {
            go.SetTag(Tag::Glass);

            auto& mc = go.AddComponent<ModelComponent>(ModelManager::Get(ModelTag::Ufo));
            (void)mc;
            auto& mr = go.AddComponent<ModelRenderer>();
            (void)mr;
            auto& bc = go.AddComponent<BoxCollider>(true, DirectX::XMFLOAT3{ 0,2,0 }, DirectX::XMFLOAT3{ 4,4 , 4 });
            (void)bc;
            LineManager::AddBoxCollider(&go);
            auto& gs = go.AddComponent<GlassScript>(GlassGrade::Grade_3);
            (void)gs;

        });
    Prefab::Register(PrefabId::ProtPlay,
        [](GameObject& go) {
            go.SetTag(Tag::Glass);
            auto& mc = go.AddComponent<ModelComponent>(ModelManager::Get(ModelTag::ProtPlay));
            (void)mc;
            auto& mr = go.AddComponent<ModelRenderer>();
            (void)mr;
            auto& bc = go.AddComponent<BoxCollider>(true, DirectX::XMFLOAT3{ -0.2f, 1, 0 }, DirectX::XMFLOAT3{ 1, 2, 1 });
            (void)bc;
            LineManager::AddBoxCollider(&go);
            auto& gs = go.AddComponent<GlassScript>(GlassGrade::Grade_1);
            (void)gs;
        });
}
