#include "SceneGame.h"
#include "GameObject.h"
#include "AllComponent.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "Prefab.h"
#include "PrefabSet.h"
#include "LineManager.h"
#include "GlassObjectSpawner.h"
#include "GameState.h"
#include "DebugLog.h"


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
	outTasks.emplace_back([this] { ModelManager::Load(ModelTag::SagaraDoll); });

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
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::GroundTile); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Stage); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::SkyDome); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::GaugeEmpty); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::GaugeMax); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::GaugeEffect); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::ScoreAtoKg); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::ScoreNumber); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::TimerColon); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::TimerNumber); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Spin); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::SpinMono); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::SpinMoji); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::SpinMojiMono); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::SpinFlash); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::SpinFront); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Dash); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::DashMono); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::DashMoji); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::DashMojiMono); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::UIFrostNoise); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::EffectLv1); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::GlassShard); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::TasksUI); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::TutorialLocation); });

	// プレイハブ設定
	outTasks.emplace_back([this] { RegisterAllPrefabs(); });

	// ゲームオブジェクト生成
	outTasks.emplace_back([this] { CreateManagers(); });
	outTasks.emplace_back([this] { CreateCameraAndPlayer(); });
	outTasks.emplace_back([this] { CreateStage(); });

	// UI
	outTasks.emplace_back([this] { CreateUi(); });
	outTasks.emplace_back([this] { CreateGaugeUI(); });
	outTasks.emplace_back([this] { CreateTimerUI(); });
	outTasks.emplace_back([this] { CreateScoreUI(); });
	outTasks.emplace_back([this] { CreateActionUI(); });

	//ギミック
	outTasks.emplace_back([this] { CreateGlassObjectSpawner(); });
	outTasks.emplace_back([this] { CreateTutorialWall(); });

	//outTasks.emplace_back([this] { InitPlayerTex(); });
	outTasks.emplace_back([this] { InitSkyDome(); });


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
}

void SceneGame::CreateCameraAndPlayer()
{
	World& w = GetWorld();

	//70×70 ①
	GameObject& cameraPivot = w.CreateGameObject(Vector3::zero(), Vector3(0.f, 180.f, 0.f), Vector3::one());

	cameraPivot.SetTag(Tag::CamreraPivot);
	cameraPivot.AddComponent<CameraControl>();

	GameObject& mainCamera = cameraPivot.CreateChild(Camera::kInitPos, Camera::kInitRotate, Vector3::one());
	mainCamera.SetTag(Tag::MainCamera);
	mainCamera.AddComponent<Camera>();
	mainCamera.AddComponent<CameraShake>();

	//70×70 ①
	GameObject& player = w.CreateGameObject(Vector3(0.f, 2.f, 0.f), Vector3(0.f, 0.f, 0.f));
	 
	player.SetTag(Tag::Player);
	player.AddComponent<MeshFilter>(ModelTag::Player);
	player.AddComponent<MeshRenderer>();
	player.AddComponent<Rigidbody>();
	player.AddComponent<BoxCollider>(false, Vector3(0.f, 0.5f, 0.f));
	player.AddComponent<PlayerState>();
	player.AddComponent<PlayerMove>();
	player.AddComponent<PlayerAction>();
	player.AddComponent<Animator>();
	player.AddComponent<PlayerAnimation>();

	GameObject& hummer = player.CreateChild(Hammer::BasePos, Hammer::BaseRota, Vector3(0.2f, 0.2f, 0.2f));
	hummer.SetTag(Tag::Hammer);
	hummer.AddComponent<MeshFilter>(ModelTag::Hammer);
	hummer.AddComponent<MeshRenderer>();
	hummer.AddComponent<BoxCollider>(false, Vector3(-3.f, 2.f, -3.f), Vector3(5.f, 5.f, 2.f));
	hummer.AddComponent<Hammer>();

	GameObject& glassShardPool = w.CreateGameObject();
	glassShardPool.AddComponent<GlassShardPool>();
}

void SceneGame::CreateStage()
{
	StageId stage = GetCurrentStage();

	// デバック用
	stage = StageId::Stage1;

	CreateStageFromCSV("CSV/StageData_Grade1.csv");
	CreateStageFromCSV("CSV/StageData_Grade2.csv");
	CreateStageFromCSV("CSV/StageData_Grade3.csv");

	switch (stage)
	{
	case StageId::Stage1:	CreateStage1();
		break;
	case StageId::Stage2:	CreateStage2();
		break;
	case StageId::Stage3:	CreateStage3();
		break;
	default:
		assert(false && "不正なステージID");
		break;
	}
}

