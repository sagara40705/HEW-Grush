#include "SceneGame.h"
#include "GameObject.h"
#include "AllComponent.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "Prefab.h"
#include "PrefabSet.h"
#include "LineManager.h"

SceneGame::SceneGame()
{
	
}

SceneGame::~SceneGame()
{
	
}
void SceneGame::Load()
{
	// 従来の同期ロードが必要なら、タスクを構築して全部回す
	std::vector<LoadTask> tasks;
	BuildLoadTasks(tasks);
	for (auto& t : tasks) t();
}

void SceneGame::Unload()
{

}

void SceneGame::BuildLoadTasks(std::vector<LoadTask>& outTasks)
{
	// World の中身を消す
	outTasks.emplace_back([this]
		{
		GetWorld().ClearAll();        // 既存の World API に合わせる
		});

	// ==== タスク設定 ====

	// モデルロード
	outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Karubi); });
	outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Swing); });
	outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Benti); });
	outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Ufo); });
	outTasks.emplace_back([this] { ModelManager::Load(ModelTag::ProtPlay); });
	outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Player); });

	// テクスチャーロード
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Star); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::GaugeEffect); });


	// プレイハブ設定
	outTasks.emplace_back([this] { RegisterAllPrefabs(); });

	// ゲームオブジェクト生成
	outTasks.emplace_back([this] { CreateCameraAndPlayer(); });
	outTasks.emplace_back([this] { CreateStage(); });
	outTasks.emplace_back([this] { CreateUi(); });

	// 最後に Start を流す
	outTasks.emplace_back([this] 
		{
		GetWorld().Start();
		});
}

void SceneGame::LoadModels()
{
	ModelManager::Load(ModelTag::Karubi);
	ModelManager::Load(ModelTag::Swing);
	ModelManager::Load(ModelTag::Benti);
	ModelManager::Load(ModelTag::Ufo);
	ModelManager::Load(ModelTag::ProtPlay);
	ModelManager::Load(ModelTag::Player);
}

void SceneGame::LoadTextures()
{
	TextureManager::Load(TextureTag::Star);
	TextureManager::Load(TextureTag::GaugeEffect);
}

void SceneGame::CreateCameraAndPlayer()
{
	World& w = GetWorld();

	GameObject& cameraPivot = w.CreateGameObject(Vector3::zero(), Vector3::zero(), Vector3::one());
	cameraPivot.SetTag(Tag::CamreraPivot);
	cameraPivot.AddComponent<CameraControl>();

	GameObject& mainCamera = cameraPivot.CreateChild(Camera::kInitPos, Camera::kInitRotate, Vector3::one());
	mainCamera.AddComponent<Camera>();

	GameObject& player = w.CreateGameObject(Vector3(-3.f, 2.f, 0.f), Vector3(0.f, 0.f, 0.f));
	player.SetTag(Tag::Player);
	player.AddComponent<MeshFilter>(ModelTag::Swing);
	player.AddComponent<MeshRenderer>();
	player.AddComponent<Rigidbody>();
	player.AddComponent<BoxCollider>();
	player.AddComponent<PlayerState>();
	player.AddComponent<PlayerMove>();
	player.AddComponent<PlayerAction>();
	player.AddComponent<Animator>();


	GameObject& hummer = 
		player.CreateChild(Vector3(0.f, 0.f, -2.f), Vector3::zero(), Vector3(0.3f, 0.3f, 0.3f));
	hummer.SetTag(Tag::Hammer);
	hummer.AddComponent<MeshFilter>(PrimitiveMeshTag::Cube);
	hummer.AddComponent<MeshRenderer>();
	hummer.AddComponent<BoxCollider>();
	hummer.AddComponent<Hammer>();
	//hummer.Destroy(1.0f);

	GameObject& glassGaugeManager = w.CreateGameObject();
	glassGaugeManager.AddComponent<GlassGaugeManager>();

	GameObject& sample = w.CreateGameObject(Vector3(3.f, 2.f, 3.f));
	sample.AddComponent<MeshFilter>(PrimitiveMeshTag::Cube);
	sample.AddComponent<MeshRenderer>();
	sample.AddComponent<BoxCollider>();
	sample.Destroy(1.0f);
}

void SceneGame::CreateStage()
{
	World& w = GetWorld();

	GameObject& floor = w.CreateGameObject(Vector3(0.f, -0.5f, 0.f), Vector3::zero(), Vector3(20.f, 0.1f, 20.f));
	floor.AddComponent<MeshFilter>(PrimitiveMeshTag::Cube);
	floor.AddComponent<MeshRenderer>();
	BoxCollider& collider = floor.AddComponent<BoxCollider>();
	collider.layer = Layer::Ground;
	//floor.Destroy(1.0f);

	Prefab::Instantiate(w, PrefabId::ProtPlay, Vector3(5.f, 0.f, 0.f), Vector3::zero(), Vector3::one());
}

void SceneGame::CreateUi()
{
	World& w = GetWorld();

	GameObject& canvas = w.CreateGameObject();
	canvas.AddComponent<Canvas>();

	GameObject& sampleUi = w.CreateGameObject();
	sampleUi.AddComponent<UiSpriteRenderer>(TextureTag::GaugeEffect, Vector2(0.f, 0.f) , Vector2(320.f, 270.f));
	sampleUi.AddComponent<SpriteAnimation>(8, 4, 3, 32, true);

	GameObject& gaugeUi = w.CreateGameObject();
	GameObject& gaugeFrameUi = w.CreateGameObject();
	GameObject& gaugeEffectUi = w.CreateGameObject();
}

void SceneGame::Update()
{
}

void SceneGame::Draw()
{
}


