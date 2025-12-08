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
	outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Hammer); });

	// テクスチャーロード
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Star); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::GaugeEmpty); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::GaugeMax); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::GaugeEffect); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::ScoreAtoKg); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::ScoreNumber); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::TimerColon); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::TimerNumber); });

	// プレイハブ設定
	outTasks.emplace_back([this] { RegisterAllPrefabs(); });

	// ゲームオブジェクト生成
	outTasks.emplace_back([this] { CreateManagers(); });
	outTasks.emplace_back([this] { CreateCameraAndPlayer(); });
	outTasks.emplace_back([this] { CreateStage(); });
	outTasks.emplace_back([this] { CreateUi(); });
	outTasks.emplace_back([this] { CreateGaugeUI(); });
	outTasks.emplace_back([this] { CreateTimerUI(); });
	outTasks.emplace_back([this] { CreateScoreUI(); });

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
	TextureManager::Load(TextureTag::ScoreAtoKg);
	TextureManager::Load(TextureTag::ScoreNumber);
	TextureManager::Load(TextureTag::TimerColon);
	TextureManager::Load(TextureTag::TimerNumber);
}

void SceneGame::CreateManagers()
{
	World& w = GetWorld();

	GameObject& gameManager = w.CreateGameObject();
	gameManager.SetTag(Tag::GameManager);

	GameObject& glassGaugeManager = w.CreateGameObject();
	glassGaugeManager.SetTag(Tag::GaugeManager);
	glassGaugeManager.AddComponent<GlassGaugeManager>();

	GameObject& timerManager = w.CreateGameObject();
	timerManager.SetTag(Tag::TimerUI);
}

void SceneGame::CreateCameraAndPlayer()
{
	World& w = GetWorld();

	GameObject& cameraPivot = w.CreateGameObject(Vector3::zero(), Vector3::zero(), Vector3::one());
	cameraPivot.SetTag(Tag::CamreraPivot);
	cameraPivot.AddComponent<CameraControl>();

	GameObject& mainCamera = cameraPivot.CreateChild(Camera::kInitPos, Camera::kInitRotate, Vector3::one());
	mainCamera.SetTag(Tag::MainCamera);
	mainCamera.AddComponent<Camera>();
	mainCamera.AddComponent<CameraShake>();

	GameObject& player = w.CreateGameObject(Vector3(-3.f, 2.f, 0.f), Vector3(0.f, 0.f, 0.f));
	player.SetTag(Tag::Player);
	player.AddComponent<MeshFilter>(ModelTag::Player);
	//player.AddComponent<MeshRenderer>();
	player.AddComponent<Rigidbody>();
	player.AddComponent<BoxCollider>(false, Vector3(0.f, 0.5f, 0.f));
	player.AddComponent<PlayerState>();
	player.AddComponent<PlayerMove>();
	player.AddComponent<PlayerAction>();

	GameObject& hummer = 
		player.CreateChild(Hammer::BasePos, Vector3::zero(), Vector3(0.3f, 0.3f, 0.3f));
	hummer.SetTag(Tag::Hammer);
	hummer.AddComponent<MeshFilter>(ModelTag::Hammer);
	//hummer.AddComponent<MeshRenderer>();
	hummer.AddComponent<BoxCollider>(false, Vector3(0.f, 1.f, 0.f));
	hummer.AddComponent<Hammer>();

	GameObject& glassShardPool = w.CreateGameObject();
	glassShardPool.AddComponent<GlassShardPool>();
}

