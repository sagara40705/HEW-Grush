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
	outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Eiseii); });

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

	GameObject& cameraPivot = w.CreateGameObject(Vector3::zero(), Vector3(0.f, 180.f, 0.f), Vector3::one());
	cameraPivot.SetTag(Tag::CamreraPivot);
	cameraPivot.AddComponent<CameraControl>();

	GameObject& mainCamera = cameraPivot.CreateChild(Camera::kInitPos, Camera::kInitRotate, Vector3::one());
	mainCamera.SetTag(Tag::MainCamera);
	mainCamera.AddComponent<Camera>();
	mainCamera.AddComponent<CameraShake>();

	GameObject& player = w.CreateGameObject(Vector3(-40.f, 2.f, 43.f), Vector3(0.f, 0.f, 0.f));
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

	//////Grade_3
	//Prefab::Instantiate(w, PrefabId::Ufo, Vector3(-5.f, -0.5f, 20.f), Vector3::zero(), Vector3::one());
	//Prefab::Instantiate(w, PrefabId::Eisei, Vector3(5.f, -0.5f, 20.f), Vector3::zero(), Vector3::one());

	////Grade_2
	//Prefab::Instantiate(w, PrefabId::Benti, Vector3(-10.f, -0.9f, 10.f), Vector3::zero(), Vector3::one());
	//Prefab::Instantiate(w, PrefabId::Aircon, Vector3(-5.f, -0.9f, 10.f), Vector3::zero(), Vector3::one());
	//Prefab::Instantiate(w, PrefabId::Jihanki, Vector3(0.f, -0.9f, 10.f), Vector3::zero(), Vector3::one());
	//Prefab::Instantiate(w, PrefabId::Reizouko, Vector3(5.f, -0.9f, 10.f), Vector3::zero(), Vector3::one());

	//Grade_1
	/*Prefab::Instantiate(w, PrefabId::Swing, Vector3(-10.f, -0.9f, 5.f), Vector3::zero(), Vector3::one());
	Prefab::Instantiate(w, PrefabId::Akican, Vector3(-8.f, -0.9f, 5.f), Vector3::zero(), Vector3::one());
	Prefab::Instantiate(w, PrefabId::Desktop, Vector3(-6.f, -0.9f, 5.f), Vector3::zero(), Vector3::one());
	Prefab::Instantiate(w, PrefabId::Gomibukuro, Vector3(-4.f, -0.9f, 5.f), Vector3::zero(), Vector3::one());
	Prefab::Instantiate(w, PrefabId::Isu, Vector3(-2.f, -0.9f, 5.f), Vector3::zero(), Vector3::one());
	Prefab::Instantiate(w, PrefabId::Kuukseizixyoukii, Vector3(-0.f, -0.9f, 5.f), Vector3::zero(), Vector3::one());
	Prefab::Instantiate(w, PrefabId::Ningixyou, Vector3(2.f, -0.9f, 5.f), Vector3::zero(), Vector3::one());
	Prefab::Instantiate(w, PrefabId::Swing, Vector3(-10.f, -0.9f, 5.f), Vector3::zero(), Vector3::one());
	Prefab::Instantiate(w, PrefabId::Akican, Vector3(-8.f, -0.9f, 5.f), Vector3::zero(), Vector3::one());
	Prefab::Instantiate(w, PrefabId::Desktop, Vector3(-6.f, -0.9f, 5.f), Vector3::zero(), Vector3::one());
	Prefab::Instantiate(w, PrefabId::Gomibukuro, Vector3(-4.f, -0.9f, 5.f), Vector3::zero(), Vector3::one());
	Prefab::Instantiate(w, PrefabId::Isu, Vector3(-2.f, -0.9f, 5.f), Vector3::zero(), Vector3::one());
	Prefab::Instantiate(w, PrefabId::Kuukseizixyoukii, Vector3(-0.f, -0.9f, 5.f), Vector3::zero(), Vector3::one());
	Prefab::Instantiate(w, PrefabId::Ningixyou, Vector3(2.f, -0.9f, 5.f), Vector3::zero(), Vector3::one());*/
     
	//プロト
	//Prefab::Instantiate(w, PrefabId::ProtPlay, Vector3(-12, -0.9f, 8), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));

	// 座標用構造体（共通）
	struct Pos { float x; float y; float z;};
	struct Rot { float x, y, z; };

	//--------------------------------------
	// Grade 1
	//--------------------------------------
	PrefabId prefabOrder1[] = {
		PrefabId::Swing,
		PrefabId::Akican,
		PrefabId::Desktop,
		PrefabId::Gomibukuro,
		PrefabId::Isu,
		PrefabId::Kuukseizixyoukii,
		PrefabId::Ningixyou
	};

	Pos positionsGrade_1[] = {
	{ 19, -0.754f,  48 }, { 46, -0.650f,  26 }, { 36, -0.754f,  35 },
	{ 11, -0.754f,  33 }, { -4, -0.754f,  24 }, { 20, -0.400f, -37 },
	{ -9, -0.754f, -27 }, { 14, -0.754f, -43 }, { -38, -0.400f,  -7 },
	{ 21, -0.900f, -49 }, { -30, -0.650f,  33 }, { -28, -0.400f,   6 },
	{ 34, -0.400f,   8 }, {  8, -0.754f, -47 }, { -28, -0.900f,  29 },
	{ 43, -0.400f,  40 }, { -3, -0.650f, -31 }, { -47, -0.754f, -13 },
	{ -15, -0.754f, -29 }, { -20, -0.400f, -29 }, { 16, -0.900f, -44 },
	{ 42, -0.900f, -14 }, { 26, -0.400f,  28 }, { 30, -0.650f,   5 },
	{ -19, -0.900f, 46 }, { -34, -0.754f, -41 }, { -4, -0.400f,  19 },
	{ 27, -0.754f, -24 }, { 19, -0.754f, -14 }, { 26, -0.754f, -17 },
	{ 26, -0.754f, -34 }, { 47, -0.650f, -36 }, { 31, -0.650f,  29 },
	{ -22, -0.400f,   8 }, { 33, -0.650f,  22 }, { -13, -0.900f, -40 },
	{ -10, -0.754f,  41 }, { -41, -0.400f,   3 }, { 35, -0.754f, -13 },
	{ -13, -0.754f,  -9 }, { 22, -0.400f, -29 }, { 12, -0.400f,   7 },
	{ -48, -0.400f, -3 }, { 22, -0.754f,  17 }, { -18, -0.650f,   6 },
	{ -22, -0.650f, -11 }, { -14, -0.754f, 19 }, { 10, -0.754f, 28 },
	{ 16, -0.400f, 16 }, { -12, -0.754f, 25 }, { -28, -0.400f, -46 },
	{ 24, -0.400f, 33 }, { 41, -0.650f, -33 }, { 42, -0.400f, -49 },
	{ -25, -0.400f, -37 }, { 18, -0.400f, 43 }, { 17, -0.400f, -48 },
	{ -49, -0.650f, -25 }, { -31, -0.400f, 26 }, { 36, -0.650f, -21 },
	{ -44, -0.754f, -11 }, { 25, -0.400f, -39 }, { -8, -0.754f, 48 },
	{ 47, -0.900f, 20 }, { 7, -0.400f, -43 }, { -44, -0.400f, -31 },
	{ 48, -0.400f, -8 }, { -45, -0.754f, 41 }, { 47, -0.650f, -16 },
	{ -7, -0.650f, -46 }, { -42, -0.900f, 6 }, { -29, -0.900f, 44 },
	{ -46, -0.754f, 43 }, { 48, -0.650f, 8 }, { -45, -0.754f, 20 },
	{ -50, -0.400f, -15 }, { 42, -0.400f, 16 }, { -41, -0.400f, 46 },
	{ -32, -0.650f, 39 }, { -14, -0.754f, -45 }, { 41, -0.754f, -39 },
	{ -12, -0.900f, 46 }, { -2, -0.400f, -49 }, { -44, -0.650f, -27 },
	{ -48, -0.754f, 11 }, { 44, -0.754f, 6 }, { 44, -0.400f, 24 },
	{ 39, -0.400f, -35 }, { -49, -0.754f, -32 }, { -9, -0.754f, -46 },
	{ -47, -0.754f, -47 }, { 34, -0.650f, 27 }, { -36, -0.754f, -44 },
	{ -43, -0.754f,  -6 }, { -19, -0.754f, 43 }, { -44, -0.400f, -15 },
	{ 3, -0.754f, 42 }, { 46, -0.754f, -6 }, { 27, -0.400f, -36 },
	{ -6, -0.900f, -43 }, { 29, -0.650f, 44 }, { -33, -0.400f, -31 },
	{ 24, -0.400f, 48 }, { -47, -0.754f, 28 }, { -43, -0.900f, -33 },
	{ 42, -0.400f, 37 }, { 38, -0.650f, -12 }, { 38, -0.754f, -23 },
	{ 35, -0.754f, 23 }, { -10, -0.400f, 44 }, { 37, -0.900f, 46 },
	{ 35, -0.900f, 46 }, { 29, -0.400f, 48 }, { -41, -0.650f, 39 },
	{ -24, -0.900f, -45 }, { 23, -0.754f, -48 }, { -41, -0.400f, -37 },
	{ 43, -0.754f, -7 }, { 39, -0.754f, 22 }, { -47, -0.754f, 6 },
	{ 46, -0.754f, -38 }, { 44, -0.650f, -48 }, { -12, -0.650f, 40 },
	{ 46, -0.400f, 29 }, { 38, -0.650f, 40 }, { -38, -0.900f, 25 },
	{ -46, -0.754f, -49 }, { 42, -0.400f, -24 }, { 33, -0.754f, -31 },
	{ 30, -0.754f, -34 }, { -15, -0.400f, 48 }, { 46, -0.400f, -13 },
	{ 33, -0.400f, -44 }, { 14, -0.754f, 48 }, { 20, -0.650f, -41 },
	{ -45, -0.650f, -42 }, { -32, -0.754f, -34 }, { 40, -0.754f, -23 },
	{ -39, -0.400f, -35 }, { -33, -0.754f, -25 }, { 17, -0.400f, 46 },
	{ -3, -0.400f, 48 }, { -42, -0.650f, -26 }, { -2, -0.400f, -43 },
	{ -42, -0.400f, -44 }, { -25, -0.400f, 42 }, {  5, -0.400f, 40 },
	{ -6, -0.650f, 45 }, { 41, -0.400f, -44 }, { 39, -0.650f, 18 },
	{ 44, -0.754f, -5 }, { 39, -0.400f, 32 }, { 34, -0.754f, -46 },
	{ 42, -0.900f, -42 }, { 13, -0.400f, -47 }, { 41, -0.400f, -27 },
	{ -44, -0.400f, 6 }, { 23, -0.754f, -36 }, { -49, -0.650f, -12 },
	{ 2, -0.650f, -42 }, { -46, -0.900f, 34 }, { 45, -0.900f, -17 },
	{ -36, -0.754f, -30 }, { 3, -0.650f, 48 }, { -47, -0.754f, -27 },
	{ 31, -0.400f, -42 }, { -25, -0.400f, 40 }, { -40, -0.400f, -45 },
	{ -17, -0.650f, 42 }, { 46, -0.754f, -42 }, { -49, -0.754f, 47 },
	{ -34, -0.900f, 46 }, { 23, -0.400f, -42 }, { -22, -0.650f, 41 },
	{ -44, -0.754f, 37 }, { -50, -0.754f, -9 }, { -1, -0.400f, 46 },
	{ -42, -0.400f, 22 }, { 37, -0.754f, 43 }, { 22, -0.754f, 33 },
	{ -50, -0.754f,  9 }, { -50, -0.650f, -37 }, { -22, -0.754f, 37 },
	{ 38, -0.754f, 37 }, { -31, -0.754f, 28 }, { 46, -0.400f, -45 },
	{ -48, -0.754f, 32 }, { 37, -0.754f, -50 }, { -49, -0.400f, 20 },
	{ 48, -0.900f, -21 }, { 32, -0.650f, 42 }, { 34, -0.400f, -23 },
	{ 30, -0.400f, 39 }, { 39, -0.754f, -21 }, { -45, -0.900f, -7 },
	{ -47, -0.400f, -35 }, { -15, -0.650f, -40 }, { 45, -0.754f, -34 },
	{ 30, -0.754f, 42 }, { -39, -0.400f, 39 }
	};

	Rot rotationsGrade_1[] = {
	{45, 90, 180}, {45, 135, 45}, {45, 45, 180}, {180, 0, 135}, {45, 45, 0},
	{180, 180, 90}, {180, 90, 135}, {45, 135, 0}, {180, 0, 90}, {0, 90, 0},
	{135, 45, 45}, {90, 135, 45}, {90, 180, 90}, {180, 0, 45}, {180, 45, 180},
	{45, 90, 90}, {45, 135, 45}, {45, 180, 0}, {135, 0, 0}, {90, 90, 180},
	{180, 45, 0}, {180, 135, 0}, {45, 180, 90}, {45, 0, 45}, {0, 90, 0},
	{135, 45, 0}, {90, 135, 90}, {135, 180, 0}, {45, 0, 0}, {180, 90, 135},
	{0, 45, 45}, {135, 135, 45}, {135, 180, 135}, {90, 0, 180}, {135, 90, 135},
	{180, 45, 180}, {45, 135, 180}, {45, 180, 90}, {45, 0, 180}, {135, 90, 180},
	{90, 45, 45}, {90, 135, 0}, {90, 180, 180}, {0, 0, 45}, {45, 90, 135},
	{45, 45, 135}, {180, 135, 45}, {180, 180, 45}, {90, 0, 135}, {180, 90, 135},
	{90, 45, 45}, {90, 135, 0}, {135, 180, 135}, {90, 0, 0}, {45, 90, 90},
	{0, 45, 90}, {135, 135, 90}, {45, 180, 135}, {180, 0, 90}, {135, 90, 45},
	{0, 45, 135}, {90, 135, 45}, {45, 180, 0}, {0, 0, 0}, {90, 90, 45},
	{0, 45, 90}, {90, 135, 90}, {180, 180, 135}, {135, 0, 45}, {135, 90, 135},
	{180, 45, 180}, {0, 135, 180}, {180, 180, 45}, {135, 0, 45}, {180, 90, 135},
	{90, 45, 135}, {135, 135, 90}, {90, 180, 0}, {45, 0, 135}, {180, 90, 45},
	{180, 45, 135}, {0, 135, 0}, {180, 180, 90}, {45, 0, 135}, {0, 90, 135},
	{0, 45, 45}, {90, 135, 45}, {0, 180, 90}, {0, 0, 45}, {180, 90, 135}
	{ 45, 90, 180 }, {45, 135, 45}, {45, 45, 180}, {180, 0, 135}, {45, 45, 0},
	{180, 180, 90}, {180, 90, 135}, {45, 135, 0}, {180, 0, 90}, {0, 90, 0},
	{135, 45, 45}, {90, 135, 45}, {90, 180, 90}, {180, 0, 45}, {180, 45, 180},
	{45, 90, 90}, {45, 135, 45}, {45, 180, 0}, {135, 0, 0}, {90, 90, 180},
	{180, 45, 0}, {180, 135, 0}, {45, 180, 90}, {45, 0, 45}, {0, 90, 0},
	{135, 45, 0}, {90, 135, 90}, {135, 180, 0}, {45, 0, 0}, {180, 90, 135},
	{0, 45, 45}, {135, 135, 45}, {135, 180, 135}, {90, 0, 180}, {135, 90, 135},
	{180, 45, 180}, {45, 135, 180}, {45, 180, 90}, {45, 0, 180}, {135, 90, 180},
	{90, 45, 45}, {90, 135, 0}, {90, 180, 180}, {0, 0, 45}, {45, 90, 135},
	{45, 45, 135}, {180, 135, 45}, {180, 180, 45}, {90, 0, 135}, {180, 90, 135},
	{90, 45, 45}, {90, 135, 0}, {135, 180, 135}, {90, 0, 0}, {45, 90, 90},
	{0, 45, 90}, {135, 135, 90}, {45, 180, 135}, {180, 0, 90}, {135, 90, 45},
	{0, 45, 135}, {90, 135, 45}, {45, 180, 0}, {0, 0, 0}, {90, 90, 45},
	{0, 45, 90}, {90, 135, 90}, {180, 180, 135}, {135, 0, 45}, {135, 90, 135},
	{180, 45, 180}, {0, 135, 180}, {180, 180, 45}, {135, 0, 45}, {180, 90, 135},
	{90, 45, 135}, {135, 135, 90}, {90, 180, 0}, {45, 0, 135}, {180, 90, 45},
	{180, 45, 135}, {0, 135, 0}, {180, 180, 90}, {45, 0, 135}, {0, 90, 135},
	{0, 45, 45}, {90, 135, 45}, {0, 180, 90}, {0, 0, 45}, {180, 90, 135},
	{ 45, 90, 180 }, {45, 135, 45}, {45, 45, 180}, {180, 0, 135}, {45, 45, 0},
	{180, 180, 90}, {180, 90, 135}, {45, 135, 0}, {180, 0, 90}, {0, 90, 0},
	{135, 45, 45}, {90, 135, 45}, {90, 180, 90}, {180, 0, 45}, {180, 45, 180},
	{0, 45, 90}, {135, 135, 90}, {45, 180, 135}, {180, 0, 90}, {135, 90, 45},
	{135, 45, 0}, {90, 135, 90}, {135, 180, 0}
	};

	int prefabCount1 = sizeof(prefabOrder1) / sizeof(prefabOrder1[0]);
