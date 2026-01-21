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

            auto& mf = go.AddComponent<MeshFilter>(ModelTag::Swing); mf.SetBoundsOffset({ 0.f, 0.7f, 0.2f });    (void)mf;
            auto& mr = go.AddComponent<MeshRenderer>(); (void)mr;
            auto& bc = go.AddComponent<BoxCollider>(
                true,
                DirectX::XMFLOAT3{ 0, 0.7f, 0.2f },
                DirectX::XMFLOAT3{ 1.5f, 1.4f, 1.5f });
            (void)bc;
            auto& gs = go.AddComponent<GlassObject>(GlassGrade::Grade_1);
            (void)gs;
        });

    Prefab::Register(P::Akican,
        [](GameObject& go) {
            go.SetTag(Tag::Glass);

            auto& mf = go.AddComponent<MeshFilter>(ModelTag::Akican); mf.SetBoundsOffset({ 0.f, 0.5f, 0.f });    (void)mf;
            auto& mr = go.AddComponent<MeshRenderer>(); (void)mr;
            auto& bc = go.AddComponent<BoxCollider>(
                true,
                DirectX::XMFLOAT3{ 0.f, 0.5f, 0.f },
                DirectX::XMFLOAT3{ 0.7f, 1.0f, 0.7f });
            (void)bc;
            auto& gs = go.AddComponent<GlassObject>(GlassGrade::Grade_1);
            (void)gs;
        });

    Prefab::Register(P::SagaraDoll,
        [](GameObject& go) {
            go.SetTag(Tag::Glass);

            auto& mf = go.AddComponent<MeshFilter>(ModelTag::SagaraDoll); mf.SetBoundsOffset({ 0.f, 0.5f, 0.f });    (void)mf;
            auto& mr = go.AddComponent<MeshRenderer>(); (void)mr;
            auto& bc = go.AddComponent<BoxCollider>(
                true,
                DirectX::XMFLOAT3{ 0.f, 0.5f, 0.f },
                DirectX::XMFLOAT3{ 0.7f, 1.0f, 0.7f });
            (void)bc;
            auto& gs = go.AddComponent<GlassObject>(GlassGrade::Grade_1);
            (void)gs;
        });

    Prefab::Register(P::Desktop,
        [](GameObject& go) {
            go.SetTag(Tag::Glass);

            auto& mf = go.AddComponent<MeshFilter>(ModelTag::Desktop); mf.SetBoundsOffset({ -0.3f, 0.8f, 0.5f });    (void)mf;
            auto& mr = go.AddComponent<MeshRenderer>(); (void)mr;
            auto& bc = go.AddComponent<BoxCollider>(
                true,
                DirectX::XMFLOAT3{ -0.3f, 0.65f, 0.6f },
                DirectX::XMFLOAT3{ 2.f, 1.3f, 1.f });
            (void)bc;
            auto& gs = go.AddComponent<GlassObject>(GlassGrade::Grade_1);
            (void)gs;
        });

    Prefab::Register(P::Gomibukuro,
        [](GameObject& go) {
            go.SetTag(Tag::Glass);

            auto& mf = go.AddComponent<MeshFilter>(ModelTag::Gomibukuro); mf.SetBoundsOffset({ 0.f, 0.7f, 0.f });    (void)mf;
            auto& mr = go.AddComponent<MeshRenderer>(); (void)mr;
            auto& bc = go.AddComponent<BoxCollider>(
                true,
                DirectX::XMFLOAT3{ 0, 0.8f, 0.f },
                DirectX::XMFLOAT3{ 1.1f, 1.6f, 1.1f });
            (void)bc;
            auto& gs = go.AddComponent<GlassObject>(GlassGrade::Grade_1);
            (void)gs;
        });

    Prefab::Register(P::Isu,
        [](GameObject& go) {
            go.SetTag(Tag::Glass);

            auto& mf = go.AddComponent<MeshFilter>(ModelTag::Isu); mf.SetBoundsOffset({ 0.f, 0.9f, 0.f });    (void)mf;
            auto& mr = go.AddComponent<MeshRenderer>(); (void)mr;
            auto& bc = go.AddComponent<BoxCollider>(
                true,
                DirectX::XMFLOAT3{ 0, 0.9f, 0.f },
                DirectX::XMFLOAT3{ 1.3f, 1.8f, 1.3f });
            (void)bc;
            auto& gs = go.AddComponent<GlassObject>(GlassGrade::Grade_1);
            (void)gs;
        });

    Prefab::Register(P::Kuukseizixyoukii,
        [](GameObject& go) {
            go.SetTag(Tag::Glass);

            auto& mf = go.AddComponent<MeshFilter>(ModelTag::Kuukseizixyoukii); mf.SetBoundsOffset({ 0.f, 1.0f, 0.f });    (void)mf;
            auto& mr = go.AddComponent<MeshRenderer>(); (void)mr;
            auto& bc = go.AddComponent<BoxCollider>(
                true,
                DirectX::XMFLOAT3{ 0.f, 1.0f, 0.f },
                DirectX::XMFLOAT3{ 1.0f, 2.0f, 0.8f });
            (void)bc;
            auto& gs = go.AddComponent<GlassObject>(GlassGrade::Grade_1);
            (void)gs;
        });

    Prefab::Register(P::Ningixyou,
        [](GameObject& go) {
            go.SetTag(Tag::Glass);

            auto& mf = go.AddComponent<MeshFilter>(ModelTag::Ningixyou); mf.SetBoundsOffset({ 0.f, 0.9f, 0.f });    (void)mf;
            auto& mr = go.AddComponent<MeshRenderer>(); (void)mr;
            auto& bc = go.AddComponent<BoxCollider>(
                true,
                DirectX::XMFLOAT3{ 0, 0.9f, 0.f },
                DirectX::XMFLOAT3{ 0.6f, 1.8f, 0.6f });
            (void)bc;
            auto& gs = go.AddComponent<GlassObject>(GlassGrade::Grade_1);
            (void)gs;
        });

    Prefab::Register(P::SagaraDoll,
        [](GameObject& go) {
            go.SetTag(Tag::Glass);

            auto& mf = go.AddComponent<MeshFilter>(ModelTag::SagaraDoll); mf.SetBoundsOffset({ 0.f, 0.9f, 0.f });    (void)mf;
            auto& mr = go.AddComponent<MeshRenderer>(); (void)mr;
            auto& bc = go.AddComponent<BoxCollider>(
                true,
                DirectX::XMFLOAT3{ 0, 0.9f, 0.f },
                DirectX::XMFLOAT3{ 0.6f, 1.8f, 0.6f });
            (void)bc;
            auto& gs = go.AddComponent<GlassObject>(GlassGrade::Grade_1);
            (void)gs;
        });

    Prefab::Register(P::Benti,
        [](GameObject& go) {
            go.SetTag(Tag::Glass);
            auto& mf = go.AddComponent<MeshFilter>(ModelTag::Benti);  mf.SetBoundsOffset({ 0.f, 1.1f, 0.f });     (void)mf;
            auto& mr = go.AddComponent<MeshRenderer>(); (void)mr;
            auto& bc = go.AddComponent<BoxCollider>(
                true,
                DirectX::XMFLOAT3{ 0.f, 1.f,  0.f },
                DirectX::XMFLOAT3{ 4.f, 2.f, 1.8f });
            (void)bc;
            auto& gs = go.AddComponent<GlassObject>(GlassGrade::Grade_2); (void)gs;
        });
    Prefab::Register(P::Aircon,
        [](GameObject& go) {
            go.SetTag(Tag::Glass);
            auto& mf = go.AddComponent<MeshFilter>(ModelTag::Aircon);  mf.SetBoundsOffset({ 0.f, 1.1f, 0.f });     (void)mf;
            auto& mr = go.AddComponent<MeshRenderer>(); (void)mr;
            auto& bc = go.AddComponent<BoxCollider>(
                true,
                DirectX::XMFLOAT3{ 0.f, 1.1f, 0.f },
                DirectX::XMFLOAT3{ 4.f, 2.2f, 1.6f });
            (void)bc;
            auto& gs = go.AddComponent<GlassObject>(GlassGrade::Grade_2); (void)gs;
        });
    Prefab::Register(P::Jihanki,
        [](GameObject& go) {
            go.SetTag(Tag::Glass);
            auto& mf = go.AddComponent<MeshFilter>(ModelTag::Jihanki);  mf.SetBoundsOffset({ 0.f, 1.8f, 0.f });     (void)mf;
            auto& mr = go.AddComponent<MeshRenderer>(); (void)mr;
            auto& bc = go.AddComponent<BoxCollider>(
                true,
                DirectX::XMFLOAT3{ 0.f, 1.8f, 0.f },
                DirectX::XMFLOAT3{ 1.8f, 3.6f, 1.4f });
            (void)bc;
            auto& gs = go.AddComponent<GlassObject>(GlassGrade::Grade_2); (void)gs;
        });
    Prefab::Register(P::Reizouko,
        [](GameObject& go) {
            go.SetTag(Tag::Glass);
            auto& mf = go.AddComponent<MeshFilter>(ModelTag::Reizouko);  mf.SetBoundsOffset({ 0.f, 1.6f, 0.f });     (void)mf;
            auto& mr = go.AddComponent<MeshRenderer>(); (void)mr;
            auto& bc = go.AddComponent<BoxCollider>(
                true,
                DirectX::XMFLOAT3{ 0.f, 1.5, 0.f },
                DirectX::XMFLOAT3{ 2.f, 3.f, 2.f });
            (void)bc;
            auto& gs = go.AddComponent<GlassObject>(GlassGrade::Grade_2); (void)gs;
        });


    Prefab::Register(P::Ufo,
        [](GameObject& go) {
            go.SetTag(Tag::Glass);
            auto& mf = go.AddComponent<MeshFilter>(ModelTag::Ufo); mf.SetBoundsOffset({ 0.f, 2.f, 0.f });     (void)mf;
            auto& mr = go.AddComponent<MeshRenderer>(); (void)mr;
            auto& bc = go.AddComponent<BoxCollider>(
                true,
                DirectX::XMFLOAT3{ 0.f, 1.5f, 0.f },
                DirectX::XMFLOAT3{ 5.4f, 3.f, 5.4f });
            (void)bc;
            auto& gs = go.AddComponent<GlassObject>(GlassGrade::Grade_3); (void)gs;
        });
    Prefab::Register(P::Eisei,
        [](GameObject& go) {
            go.SetTag(Tag::Glass);
            auto& mf = go.AddComponent<MeshFilter>(ModelTag::Eiseii); mf.SetBoundsOffset({ 0.f, 3.0f, 0.f });     (void)mf;
            auto& mr = go.AddComponent<MeshRenderer>(); (void)mr;
            auto& bc = go.AddComponent<BoxCollider>(
                true,
                DirectX::XMFLOAT3{ 0, 1.8f, 0 },
                DirectX::XMFLOAT3{ 12.f, 3.2f, 2.4f });
            (void)bc;
            auto& gs = go.AddComponent<GlassObject>(GlassGrade::Grade_3); (void)gs;
        });

    Prefab::Register(P::ProtPlay,
        [](GameObject& go) {
            go.SetTag(Tag::Glass);
            auto& mf = go.AddComponent<MeshFilter>(ModelTag::ProtPlay); mf.SetBoundsOffset({ -0.2f, 0.8f, 0.f });     (void)mf;
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
            auto& mf = go.AddComponent<MeshFilter>(PrimitiveMeshTag::Cube);                 (void)mf;
            auto& mr = go.AddComponent<MeshRenderer>();                                     (void)mr;
            //auto& bc = go.AddComponent<BoxCollider>(
            //    false,
            //    DirectX::XMFLOAT3{ 0.f, 0.5f, 0.f },
            //    DirectX::XMFLOAT3{ 0.6f, 1.0f, 0.2f });
            //bc.SetMask(ToBit(Layer::Ground));                                               (void)bc;
            auto& rb = go.AddComponent<Rigidbody>();                                        (void)rb;
            auto& shard = go.AddComponent<GlassShard>();                                    (void)shard;
        });

    Prefab::Register(P::StageWall,
        [](GameObject& go) {
            auto& mf = go.AddComponent<MeshFilter>(ModelTag::Fence); mf.SetBoundsOffset({ -0.2f, 0.8f, 0.f });     (void)mf;
            auto& mr = go.AddComponent<MeshRenderer>(); (void)mr;
            auto& bc = go.AddComponent<BoxCollider>(
                true,
                DirectX::XMFLOAT3{ 0.f, 3, 0 },
                DirectX::XMFLOAT3{ 9, 6, 1 });
            (void)bc;
            auto& gs = go.AddComponent<GlassObject>(GlassGrade::Grade_1); (void)gs;
     
        });
}