void SceneGame::CreateStage1()
{
	World& w = GetWorld();

	//80端
	constexpr float SIZE = 80.f;
	constexpr float HALF = SIZE / 2.f;
	constexpr float OFFSET = HALF + 0.5f;
	
	//80フィールド
	GameObject& floor = w.CreateGameObject(Vector3(0.f, -1.f, 0.f), Vector3::zero(), Vector3(80.f, 0.1f, 80.f));
	MeshFilter& mf = floor.AddComponent<MeshFilter>(PrimitiveMeshTag::Plane); mf.boundsRadius = 0.f;
	floor.AddComponent<GroundRenderer>();
	BoxCollider& collider = floor.AddComponent<BoxCollider>();
	collider.layer = Layer::Ground;


	//壁
	GameObject& wT = w.CreateGameObject(Vector3(0.f, 1.f, OFFSET), Vector3(90.f, 0.f, 0.f), Vector3(SIZE, 1.f, 4.f));
	MeshFilter& mfT = wT.AddComponent<MeshFilter>(PrimitiveMeshTag::Cube); mfT.boundsRadius = 0.f;
	wT.AddComponent<GroundRenderer>();
	BoxCollider& bcT = wT.AddComponent<BoxCollider>(); bcT.layer = Layer::Ground;
	wT.AddComponent<StageWall>();

	GameObject& wB = w.CreateGameObject(Vector3(0.f, 1.f, -OFFSET), Vector3(90.f, 0.f, 0.f), Vector3(SIZE, 1.f, 4.0f));
	MeshFilter& mfB = wB.AddComponent<MeshFilter>(PrimitiveMeshTag::Cube); mfB.boundsRadius = 0.f;
	wB.AddComponent<GroundRenderer>();
	BoxCollider& bcB = wB.AddComponent<BoxCollider>(); bcB.layer = Layer::Ground;
	wB.AddComponent<StageWall>();

	GameObject& wR = w.CreateGameObject(Vector3(OFFSET, 1.f, 0.f), Vector3(0.f, 0.f, 90.f), Vector3(4.0f, 1.f, SIZE));
	MeshFilter& mfR = wR.AddComponent<MeshFilter>(PrimitiveMeshTag::Cube); mfR.boundsRadius = 0.f;
	wR.AddComponent<GroundRenderer>();
	BoxCollider& bcR = wR.AddComponent<BoxCollider>(); bcR.layer = Layer::Ground;
	wR.AddComponent<StageWall>();

	GameObject& wL = w.CreateGameObject(Vector3(-OFFSET, 1.f, 0.f), Vector3(0.f, 0.f, 90.f), Vector3(4.0f, 1.f, SIZE));
	MeshFilter& mfL = wL.AddComponent<MeshFilter>(PrimitiveMeshTag::Cube); mfL.boundsRadius = 0.f;
	wL.AddComponent<GroundRenderer>();
	BoxCollider& bcL = wL.AddComponent<BoxCollider>(); bcL.layer = Layer::Ground;
	wL.AddComponent<StageWall>();

	return;

	////Grade_3
	//Prefab::Instantiate(w, PrefabId::Ufo, Vector3(-5.f, -0.5f, 20.f), Vector3::zero(), Vector3::one());
	//Prefab::Instantiate(w, PrefabId::Eisei, Vector3(5.f, -0.5f, 20.f), Vector3::zero(), Vector3::one());

	//Grade_2
	//Prefab::Instantiate(w, PrefabId::Benti, Vector3(-10.f, -0.9f, 10.f), Vector3::zero(), Vector3::one());
	//Prefab::Instantiate(w, PrefabId::Aircon, Vector3(-5.f, -0.9f, 10.f), Vector3::zero(), Vector3::one());
	//Prefab::Instantiate(w, PrefabId::Jihanki, Vector3(0.f, -0.9f, 10.f), Vector3::zero(), Vector3::one());
	//Prefab::Instantiate(w, PrefabId::Reizouko, Vector3(5.f, -0.9f, 10.f), Vector3::zero(), Vector3::one());

	//Grade_1
	//Prefab::Instantiate(w, PrefabId::Swing, Vector3(-10.f, -0.9f, 5.f), Vector3::zero(), Vector3::one());
	//Prefab::Instantiate(w, PrefabId::Akican, Vector3(-8.f, -0.9f, 5.f), Vector3::zero(), Vector3::one());
	//Prefab::Instantiate(w, PrefabId::Desktop, Vector3(-6.f, -0.9f, 5.f), Vector3::zero(), Vector3::one());
	//Prefab::Instantiate(w, PrefabId::Gomibukuro, Vector3(-4.f, -0.9f, 5.f), Vector3::zero(), Vector3::one());
	//Prefab::Instantiate(w, PrefabId::Isu, Vector3(-2.f, -0.9f, 5.f), Vector3::zero(), Vector3::one());
	//Prefab::Instantiate(w, PrefabId::Kuukseizixyoukii, Vector3(-0.f, -0.9f, 5.f), Vector3::zero(), Vector3::one());
	//Prefab::Instantiate(w, PrefabId::Ningixyou, Vector3(2.f, -0.9f, 5.f), Vector3::zero(), Vector3::one());
	//Prefab::Instantiate(w, PrefabId::Swing, Vector3(-10.f, -0.9f, 5.f), Vector3::zero(), Vector3::one());
	//Prefab::Instantiate(w, PrefabId::Akican, Vector3(-8.f, -0.9f, 5.f), Vector3::zero(), Vector3::one());
	//Prefab::Instantiate(w, PrefabId::Desktop, Vector3(-6.f, -0.9f, 5.f), Vector3::zero(), Vector3::one());
	//Prefab::Instantiate(w, PrefabId::Gomibukuro, Vector3(-4.f, -0.9f, 5.f), Vector3::zero(), Vector3::one());
	//Prefab::Instantiate(w, PrefabId::Isu, Vector3(-2.f, -0.9f, 5.f), Vector3::zero(), Vector3::one());
	//Prefab::Instantiate(w, PrefabId::Kuukseizixyoukii, Vector3(-0.f, -0.9f, 5.f), Vector3::zero(), Vector3::one());
	//Prefab::Instantiate(w, PrefabId::Ningixyou, Vector3(2.f, -0.9f, 5.f), Vector3::zero(), Vector3::one());
     
	//プロト
	//Prefab::Instantiate(w, PrefabId::ProtPlay, Vector3(-12, -0.9f, 8), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));

	// 座標用構造体（共通）

	struct Pos { float x;  float y; float z;};
	struct Rot { float x, y, z; };



	//ステージ1

	//--------------------------------------
   // Grade 1
   //--------------------------------------

	const Pos positionsGrade_1[] = {
    { -8.f,-0.9f,-11.f},{ 6.f,-0.6f,-12.f},{ 34.f,-0.6f, -9.f},{  5.f,-0.5f,-14.f},{ 11.f,-0.5f, -3.f},{-3.f,-1.2f, 13.f},{20.f,-1.1f, 39.f},
	{-10.f,-1.1f , 7.f},{ 5.f,-0.9f, 11.f},{  1.f,-0.9f,-13.f},{  3.f,-1.1f, 14.f},{ 11.f,-1.1f, 14.f},{ 3.f,-1.2f,  7.f},{-7.f,-1.2f, -5.f},
	{  8.f,-1.1f,  4.f},{ 6.f,-1.1f,  9.f},{  2.f,-0.9f, 11.f},{  8.f,-0.9f,  0.f},{  1.f,-1.1f,  9.f},{ 3.f,-1.1f, 12.f},{ 8.f,-1.2f,  9.f},
	{-12.f,-1.2f,  4.f},{ 2.f,-1.1f,-13.f},{ -8.f,-1.1f,-13.f},{  7.f,-0.9f,  1.f},{ -2.f,-0.9f, 14.f},{-6.f,-1.1f,-27.f},{12.f,-1.1f, -5.f},
	{ 10.f,-1.2f,  2.f},{ 1.f,-1.2f, 12.f},{-12.f,-1.1f,  9.f},{  6.f,-1.1f,  7.f},{  5.f,-0.9f, -8.f},{ 5.f,-0.9f,-26.f},{ 3.f,-1.1f,-14.f}, 
	{ 14.f,-1.1f,  0.f},{-9.f,-1.2f,  5.f},{  2.f,-1.2f,  9.f},{ -4.f,-1.1f, -6.f},{ -8.f,-1.1f, -1.f},{14.f,-0.9f, -1.f},{ 7.f,-0.9f, -8.f}, 
	{  6.f,-1.1f, 10.f},{-9.f,-1.1f,  4.f},{  6.f,-1.2f, -7.f},{ -6.f,-1.2f, 12.f},{-11.f,-1.1f,  4.f},{-6.f,-1.1f, 13.f},{ 7.f,-0.9f, -5.f},
	{ -5.f,-0.9f, -7.f},{ 2.f,-1.1f,-15.f},{ 12.f,-1.1f, -4.f},{ -7.f,-1.2f, -2.f},{-30.f,-1.2f,-25.f},{-6.f,-1.1f,  6.f},{-3.f,-1.1f,-10.f},
	{  0.f,-0.9f,-11.f},{-2.f,-0.9f,  6.f},{  7.f,-1.1f,-11.f},{-11.f,-1.1f, -7.f}
    };

	
	const Rot rotationsGrade_1[] = {
	{ 0.f,0.f,0.f},{-90.f,0.f,0.f},{90.f,0.f, 0.f},{0.f,0.f,-90.f},{ 0.f,0.f,90.f},{-15.f,0.f,-15.f},{30.f,0.f,30.f},{-30.f,0.f,-30.f},
	{45.f,0.f,0.f},{-45.f,0.f,0.f},{ 0.f,0.f,45.f},{0.f,0.f,-45.f},{15.f,0.f,15.f},{-15.f,0.f,-15.f},{30.f,0.f,30.f},{-30.f,0.f,-30.f},
	{45.f,0.f,0.f},{-45.f,0.f,0.f},{ 0.f,0.f,45.f},{0.f,0.f,-45.f},{15.f,0.f,15.f},{-15.f,0.f,-15.f},{30.f,0.f,30.f},{-30.f,0.f,-30.f},
	{45.f,0.f,0.f},{-45.f,0.f,0.f},{ 0.f,0.f,45.f},{0.f,0.f,-45.f},{15.f,0.f,15.f},{-15.f,0.f,-15.f},{30.f,0.f,30.f},{-30.f,0.f,-30.f},
	{45.f,0.f,0.f},{-45.f,0.f,0.f},{ 0.f,0.f,45.f},{0.f,0.f,-45.f},{15.f,0.f,15.f},{-15.f,0.f,-15.f},{30.f,0.f,30.f},{-30.f,0.f,-30.f},
	{45.f,0.f,0.f},{-45.f,0.f,0.f},{ 0.f,0.f,45.f},{0.f,0.f,-45.f},{15.f,0.f,15.f},{-15.f,0.f,-15.f},{30.f,0.f,30.f},{-30.f,0.f,-30.f},
	{45.f,0.f,0.f},{-45.f,0.f,0.f},{ 0.f,0.f,45.f},{0.f,0.f,-45.f},{15.f,0.f,15.f},{-15.f,0.f,-15.f},{30.f,0.f,30.f},{-30.f,0.f,-30.f},
	{45.f,0.f,0.f},{-45.f,0.f,0.f},{ 0.f,0.f,45.f},{0.f,0.f,-45.f}
	};


	//--------------------------------------
	// Grade 2
	//--------------------------------------

	const Pos positionsGrade_2[] = {

	{ -8.f,-0.9f, 17.f},{-21.f,-0.4f,  0.f},{-27.f,-0.4f,  8.f},{ -9.f,-0.4f,-21.f},{-17.f,-0.4f,-21.f},{ 12.f,-1.0f,-16.f},{-27.f,-1.0f, 15.f},
	{  7.f,-0.9f,-15.f},{-10.f,-0.5f,-25.f},{ 14.f,-0.5f, -7.f},{  0.f,-0.5f,-16.f},{ 11.f,-0.5f,  9.f},{ 18.f,-1.4f, -6.f},{ 15.f,-1.4f,-13.f},
	{ -6.f,-0.9f,-33.f},{ 16.f,-0.5f,  3.f},{-15.f,-0.5f,-15.f},{  6.f,-0.5f, 18.f},{-22.f,-0.5f,-11.f},{ 10.f,-1.4f,-18.f},{ 21.f,-1.4f,-18.f},
	{ 15.f,-0.9f,-11.f},{ -5.f,-0.5f,-18.f},{ 25.f,-0.5f,  3.f},{  6.f,-0.5f, 22.f},{ -6.f,-0.5f,-24.f},{  8.f,-1.4f, 22.f},{ 19.f,-1.4f,  9.f},
	{ 33.f,-0.9f,-17.f},{-25.f,-0.5f,  8.f},{-12.f,-0.5f, 26.f},{-29.f,-0.5f,-16.f},{  3.f,-0.5f, 25.f},{-16.f,-1.4f,  6.f},{ 21.f,-1.4f, 15.f},
	{-12.f,-0.9f,-13.f},{-14.f,-0.5f,-18.f},{-25.f,-0.5f, -8.f},{-28.f,-0.5f, -1.f},{-27.f,-0.5f,  2.f},{ 13.f,-1.4f, -9.f},{-20.f,-1.4f,-10.f},
	{-20.f,-0.9f, -2.f},{ 24.f,-0.5f, -7.f},{  1.f,-0.5f,-21.f},{-21.f,-0.5f, 18.f},{-20.f,-0.5f,  9.f},{ 14.f,-1.4f,-22.f},{-17.f,-1.4f, 18.f},
	{-10.f,-0.9f, 16.f}
	};

	const Rot rotationsGrade_2[] = {
	{ 0.f,0.f,0.f}, {90.f,0.f,0.f}, {  0.f, 0.f, 90.f}, {90.f,0.f,45.f}, {  45.f, 0.f, 90.f},{  0.f,0.f,10.f}, {  0.f,0.f,-10.f},
	{ 0.f,0.f,0.f}, {90.f,0.f,0.f}, {  0.f, 0.f, 90.f}, {90.f,0.f,45.f}, {  45.f, 0.f, 90.f},{  0.f,0.f,10.f}, {  0.f,0.f,-10.f},
	{ 0.f,0.f,0.f}, {90.f,0.f,0.f}, {  0.f, 0.f, 90.f}, {90.f,0.f,45.f}, {  45.f, 0.f, 90.f},{  0.f,0.f,10.f}, {  0.f,0.f,-10.f},
	{ 0.f,0.f,0.f}, {90.f,0.f,0.f}, {  0.f, 0.f, 90.f}, {90.f,0.f,45.f}, {  45.f, 0.f, 90.f},{  0.f,0.f,10.f}, {  0.f,0.f,-10.f},
	{ 0.f,0.f,0.f}, {90.f,0.f,0.f}, {  0.f, 0.f, 90.f}, {90.f,0.f,45.f}, {  45.f, 0.f, 90.f},{  0.f,0.f,10.f}, {  0.f,0.f,-10.f},
	{ 0.f,0.f,0.f}, {90.f,0.f,0.f}, {  0.f, 0.f, 90.f}, {90.f,0.f,45.f}, {  45.f, 0.f, 90.f},{  0.f,0.f,10.f}, {  0.f,0.f,-10.f},
	{ 0.f,0.f,0.f}, {90.f,0.f,0.f}, {  0.f, 0.f, 90.f}, {90.f,0.f,45.f}, {  45.f, 0.f, 90.f},{  0.f,0.f,10.f}, {  0.f,0.f,-10.f},
	{ 0.f,0.f,0.f}
	};

	//--------------------------------------
	// Grade 3
	//--------------------------------------

	const Pos positionsGrade_3[] = {
	{-29.f, 2.f, -25.f}, { 28.f, 2.f, -23.f}, { 12.f, -0.9f, -32.f}, { 17.f ,3.f, 20.f}, { -3.f, 3.f,  36.f},
	{-33.f, 1.f, -38.f}, { 22.f, 1.f, -21.f}, { 19.f, -0.9f, -30.f}, {-28.f, 2.f, 17.f}, {-33.f, 2.f,  -5.f},
	{ 30.f, 1.f,  11.f}, {  9.f, 1.f,  -8.f}, { 27.f, -0.9f,  17.f}, { -8.f, 2.f, 37.f}, { 15.f, 2.f,  33.f},
	{ 21.f, 1.f,  29.f}, {-31.f, 1.f,   9.f}, {  1.f, -0.9f,  27.f}, { 31.f, 2.f, 21.f}, {-25.f, 2.f, -21.f},
	{ 28.f, 1.f, -13.f}, { 30.f, 1.f, -18.f}, { 36.f, -0.9f,  -2.f}, {-15.f, 2.f, 34.f}, { 23.f, 2.f,  21.f},
	{ 17.f, 1.f,  15.f}, {  6.f, 1.f,  27.f}, {-32.f, -0.9f, -10.f}, { 23.f, 2.f, 24.f}, { 29.f, 2.f,   6.f},
	{ -5.f, 1.f, -36.f}, { 32.f, 1.f,   3.f},{ -17.f, -0.9f,  25.f}, { 33.f, 2.f, 11.f}, {-26.f, 2.f, -17.f}
	};

	const Rot rotationsGrade_3[] = {
	{125.f,  0.f, 0.f}, {0.f,  0.f, 125.f}, {0.f,  0.f, 0.f}, {0.f,  0.f, 150.f}, {150.f,  0.f, 0.f},
    {125.f,  0.f, 0.f}, {0.f,  0.f, 125.f}, {0.f,  0.f, 0.f}, {0.f,  0.f, 150.f}, {150.f,  0.f, 0.f},
	{125.f,  0.f, 0.f}, {0.f,  0.f, 125.f}, {0.f,  0.f, 0.f}, {0.f,  0.f, 150.f}, {150.f,  0.f, 0.f},
	{125.f,  0.f, 0.f}, {0.f,  0.f, 125.f}, {0.f,  0.f, 0.f}, {0.f,  0.f, 150.f}, {150.f,  0.f, 0.f},
	{125.f,  0.f, 0.f}, {0.f,  0.f, 125.f}, {0.f,  0.f, 0.f}, {0.f,  0.f, 150.f}, {150.f,  0.f, 0.f},
	{125.f,  0.f, 0.f}, {0.f,  0.f, 125.f}, {0.f,  0.f, 0.f}, {0.f,  0.f, 150.f}, {150.f,  0.f, 0.f},
	{125.f,  0.f, 0.f}, { 0.f, 0.f, 125.f}, {0.f,  0.f, 0.f}, {0.f,  0.f, 150.f}, {150.f,  0.f, 0.f}
	};

PrefabId prefabOrder1[] = {
		PrefabId::Swing,
		PrefabId::Akican,
		PrefabId::Desktop,
		PrefabId::Gomibukuro,
		PrefabId::Isu,
		PrefabId::Kuukseizixyoukii,
		PrefabId::Ningixyou
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

	//--------------------------------------
	// Grade 3
	//--------------------------------------
	PrefabId prefabOrder3[] = {
		PrefabId::Ufo,
		PrefabId::Eisei,
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

void SceneGame::CreateStage2()
{
	World& w = GetWorld();

	//80 * 50 フィールド1
	GameObject& floor1 = w.CreateGameObject(Vector3(0.f, -1.f, 0.f), Vector3::zero(), Vector3(80.f, 0.1f, 50.f));
	MeshFilter& mf1 = floor1.AddComponent<MeshFilter>(PrimitiveMeshTag::Plane); mf1.boundsRadius = 0.f;
	floor1.AddComponent<GroundRenderer>();
	BoxCollider& collider1 = floor1.AddComponent<BoxCollider>();
	collider1.layer = Layer::Ground;

	//80 * 50 フィールド1
	GameObject& floor2 = w.CreateGameObject(Vector3(90.f, -1.f, 0.f), Vector3::zero(), Vector3(80.f, 0.1f, 50.f));
	MeshFilter& mf2 = floor2.AddComponent<MeshFilter>(PrimitiveMeshTag::Plane); mf2.boundsRadius = 0.f;
	floor2.AddComponent<GroundRenderer>();
	BoxCollider& collider2 = floor2.AddComponent<BoxCollider>();
	collider2.layer = Layer::Ground;

	CreateWarp();
}

void SceneGame::CreateStage3()
{
	World& w = GetWorld();

	//80 * 50 フィールド
	GameObject& floor1 = w.CreateGameObject(Vector3(0.f, -1.f, 0.f), Vector3::zero(), Vector3(80.f, 0.1f, 50.f));
	MeshFilter& mf1 = floor1.AddComponent<MeshFilter>(PrimitiveMeshTag::Plane); mf1.boundsRadius = 0.f;
	floor1.AddComponent<GroundRenderer>();
	BoxCollider& collider1 = floor1.AddComponent<BoxCollider>();
	collider1.layer = Layer::Ground;

	//50 * 50 フィールド1
	GameObject& floor2 = w.CreateGameObject(Vector3(0.f, -1.f, -60.f), Vector3::zero(), Vector3(50.f, 0.1f, 50.f));
	MeshFilter& mf2 = floor2.AddComponent<MeshFilter>(PrimitiveMeshTag::Plane); mf2.boundsRadius = 0.f;
	floor2.AddComponent<GroundRenderer>();
	BoxCollider& collider2 = floor2.AddComponent<BoxCollider>();
	collider2.layer = Layer::Ground;

	//50 * 50 フィールド2
	GameObject& floor3 = w.CreateGameObject(Vector3(0.f, -1.f, 60.f), Vector3::zero(), Vector3(50.f, 0.1f, 50.f));
	MeshFilter& mf3 = floor3.AddComponent<MeshFilter>(PrimitiveMeshTag::Plane); mf3.boundsRadius = 0.f;
	floor3.AddComponent<GroundRenderer>();
	BoxCollider& collider3 = floor3.AddComponent<BoxCollider>();
	collider3.layer = Layer::Ground;
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

void SceneGame::CreateActionUI()
{
	World& w = GetWorld();

	GameObject& actionManager = w.CreateGameObject();
	actionManager.SetTag(Tag::ActionUI);
	actionManager.AddComponent<ActionUI>();
}

void SceneGame::CreateTutorialWall()
{
	if (IsEndTutorial()) return;

	World& w = GetWorld();

	GameObject& wall_01 = w.CreateGameObject(Vector3(0.f, 0.f, 5.f), Vector3::zero(), Vector3(5.f, 1.f, 1.f));
	wall_01.AddComponent<BoxCollider>();
	wall_01.AddComponent<MeshFilter>(PrimitiveMeshTag::Cube);
	wall_01.AddComponent<MeshRenderer>();
}

void SceneGame::CreateGlassObjectSpawner()
{
	World& w = GetWorld();

	GameObject& glassObjectSpawner = w.CreateGameObject();
	//glassObjectSpawner.AddComponent<GlassObjectSpawner>();

	GameObject& Frost = w.CreateGameObject(Vector3(0.f, -0.9f, 0.f), Vector3(90.f, 0.f, 0.f));
	Frost.AddComponent<Sprite3DRenderer>(TextureTag::UIFrostNoise, Vector2::zero(), Vector2(12.8f, 7.2f));

	if (IsEndTutorial()) return;
	GameObject& tutorial = w.CreateGameObject();
	tutorial.AddComponent<TutorialManager>();
}

void SceneGame::CreateWarp()
{
	World& w = GetWorld();

	GameObject& warpA = w.CreateGameObject(Vector3(37.f, 0.f, 0.f), Vector3::zero(), Vector3::one());
	warpA.AddComponent< MeshFilter > (PrimitiveMeshTag::Sphere);
	warpA.AddComponent<MeshRenderer>();
	BoxCollider& bcA = warpA.AddComponent<BoxCollider>(true); bcA.isTrigger = true;
	warpA.AddComponent<WarpPoint>(Point::AtoB);

	GameObject& warpB = w.CreateGameObject(Vector3(53.f, 0.f, 0.f), Vector3::zero(), Vector3::one());
	warpB.AddComponent< MeshFilter >(PrimitiveMeshTag::Sphere);
	warpB.AddComponent<MeshRenderer>();
	BoxCollider& bcB = warpB.AddComponent<BoxCollider>(true); bcB.isTrigger = true;
	warpB.AddComponent<WarpPoint>(Point::BtoA);

}

void SceneGame::InitSkyDome()
{
	using namespace DirectX;

	ID3D11Device* device = GetDevice();
	if (!device) return;

	// 1) メッシュ初期化
	m_skyMesh.Init(device, 32, 16, 1.0f);

	// 2) シェーダー初期化
	m_skyShader.Init();

	// 3) テクスチャ取得 → シェーダへ設定
	//    ・事前に BuildLoadTasks で Load(TextureTag::Background_Game) しておく前提
	m_pSkyTex = TextureManager::Get(TextureTag::SkyDome);
	if (m_pSkyTex)
	{
		ID3D11ShaderResourceView* srv = m_pSkyTex->GetResource();
		m_skyShader.SetTexture(srv);      // t0 に設定
		m_skyShader.SetUseTexture(true);  // gUseTexture = 1
	}
	else
	{
		// 読み込み失敗時にだけメッセージを出したいならここで
		// MessageBox(nullptr, "Background_Game テクスチャが読み込まれていません", "Error", MB_OK);
		m_skyShader.SetUseTexture(false); // プロシージャルにフォールバック
	}

	// 4) 深度ステート（深度読み取りのみ）
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	dsDesc.StencilEnable = FALSE;

	HRESULT hr = device->CreateDepthStencilState(&dsDesc, &m_pDepthNoWrite);
	if (FAILED(hr))
	{
		m_pDepthNoWrite = nullptr;
	}


}

void SceneGame::SkyDomeDorw()
{
	using namespace DirectX;

	ID3D11DeviceContext* context = GetContext();

	// 1. メインカメラから View / Proj 行列と位置を取得
	Camera* cam = Camera::Main();
	if (!cam) return;

	m_view = cam->GetView();
	m_proj = cam->GetProjection();

	Transform* tf = cam->gameObject->GetComponent<Transform>();
	const XMFLOAT4X4& M = tf->GetMatrix();
	XMFLOAT3 camPos(M._41, M._42, M._43);

	// 2. カメラ中心・大きな半径の球を World 行列として作成
	float radius = 100.0f;
	XMMATRIX S = XMMatrixScaling(radius, radius, radius);
	XMMATRIX T = XMMatrixTranslation(camPos.x, camPos.y, camPos.z);
	XMMATRIX W = S * T;

	XMFLOAT4X4 worldSky, viewT, projT;
	XMStoreFloat4x4(&worldSky, XMMatrixTranspose(W));
	XMStoreFloat4x4(&viewT, XMMatrixTranspose(XMLoadFloat4x4(&m_view)));
	XMStoreFloat4x4(&projT, XMMatrixTranspose(XMLoadFloat4x4(&m_proj)));

	// 3. スカイドーム用シェーダに行列を渡す
	m_skyShader.SetWVP(worldSky, viewT, projT);

	// 4. 現在の深度ステートを退避してから、スカイドーム用へ切り替え
	ID3D11DepthStencilState* oldDS = nullptr;
	UINT                     oldRef = 0;
	context->OMGetDepthStencilState(&oldDS, &oldRef);

	context->OMSetDepthStencilState(m_pDepthNoWrite, 1);

	// 5. シェーダをバインドしてメッシュを描画
	m_skyShader.Bind();
	context->RSSetState(nullptr);
	m_skyMesh.Draw(context);

	// 6. 深度ステートを元に戻す
	context->OMSetDepthStencilState(oldDS, oldRef);
	if (oldDS)
	{
		oldDS->Release();
	}
}

void SceneGame::Update()
{
}

void SceneGame::Draw()
{
	SkyDomeDorw();
}

void SceneGame::CreateStageFromCSV(const std::string& filename)
{
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::string msg = "File Load Failed: " + filename;
		MessageBox(NULL, msg.c_str(), "Error", MB_OK);
		return;
	}

	World& w = GetWorld();
	std::string line;
	int lineCount = 0; // 行数のカウント用

	while (std::getline(file, line))
	{
		lineCount++;

		// 空行やコメント(#)をスキップ
		if (lineCount == 1 && line.size() >= 3)
		{
			if ((unsigned char)line[0] == 0xEF &&
				(unsigned char)line[1] == 0xBB &&
				(unsigned char)line[2] == 0xBF) {
				line = line.substr(3); // 先頭3文字をカット
			}
		}

		std::stringstream ss(line);
		std::string segment;
		std::vector<std::string> data;

		while (std::getline(ss, segment, ','))
		{
			data.push_back(segment);
		}

		if (data.size() < 7) continue;

		try // ★ エラー監視開始
		{
			// 1. タイプ
			std::string typeName = data[0];
			PrefabId id = GetPrefabIdFromString(typeName);

			// 2. 座標
			float px = std::stof(data[1]);
			float py = std::stof(data[2]);
			float pz = std::stof(data[3]);

			// 3. 回転
			float rx = std::stof(data[4]);
			float ry = std::stof(data[5]);
			float rz = std::stof(data[6]);

			// 4. スケール
			float sx = 1.0f;
			float sy = 1.0f;
			float sz = 1.0f;

			if (data.size() >= 10)
			{
				sx = std::stof(data[7]);
				sy = std::stof(data[8]);
				sz = std::stof(data[9]);
			}

			// 生成
			Prefab::Instantiate(
				w,
				id,
				Vector3(px, py, pz),
				Vector3(rx, ry, rz),
				Vector3(sx, sy, sz)
			);
		}
		catch (const std::invalid_argument& e) // ★ 変換できない文字があった場合ここに来る
		{
			// エラー内容を表示する
			std::string errorMsg = "CSV Error at Line " + std::to_string(lineCount) + "\n";
			errorMsg += "Content: " + line + "\n";
			errorMsg += "Reason: " + std::string(e.what());

			MessageBox(NULL, errorMsg.c_str(), "CSV Format Error", MB_OK);
		}
		catch (const std::out_of_range& e)
		{
			MessageBox(NULL, "Number out of range in CSV", "Error", MB_OK);
		}
	}
}


// 文字列の前後の空白を削除するヘルパー関数
std::string Trim(const std::string& str)
{
	const char* trimChars = " \t\n\r\f\v\"";

	std::string result = str;

	// 左側の空白削除
	result.erase(0, result.find_first_not_of(trimChars));

	// 右側の空白削除
	result.erase(result.find_last_not_of(trimChars) + 1);

	return result;
}

PrefabId SceneGame::GetPrefabIdFromString(const std::string& rawname)
{
	// 空白を削除してきれいにする
	std::string name = Trim(rawname);

	// CSVに書く名前とPrefabIdの対応付け
	if (name == "Swing") return PrefabId::Swing;
	if (name == "Akican") return PrefabId::Akican;
	if (name == "Desktop") return PrefabId::Desktop;
	if (name == "Gomibukuro") return PrefabId::Gomibukuro;
	if (name == "Isu") return PrefabId::Isu;
	if (name == "Kuukseizixyoukii") return PrefabId::Kuukseizixyoukii;
	if (name == "Ningixyou") return PrefabId::Ningixyou;
	if(name == "SagaraDoll") return PrefabId::SagaraDoll;

	if (name == "Benti") return PrefabId::Benti;
	if (name == "Aircon") return PrefabId::Aircon;
	if (name == "Jihanki") return PrefabId::Jihanki;
	if (name == "Reizouko") return PrefabId::Reizouko;

	if (name == "Ufo") return PrefabId::Ufo;
	if (name == "Eisei") return PrefabId::Eisei;

	// 一致するものがなかった場合、勝手にSwingにせず、エラーを出す！
	std::string errorMsg = "Unknown Prefab Name: [" + name + "]\n(Fallback to Swing)";
	MessageBox(NULL, errorMsg.c_str(), "Prefab ID Error", MB_OK);

	// とりあえず無効な値を返すか、Swing等をデフォルトにする
	return PrefabId::Swing;
}