int posCount1 = sizeof(positionsGrade_1) / sizeof(positionsGrade_1[0]);
int rotCount1 = sizeof(rotationsGrade_1) / sizeof(rotationsGrade_1[0]);

for (int i = 0; i < posCount1; i++) {
	PrefabId id = prefabOrder1[i % prefabCount1];
	Rot rot = rotationsGrade_1[i % rotCount1];
	Prefab::Instantiate(
		w,
		id,
		Vector3(positionsGrade_1[i].x, positionsGrade_1[i].y, positionsGrade_1[i].z),
		Vector3(rot.x, rot.y, rot.z),
		Vector3::one()
	);
}



	//--------------------------------------
	// Grade 2
	//--------------------------------------
	PrefabId prefabOrder2[] = {
		PrefabId::Benti,
		PrefabId::Aircon,
		PrefabId::Jihanki,
		PrefabId::Reizouko,
	};

	Pos positionsGrade_2[] = {
		{-4, -0.4f, -31}, {11, -0.65f, 7}, {-18, -0.9f, 5}, {-16, -0.4f, -30}, {-31, -0.754f, 12}, {-38, -0.754f, 2}, {-12, -0.4f, 24},
	{12, -0.4f, -6}, {-14, -0.9f, 18}, {-29, -0.4f, -26}, {11, -0.4f, 15}, {-36, -0.754f, 9}, {17, -0.754f, -31}, {-21, -0.754f, 19},
	{-22, -0.754f, -11}, {-39, -0.4f, -25}, {5, -0.754f, 22}, {-2, -0.4f, 4}, {-39, -0.4f, 18}, {-7, -0.9f, 32}, {33, -0.4f, 8},
	{16, -0.4f, 15}, {-13, -0.4f, 8}, {3, -0.4f, -33}, {-4, -0.4f, 18}, {8, -0.65f, -16}, {-38, -0.754f, -8}, {38, -0.4f, 9},
	{25, -0.754f, -18}, {-14, -0.9f, -10}, {-10, -0.4f, -21}, {-20, -0.4f, -22}, {-13, -0.754f, 34}, {-9, -0.4f, -28}, {19, -0.4f, -45},
	{-44, -0.65f, 15}, {-35, -0.754f, -6}, {42, -0.4f, 40}, {-32, -0.9f, -5}, {35, -0.4f, -13}, {29, -0.9f, -28}, {26, -0.754f, -34},
	{-34, -0.754f, -41}, {39, -0.754f, 24}, {-44, -0.9f, -40}, {20, -0.4f, -50}, {-29, -0.65f, 1}, {-37, -0.4f, 27}, {26, -0.754f, 28},
	{19, -0.9f, -37}, {-41, -0.65f, 2}, {39, -0.754f, -3}, {-18, -0.4f, 32}, {-25, -0.754f, 20}, {-14, -0.754f, 31}, {-13, -0.65f, -41},
	{-32, -0.4f, -46}, {-20, -0.4f, -30}, {30, -0.4f, 4}, {26, -0.65f, -24}, {-19, -0.4f, -43}, {-29, -0.4f, 5}, {41, -0.65f, 8},
	{45, -0.9f, -20}, {-50, -0.4f, 0}, {-13, -0.754f, -36}, {17, -0.65f, 32}, {30, -0.9f, -32}, {-45, -0.9f, 23}, {-49, -0.754f, -4},
	{-4, -0.65f, -46}, {45, -0.4f, 26}, {38, -0.9f, 28}, {19, -0.754f, 47}, {-19, -0.65f, 45}, {9, -0.9f, 27}, {35, -0.754f, 35},
	{22, -0.65f, 16}, {47, -0.4f, -36}, {18, -0.754f, -14}, {42, -0.754f, -14}, {32, -0.4f, 22}, {30, -0.4f, 28}, {-44, -0.754f, -37},
	{10, -0.754f, 33}, {22, -0.754f, -30}, {-2, -0.4f, -38}, {-45, -0.65f, -45}, {-9, -0.9f, 35}, {-31, -0.9f, 32}, {-15, -0.4f, -24},
	{-1, -0.754f, -30}, {-10, -0.4f, 41}, {37, -0.65f, -43}, {-4, -0.9f, 24}, {-28, -0.9f, 29}, {-47, -0.4f, -13}, {42, -0.754f, -3},
	{23, -0.754f, 39}, {41, -0.4f, 5}, {16, -0.754f, -44}, {37, -0.754f, -31}, {13, -0.754f, -44}, {-19, -0.9f, 9},
	{7, -0.4f, -47}, {18, -0.4f, -10}, {-23, -0.65f, 7}, {-26, -0.754f, 14}, {23, -0.4f, 23}, {10, -0.754f, 36},
	};

	Rot rotationsGrade_2[] = {
		{45, 90, 180}, {45, 135, 45}, {45, 45, 180}, {180, 0, 135}, {45, 45, 0},
	{180, 180, 90}, {180, 90, 135}, {45, 135, 0}, {180, 0, 90}, {0, 90, 0},
	{135, 45, 45}, {90, 135, 45}, {90, 180, 90}, {180, 0, 45}, {180, 45, 180},
	{45, 90, 90}, {45, 135, 45}, {45, 180, 0}, {135, 0, 0}, {90, 90, 180},
	{180, 45, 0}, {180, 135, 0}, {45, 180, 90}, {45, 0, 45}, {0, 90, 0},
	{135, 45, 0}, {90, 135, 90}, {135, 180, 0}, {45, 0, 0}, {180, 90, 135},
	{0, 45, 45}, {135, 135, 45}, {135, 180, 135}, {90, 0, 180}, {135, 90, 135},
	{180, 45, 180}, {45, 135, 180}, {45, 180, 90}, {45, 0, 180}, {135, 90, 180},
	{90, 45, 45}, {90, 135, 0}, {90, 180, 180}, {0, 0, 45}, {45, 90, 135},
	{45, 45, 135}, {180, 135, 45}, {180, 180, 45}, {90, 0, 135}, {180, 90, 135},
	{90, 45, 45}, {90, 135, 0}, {135, 180, 135}, {90, 0, 0}, {45, 90, 90},
	{0, 45, 90}, {135, 135, 90}, {45, 180, 135}, {180, 0, 90}, {135, 90, 45},
	{0, 45, 135}, {90, 135, 45}, {45, 180, 0}, {0, 0, 0}, {90, 90, 45},
	{0, 45, 90}, {90, 135, 90}, {180, 180, 135}, {135, 0, 45}, {135, 90, 135},
	{180, 45, 180}, {0, 135, 180}, {180, 180, 45}, {135, 0, 45}, {180, 90, 135},
	{90, 45, 135}, {135, 135, 90}, {90, 180, 0}, {45, 0, 135}, {180, 90, 45},
	{180, 45, 135}, {0, 135, 0}, {180, 180, 90}, {45, 0, 135}, {0, 90, 135},
	{0, 45, 45}, {90, 135, 45}, {0, 180, 90}, {0, 0, 45}, {180, 90, 135}
	{ 45, 90, 180 }, {45, 135, 45}, {45, 45, 180}, {180, 0, 135}, {45, 45, 0},
	{180, 180, 90}, {180, 90, 135}, {45, 135, 0}, {180, 0, 90}, {0, 90, 0},
	{135, 45, 45}, {90, 135, 45}, {90, 180, 90}, {180, 0, 45}, {180, 45, 180},
	{45, 90, 90}, {45, 135, 45}, {45, 180, 0}, {135, 0, 0}, {90, 90, 180},
	{180, 45, 0}, {180, 135, 0}, {45, 180, 90}, {45, 0, 45}, {0, 90, 0},
	{135, 45, 0}, {90, 135, 90}, {135, 180, 0}, {45, 0, 0}, {180, 90, 135},

	};

	int prefabCount2 = sizeof(prefabOrder2) / sizeof(prefabOrder2[0]);
	int posCount2 = sizeof(positionsGrade_2) / sizeof(positionsGrade_2[0]);
	int rotCount2 = sizeof(rotationsGrade_2) / sizeof(rotationsGrade_2[0]);

	for (int i = 0; i < posCount2; i++) {
		PrefabId id = prefabOrder2[i % prefabCount2];
		Rot rot = rotationsGrade_2[i % rotCount2];
		Prefab::Instantiate(
			w,
			id,
			Vector3(positionsGrade_2[i].x, positionsGrade_2[i].y, positionsGrade_2[i].z),
			Vector3(rot.x, rot.y, rot.z),
			Vector3::one()
		);
	}



	////--------------------------------------
	// //Grade 3
	////--------------------------------------
	PrefabId prefabOrder3[] = {
		PrefabId::Ufo,
		PrefabId::Eisei,
	};

	Pos positionsGrade_3[] = {
		 {-40, -0.754f, -25}, {21, -0.9f, 0}, {20, -0.4f, 8}, {-32, -0.4f, 11}, {7, -0.9f, 41}, {-6, -0.65f, 17}, {-24, -0.4f, -13},
	{ 36, -0.4f, 9 }, { 28, -0.4f, -4 }, { -10, -0.754f, -40 }, { -15, -0.754f, -10 }, { -31, -0.4f, -28 }, { -12, -0.4f, 22 }, { -38, -0.4f, 1 },
	{ 17, -0.9f, -20 }, { 2, -0.754f, -34 }, { -22, -0.4f, 17 }, { 10, -0.754f, 14 }, { -6, -0.65f, -31 }, { -40, -0.4f, -10 }, { 5, -0.65f, 20 },
	{ -8, -0.4f, 30 }, { -38, -0.9f, 7 }, { -15, -0.9f, 7 }, { -15, -0.4f, 17 }, { 15, -0.9f, 36 }, { -42, -0.65f, -17 }, { -17, -0.754f, -32 },
	{ 23, -0.9f, -18 }, { 6, -0.754f, -16 }, { 16, -0.4f, -31 }, { 16, -0.754f, 15 }, { -39, -0.4f, 17 }, { 11, -0.4f, -6 }, { -2, -0.754f, 4 },
	{ 31, -0.9f, 8 }, { -20, -0.754f, 4 }, { -3, -0.4f, 36 }, { 10, -0.9f, 6 }, { 29, -0.4f, -12 },
	};

	Rot rotationsGrade_3[] = {
		{45, 90, 180}, {45, 135, 45}, {45, 45, 180}, {180, 0, 135}, {45, 45, 0},
	{ 180, 180, 90 }, { 180, 90, 135 }, { 45, 135, 0 }, { 180, 0, 90 }, { 0, 90, 0 },
	{ 135, 45, 45 }, { 90, 135, 45 }, { 90, 180, 90 }, { 180, 0, 45 }, { 180, 45, 180 },
	{ 45, 90, 90 }, { 45, 135, 45 }, { 45, 180, 0 }, { 135, 0, 0 }, { 90, 90, 180 },
	{ 180, 45, 0 }, { 180, 135, 0 }, { 45, 180, 90 }, { 45, 0, 45 }, { 0, 90, 0 },
	{ 135, 45, 0 }, { 90, 135, 90 }, { 135, 180, 0 }, { 45, 0, 0 }, { 180, 90, 135 },
	{ 0, 45, 45 }, { 135, 135, 45 }, { 135, 180, 135 }, { 90, 0, 180 }, { 135, 90, 135 },
	{ 180, 45, 180 }, { 45, 135, 180 }, { 45, 180, 90 }, { 45, 0, 180 }, { 135, 90, 180 },
	{ 90, 45, 45 }, { 90, 135, 0 }, { 90, 180, 180 }, { 0, 0, 45 }, { 45, 90, 135 },
	{ 45, 45, 135 }, { 180, 135, 45 }, { 180, 180, 45 }, { 90, 0, 135 }, { 180, 90, 135 },
	{ 90, 45, 45 }, { 90, 135, 0 }, { 135, 180, 135 }, { 90, 0, 0 }, { 45, 90, 90 },
	{ 0, 45, 90 }, { 135, 135, 90 }, { 45, 180, 135 }, { 180, 0, 90 }, { 135, 90, 45 },
	{ 0, 45, 135 }, { 90, 135, 45 }, { 45, 180, 0 }, { 0, 0, 0 }, { 90, 90, 45 },
	{ 0, 45, 90 }, { 90, 135, 90 }, { 180, 180, 135 }, { 135, 0, 45 }, { 135, 90, 135 },
	{ 180, 45, 180 }, { 0, 135, 180 }, { 180, 180, 45 }, { 135, 0, 45 }, { 180, 90, 135 },
	{ 90, 45, 135 }, { 135, 135, 90 }, { 90, 180, 0 }, { 45, 0, 135 }, { 180, 90, 45 },
	{ 180, 45, 135 }, { 0, 135, 0 }, { 180, 180, 90 }, { 45, 0, 135 }, { 0, 90, 135 },
	{ 0, 45, 45 }, { 90, 135, 45 }, { 0, 180, 90 }, { 0, 0, 45 }, { 180, 90, 135 }
	{ 45, 90, 180 }, { 45, 135, 45 }, { 45, 45, 180 }, { 180, 0, 135 }, { 45, 45, 0 },
	{ 180, 180, 90 }, { 180, 90, 135 }, { 45, 135, 0 }, { 180, 0, 90 }, { 0, 90, 0 },
	{ 135, 45, 45 }, { 90, 135, 45 }, { 90, 180, 90 }, { 180, 0, 45 }, { 180, 45, 180 },
	{ 45, 90, 90 }, { 45, 135, 45 }, { 45, 180, 0 }, { 135, 0, 0 }, { 90, 90, 180 },
	{ 180, 45, 0 }, { 180, 135, 0 }, { 45, 180, 90 }, { 45, 0, 45 }, { 0, 90, 0 },
	{ 135, 45, 0 }, { 90, 135, 90 }, { 135, 180, 0 }, { 45, 0, 0 }, { 180, 90, 135 },
	};

	int prefabCount3 = sizeof(prefabOrder3) / sizeof(prefabOrder3[0]);
	int posCount3 = sizeof(positionsGrade_3) / sizeof(positionsGrade_3[0]);
	int rotCount3 = sizeof(rotationsGrade_3) / sizeof(rotationsGrade_3[0]);

	for (int i = 0; i < posCount3; i++) {
		PrefabId id = prefabOrder3[i % prefabCount3];
		Rot rot = rotationsGrade_3[i % rotCount3];
		Prefab::Instantiate(
			w,
			id,
			Vector3(positionsGrade_3[i].x, positionsGrade_3[i].y, positionsGrade_3[i].z),
			Vector3(rot.x, rot.y, rot.z),
			Vector3::one()
		);
	}

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


