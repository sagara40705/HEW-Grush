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
	outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Akican); });
	outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Desktop); });
	outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Gomibukuro); });
	outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Isu); });
	outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Kuukseizixyoukii); });
	outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Ningixyou); });

	outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Benti); });
	outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Aircon); });
	outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Jihanki); });
	outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Reizouko); });

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
	MeshFilter& mf = floor.AddComponent<MeshFilter>(PrimitiveMeshTag::Cube); mf.boundsRadius = 0.f;
	floor.AddComponent<MeshRenderer>();
	BoxCollider& collider = floor.AddComponent<BoxCollider>();
	collider.layer = Layer::Ground;

	//UFO
	Prefab::Instantiate(w, PrefabId::Ufo, Vector3(0.f, -0.5f, 20.f), Vector3::zero(), Vector3::one());

	//Grade_2
	Prefab::Instantiate(w, PrefabId::Benti, Vector3(-10.f, -0.9f, 10.f), Vector3::zero(), Vector3::one());
	Prefab::Instantiate(w, PrefabId::Aircon, Vector3(-5.f, -0.9f, 10.f), Vector3::zero(), Vector3::one());
	Prefab::Instantiate(w, PrefabId::Jihanki, Vector3(0.f, -0.9f, 10.f), Vector3::zero(), Vector3::one());
	Prefab::Instantiate(w, PrefabId::Reizouko, Vector3(5.f, -0.9f, 10.f), Vector3::zero(), Vector3::one());

	//Grade_1
	Prefab::Instantiate(w, PrefabId::Swing, Vector3(-10.f, -0.9f, 5.f), Vector3::zero(), Vector3::one());

	Prefab::Instantiate(w, PrefabId::Akican, Vector3(-8.f, -0.9f, 5.f), Vector3::zero(), Vector3::one());

	Prefab::Instantiate(w, PrefabId::Desktop, Vector3(-6.f, -0.9f, 5.f), Vector3::zero(), Vector3::one());

	Prefab::Instantiate(w, PrefabId::Gomibukuro, Vector3(-4.f, -0.9f, 5.f), Vector3::zero(), Vector3::one());

	Prefab::Instantiate(w, PrefabId::Isu, Vector3(-2.f, -0.9f, 5.f), Vector3::zero(), Vector3::one());

	Prefab::Instantiate(w, PrefabId::Kuukseizixyoukii, Vector3(-0.f, -0.9f, 5.f), Vector3::zero(), Vector3::one());

	Prefab::Instantiate(w, PrefabId::Ningixyou, Vector3(2.f, -0.9f, 5.f), Vector3::zero(), Vector3::one());


	for (int y = 0; y < 1; ++y){
		for (int x = 1; x < 51; ++x) {
			Prefab::Instantiate(w, PrefabId::Swing, Vector3(-50 + x * 1.5f, -0.9f + y * 1.5f, 50), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
			Prefab::Instantiate(w, PrefabId::Swing, Vector3(-50 + x * 1.5f, -0.9f + y * 1.5f, -50), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
			Prefab::Instantiate(w, PrefabId::Swing, Vector3( 50, -0.9f + y * 1.5f, -50 + x * 1.5f), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
			Prefab::Instantiate(w, PrefabId::Swing, Vector3(-50, -0.9f + y * 1.5f, -50 + x * 1.5f), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
		}
	}

	//プロト
	//Prefab::Instantiate(w, PrefabId::ProtPlay, Vector3(-12, -0.9f, 8), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
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


