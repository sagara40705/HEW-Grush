// PrefabSet.cpp （ゲーム固有）

#include "PrefabSet.h"
#include "Prefab.h"
#include "GameObject.h"
#include "AllComponent.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "PrimitiveMeshManager.h"

void RegisterAllPrefabs()
{
    using P = PrefabId;

    Prefab::Register(P::Swing,
        [](GameObject& go) {
            go.SetTag(Tag::Glass);

            auto& mf = go.AddComponent<MeshFilter>(ModelTag::Swing); (void)mf;
            auto& mr = go.AddComponent<MeshRenderer>(); (void)mr;
            auto& bc = go.AddComponent<BoxCollider>(
                true,
                DirectX::XMFLOAT3{ 0, 0.7f, 0.2f },
                DirectX::XMFLOAT3{ 1.5f, 1.5f, 1.5f });
            (void)bc;
            auto& gs = go.AddComponent<GlassObject>(GlassGrade::Grade_1);
            (void)gs;
        });

    Prefab::Register(P::Benti,
        [](GameObject& go) {
            go.SetTag(Tag::Glass);
            auto& mf = go.AddComponent<MeshFilter>(ModelTag::Benti); (void)mf;
            auto& mr = go.AddComponent<MeshRenderer>(); (void)mr;
            auto& bc = go.AddComponent<BoxCollider>(
                true,
                DirectX::XMFLOAT3{ 0, 1, 0 },
                DirectX::XMFLOAT3{ 4, 2, 2 });
            (void)bc;
            auto& gs = go.AddComponent<GlassObject>(GlassGrade::Grade_2); (void)gs;
        });

    Prefab::Register(P::Ufo,
        [](GameObject& go) {
            go.SetTag(Tag::Glass);
            auto& mf = go.AddComponent<MeshFilter>(ModelTag::Ufo); (void)mf;
            auto& mr = go.AddComponent<MeshRenderer>(); (void)mr;
            auto& bc = go.AddComponent<BoxCollider>(
                true,
                DirectX::XMFLOAT3{ 0, 2, 0 },
                DirectX::XMFLOAT3{ 4, 4, 4 });
            (void)bc;
            auto& gs = go.AddComponent<GlassObject>(GlassGrade::Grade_3); (void)gs;
        });

    Prefab::Register(P::ProtPlay,
        [](GameObject& go) {
            go.SetTag(Tag::Glass);
            auto& mf = go.AddComponent<MeshFilter>(ModelTag::ProtPlay); (void)mf;
            auto& mr = go.AddComponent<MeshRenderer>(); (void)mr;
            auto& bc = go.AddComponent<BoxCollider>(
                true,
                DirectX::XMFLOAT3{ -0.2f, 1, 0 },
                DirectX::XMFLOAT3{ 1, 2, 1 });
            (void)bc;
            auto& gs = go.AddComponent<GlassObject>(GlassGrade::Grade_1); (void)gs;
        });
    Prefab::Register(P::GlassShard,
        [](GameObject& go) {
            auto& mf = go.AddComponent<MeshFilter>(PrimitiveMeshTag::GlassShard);                 (void)mf;
            auto& mr = go.AddComponent<MeshRenderer>();                                     (void)mr;
            auto& bc = go.AddComponent<BoxCollider>();  bc.SetMask(ToBit(Layer::Ground));   (void)bc;
            auto& rb = go.AddComponent<Rigidbody>();                                        (void)rb;
        });
}