void SceneGame::CreateStage()
{
	World& w = GetWorld();

	GameObject& floor = w.CreateGameObject(Vector3(0.f, -1.f, 0.f), Vector3::zero(), Vector3(100.f, 0.1f, 100.f));
	floor.AddComponent<MeshFilter>(PrimitiveMeshTag::Cube);
	floor.AddComponent<MeshRenderer>();
	BoxCollider& collider = floor.AddComponent<BoxCollider>();
	collider.layer = Layer::Ground;

	//UFO
	Prefab::Instantiate(w, PrefabId::Ufo, Vector3(10, -0.5f, 10), Vector3::zero(), Vector3::one());
	Prefab::Instantiate(w, PrefabId::Ufo, Vector3(-30, -0.5f, -30), Vector3::zero(), Vector3::one());
	Prefab::Instantiate(w, PrefabId::Ufo, Vector3(40, -0.5f, 10), Vector3::zero(), Vector3::one());

	//ベンチ
	Prefab::Instantiate(w, PrefabId::Benti, Vector3(-10, -0.9f, 10), Vector3::zero(), Vector3(1.2f, 1.2f, 1.2f));
	Prefab::Instantiate(w, PrefabId::Benti, Vector3(20, -0.9f, -16), Vector3::zero(), Vector3(1.2f, 1.2f, 1.2f));
	Prefab::Instantiate(w, PrefabId::Benti, Vector3(11, -0.9f, -13), Vector3::zero(), Vector3(1.2f, 1.2f, 1.2f));

	//すぃんぐ
	Prefab::Instantiate(w, PrefabId::Swing, Vector3(-7, -0.9f, -8), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
	Prefab::Instantiate(w, PrefabId::Swing, Vector3(-12, -0.9f, -2), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
	Prefab::Instantiate(w, PrefabId::Swing, Vector3(-22, -0.9f, -13), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
	Prefab::Instantiate(w, PrefabId::Swing, Vector3(10, -0.9f, 10), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
	Prefab::Instantiate(w, PrefabId::Swing, Vector3(-12, -0.9f, -22), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
	Prefab::Instantiate(w, PrefabId::Swing, Vector3(-22, -0.9f, -43), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));

	//プロト
	Prefab::Instantiate(w, PrefabId::ProtPlay, Vector3(-12, -0.9f, 8), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
	Prefab::Instantiate(w, PrefabId::ProtPlay, Vector3(5, -0.9f, 2), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
	Prefab::Instantiate(w, PrefabId::ProtPlay, Vector3(-20, -0.9f, -20), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
	Prefab::Instantiate(w, PrefabId::ProtPlay, Vector3(-16, -0.9f, -16), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
	Prefab::Instantiate(w, PrefabId::ProtPlay, Vector3(13, -0.9f, 2), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
	Prefab::Instantiate(w, PrefabId::ProtPlay, Vector3(22, -0.9f, 8), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
	Prefab::Instantiate(w, PrefabId::ProtPlay, Vector3(-17, -0.9f, 15), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
	Prefab::Instantiate(w, PrefabId::ProtPlay, Vector3(6, -0.9f, 17), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
	Prefab::Instantiate(w, PrefabId::ProtPlay, Vector3(20, -0.9f, -12), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
	Prefab::Instantiate(w, PrefabId::ProtPlay, Vector3(14, -0.9f, -19), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
}

void SceneGame::CreateUi()
{
	World& w = GetWorld();

	GameObject& canvas = w.CreateGameObject();
	canvas.AddComponent<Canvas>();
}

void SceneGame::CreateGaugeUI()
{
	World& w = GetWorld();

	GameObject& glassGaugeUI = w.CreateGameObject();
	glassGaugeUI.AddComponent<GlassGaugeUI>();
}

void SceneGame::CreateTimerUI()
{
	World& w = GetWorld();

	GameObject& timerUI = w.CreateGameObject();
	timerUI.SetTag(Tag::TimerUI);
	timerUI.AddComponent<TimerUI>();
}

void SceneGame::CreateScoreUI()
{
	World& w = GetWorld();

	GameObject& scoreManager = w.CreateGameObject();
	scoreManager.SetTag(Tag::ScoreManager);
	scoreManager.AddComponent<ScoreManager>();

	GameObject& scoreUI = w.CreateGameObject();
	scoreUI.AddComponent<ScoreUI>();
}

void SceneGame::Update()
{
}

void SceneGame::Draw()
{
}


