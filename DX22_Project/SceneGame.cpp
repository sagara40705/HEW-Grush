#include "SceneGame.h"
#include <DirectXMath.h>
#include "Defines.h"
#include "World.h"
#include "AllComponet.h"
#include "PhysicsSystem.h"
#include "SampleMapData.h"
#include "Model.h"
#include "LineManager.h"
#include "RenderSystem.h"

World W;

namespace
{
    // ブロック共通処理
    GameObject& SpawnCubeBlock(const DirectX::XMFLOAT3& pos,
        const DirectX::XMFLOAT3& scale = Vec3One,
        bool isStaticCollider = true)
    {
        GameObject& o = W.CreateGameObject();
        o.AddComponent<Transform>(pos, Vec3Zero, scale);
        o.AddComponent<MeshFilter>(MeshFilter::Type::Cube);
        o.AddComponent<MeshRender>();
        o.AddComponent<BoxCollider>();
        o.SetTag(Tag::Glass);
        return o;
    }

    // 三次元マップ(0/1)からブロック群を生成
    template<size_t Y, size_t Z, size_t X>
    void BuildBlocksFromMap(const int(&map)[Y][Z][X],
        float unit = kUnit,
        const DirectX::XMFLOAT3& scale = Vec3One,
        bool isStaticCollider = true)
    {
        for (size_t y = 0; y < Y; ++y)
        {
            for (size_t z = 0; z < Z; ++z)
            {
                for (size_t x = 0; x < X; ++x)
                {
                    if (map[y][z][x] == 0) continue;

                    DirectX::XMFLOAT3 pos
                    {
                        static_cast<float>(x) * unit,
                        static_cast<float>(y) * unit,
                        static_cast<float>(z) * unit
                    };
                    SpawnCubeBlock(pos, scale, isStaticCollider);
                }
            }
        }
    }
}

SceneGame::SceneGame()
{
	Debug::Init();
    LineManager::Init();

	GameObject& camera = W.CreateGameObject();	
	camera.AddComponent<Transform>(Camera::initPos(), Camera::initRotate(), Vector3::zero());
	camera.AddComponent<Camera>();	

	GameObject& player = W.CreateGameObject();
	player.AddComponent<Transform>(Vector3(0, 2, 0), Vector3::zero(), Vector3(0.5, 0.5, 0.5));
	player.AddComponent<MeshFilter>(MeshFilter::Type::Cylinder);
	player.AddComponent<MeshRender>(1);
    player.AddComponent<BoxCollider>(true, Vector3(0, 0, 0), Vector3(1,1,1));
   // player.AddComponent<SphereCollider>(0.5f, false);
   // player.AddComponent<CapsuleCollider>(0.01f, 0.5f, false);
	player.AddComponent<Rigidbody>();
	player.AddComponent<PlayerControll>();
	camera.AddComponent<CameraFollow>(&player.GetComponent<Transform>());
    player.SetTag(Tag::Player);
   // LineManager::AddBoxCollider(&player);

    //GameObject& hammer = W.CreateGameObject();
    //hammer.AddComponent<Transform>(Vector3(1, 0, 0), Vector3::zero(), Vector3(0.8, 0.4, 0.4));
    //hammer.TryGetComponent<Transform>()->SetParent(&player.GetComponent<Transform>());
    //hammer.AddComponent<MeshFilter>(MeshFilter::Type::Cube);
    //hammer.AddComponent<MeshRender>(1);
    //auto& hbc = hammer.AddComponent<BoxCollider>(false); hbc.isTrigger = true;
    //hammer.AddComponent<HummerScript>(player.TryGetComponent<Transform>());
    //hammer.SetTag(Tag::Weapon);
    
    //GameObject& floor = W.CreateGameObject();
    //floor.AddComponent<Transform>(Vector3(0, -30, 0), Vector3::zero(), Vector3(50, 50, 50));
    //floor.AddComponent<MeshFilter>(MeshFilter::Type::Sphere);
    //floor.AddComponent<MeshRender>();
    //floor.AddComponent<SphereCollider>();
    ////floor.AddComponent<PlanetMove>();
    //floor.SetTag(Tag::Ground);

    BuildBlocksFromMap(map, /*unit=*/kUnit, /*scale=*/blockScale, /*isStaticCollider=*/true);

    GameObject& karubi = W.CreateGameObject();
    karubi.AddComponent<Transform>(DirectX::XMFLOAT3{ 2,0.5,2 }, Vec3Zero, Vec3One);
    karubi.AddComponent<ModelComponent>("Assets/まけん式赤見かるび姫衣装MMD/まけん式赤見かるび姫衣装ver1.0.pmx", 0.08f, Model::ZFlip);
    karubi.AddComponent<ModelRenderer>();
    karubi.AddComponent<BoxCollider>(true, DirectX::XMFLOAT3{ 0,0.75f,0 }, DirectX::XMFLOAT3{ 1, 1.5, 1 });
    LineManager::AddBoxCollider(&karubi);

    GameObject& SampleUi = W.CreateGameObject();
    auto& ui = SampleUi.AddComponent<CanvasUI>("Assets/Texture/Star.png", Vector2(0, 0));
    ui.SetColor({ 0.5f ,0.5f ,0.5f , 1.f});

	W.Start();
    ui.SetOffset(Vector2(0, 60));
    ui.SetSize(Vector2(1, 0.7f));
    ui.SetUVPos(Vector2(0, 0.3f));
    ui.SetUVScale(Vector2(1, 0.7f));
}
SceneGame::~SceneGame()
{
	W.ClearAll();

}
void SceneGame::Update()
{
	W.Update();
}
void SceneGame::Draw()
{
	W.Draw();


}


