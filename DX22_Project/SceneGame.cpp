#include "SceneGame.h"
#include "GameObject.h"
#include "AllComponent.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "Prefab.h"
#include "PrefabSet.h"
#include "LineManager.h"
#include "GlassObjectSpawner.h"

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

	GameObject& timerManager = w.CreateGameObject();
	timerManager.SetTag(Tag::TimerUI);
}

void SceneGame::CreateCameraAndPlayer()
{
	World& w = GetWorld();

	//70×70 ①
	GameObject& cameraPivot = w.CreateGameObject(Vector3::zero(), Vector3(0.f, 180.f, 0.f), Vector3::one());

	//70×70 ②
	//GameObject& cameraPivot = w.CreateGameObject(Vector3::zero(), Vector3(0.f, 40.f, 0.f), Vector3::one());
	
	
	//60×60①
	//GameObject& cameraPivot = w.CreateGameObject(Vector3::zero(), Vector3(0.f, 180.f, 0.f), Vector3::one());

	//60×60②
	//GameObject& cameraPivot = w.CreateGameObject(Vector3::zero(), Vector3(0.f, 0.f, 0.f), Vector3::one());


	cameraPivot.SetTag(Tag::CamreraPivot);
	cameraPivot.AddComponent<CameraControl>();

	GameObject& mainCamera = cameraPivot.CreateChild(Camera::kInitPos, Camera::kInitRotate, Vector3::one());
	mainCamera.SetTag(Tag::MainCamera);
	mainCamera.AddComponent<Camera>();
	mainCamera.AddComponent<CameraShake>();

	//70×70 ①
	GameObject& player = w.CreateGameObject(Vector3(0.f, 2.f, 0.f), Vector3(0.f, 0.f, 0.f));
	 
	//70×70 ②
	//GameObject& player = w.CreateGameObject(Vector3(0.f, 2.f, 0.f), Vector3(0.f, 220.f, 0.f));
	
	
	//60×60①
	//GameObject& player = w.CreateGameObject(Vector3(0.f, 2.f, 0.f), Vector3(0.f, 0.f, 0.f));

	//60×60②
	//GameObject& player = w.CreateGameObject(Vector3(0.f, 2.f, 0.f), Vector3(0.f, 180.f, 0.f));


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
	World& w = GetWorld();

	//70端
	constexpr float SIZE = 80.f;
	constexpr float HALF = SIZE / 2.f;
	constexpr float OFFSET = HALF + 0.5f;


	//60端
	/*constexpr float SIZE = 60.f;
	constexpr float HALF = 60.f / 2.f;*/

	//70フィールド
	GameObject& floor = w.CreateGameObject(Vector3(0.f, -1.f, 0.f), Vector3::zero(), Vector3(SIZE, 0.1f, SIZE));
	
	//60フィールド
	//GameObject& floor = w.CreateGameObject(Vector3(0.f, -1.f, 0.f), Vector3::zero(), Vector3(60.f, 0.1f, 60.f));


	MeshFilter& mf = floor.AddComponent<MeshFilter>(PrimitiveMeshTag::Plane); mf.boundsRadius = 0.f;
	floor.AddComponent<GroundRenderer>();
	BoxCollider& collider = floor.AddComponent<BoxCollider>();
	collider.layer = Layer::Ground;

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
	struct Pos { float x; float z; };
	struct Rot { float x, y, z; };



	//70×70①

	//--------------------------------------
   // Grade 1
   //--------------------------------------

	const Pos positionsGrade_1[] = {
    { 10,  5}, {  9,  4}, {  9,  5}, { 10,  4}, {  7,  5}, {  7,  6}, { 11,  3}, { 11,  4},
	{ 10,  8}, {  6,  7}, {  1, 10}, {  0,  9}, {  2, 12}, {  0, 12}, {  5, 10}, { -7,  7},
	{ -9,  7}, { -8,  6}, { -9,  8}, {-10,  2}, {-13,  9}, { -7,  8}, { -9,  6}, {-11,  8},
	{ -9,  5}, {-14, -7}, {-15, -8}, {-18, -8}, {-13, -7}, {-14, -8}, {-16, -4}, {-16,-11},
	{-15, -6}, {-17,-11}, {-11, -7}, {-17, -7}, {-16,-12}, {-16, -6}, {-12, -6}, {-17,-10},
	{-14, -6}, {-15,-10}, {-13,-10}, {-17, -9}, {-13, -5}, { -5,-17}, { -4,-14}, { -6,-16},
	{ -6,-17}, {  1,-14}, {  8,-10}, { 12,-12}, {  9, -9}, { 11, -9}, {  9, -8}, {  8, -8},
	{ 11,-12}, {  9, -6}, {  6, -9}, {  9,-10}
	};
	
	const Rot rotationsGrade_1[] = {
		  {  0,109,151}, {  5, 79, 70}, {104, 48, 83}, {117,178,127}, {  0, 76, 57}, {104, 61, 64}, {127, 38,158}, {108, 44,  9},
    { 85,111,101}, { 74,160, 56}, { 60,102, 44}, {105,155,168}, { 81,134, 39}, {162, 83, 18}, {111,114, 51}, { 41,170,151},
    {104,130,176}, {158,131, 84}, { 71, 39, 64}, {136, 75, 12}, { 38, 53,128}, { 79, 63,  6}, {170,153, 82}, {145,  9,110},
    {135, 88, 12}, {166,136,138}, { 18,104, 22}, { 48, 12,149}, {116, 16,163}, { 57, 88, 48}, { 63,  9, 92}, {143,164, 44},
    {155, 49, 20}, {119,152,130}, { 93, 67, 50}, {133, 52, 79}, {143, 11, 41}, {124, 15, 43}, {121, 29, 60}, {177, 46,126},
    {171,109, 16}, {126,129,153}, { 91, 90, 62}, { 83, 73,109}, { 61, 30,167}, { 78, 70,100}, { 26, 90, 81}, { 69, 61, 78},
    {164,175, 65}, { 90, 83,151}, { 27,163, 69}, {135, 77, 97}, { 91, 97,176}, { 30,  6, 36}, { 17, 99, 41}, {163, 87,180},
    {166,101, 83}, { 73,119, 32}, {156,151,177}, {112,178, 82}
	};


	//--------------------------------------
	// Grade 2
	//--------------------------------------

	const Pos positionsGrade_2[] = {
		 {-13,-20}, {-12,-13}, {-13,-18}, { -8,-16}, {-10,-16}, {-10,-20}, {-10,-14}, { -8,-18},
	{-13,-22}, {-16,-24}, { 24,-30}, { 25,-24}, { 17,-26}, { 17,-22}, { 16,-24}, { 15, 12},
	{ 16, 14}, { 12, 12}, { 16,  4}, { 16, 10}, {  9, 12}, { 19,  8}, { 15,  8}, { 22,  7},
	{  9, 14}, { -1, 14}, { -5, 17}, { -1, 27}, { -1, 23}, { -4, 28}, {-21,  6}, {-21,  8},
	{-18, 11}, {-27,  6}, {-28,  8}, {-31,  4}, {-25,  5}, {-23, 12}, {-22,  2}, {-25,  7}
	};

	const Rot rotationsGrade_2[] = {
		 { 27, 89,110}, { 25,152, 62}, {150, 62,  0}, {147, 29,174}, { 46, 56,180}, { 27, 22, 75}, { 60,156, 86}, { 70, 56,115},
	{180,165,116}, { 21,178,  0}, { 53,123, 10}, {172,125,  6}, { 58,132,114}, {122,117, 71}, {116,  1, 42}, {161, 73,144},
	{ 39,112,132}, { 71, 19,124}, { 25,  8,180}, {134,120,147}, {155,  5,139}, {105,137, 57}, { 69, 48, 47}, {158, 59, 98},
	{ 31,166,179}, { 48,158,176}, { 62,  6,179}, { 69,153,127}, { 40,163,119}, { 57, 89, 32}, {122, 16,147}, {141, 50, 26},
	{  5, 41, 97}, { 80,162, 62}, { 42,135, 59}, { 68, 11,110}, { 60,127, 83}, { 89, 85,146}, {142, 37,132}, { 60, 88,  7}
	};

	//--------------------------------------
	// Grade 3
	//--------------------------------------

	const Pos positionsGrade_3[] = {
		 {-21, 30}, {-16, 26}, {-22, 24}, {-19, 26}, {-18, 23}, {-28,-21}, {-25,-23}, {-29,-24},
	{-25,-26}, {-24,-20}, {-28,-18}, {-31,-21}, {-28,-27}, {-28, -8}, {-16,-17}, {-20,-28},
	{-29,-13}, {-13,-25}, {-31,-18}, {-21,-15}, {-24,-11}, {-23,-30}, {-29, -5}, {-17,-20},
	{-31,-10}, { 23, -7}, { 29,  3}, { 28, -8}, { 28,-13}, { 24, -2}
	};

	const Rot rotationsGrade_3[] = {
		{102,  4, 41}, { 52,  9,170}, {115, 71, 77}, {140,171, 58}, {106, 60,137}, { 37, 59, 75}, { 99, 20, 79}, {143,128,114},
	{ 40,152,  1}, {  9, 18,  3}, {169,134,146}, { 47,142,  8}, {127, 46,170}, { 31, 40,175}, {143, 72, 48}, {144, 63,104},
	{ 36,155,104}, { 35, 37, 88}, { 15,  5,132}, {126,172,130}, { 33,145, 82}, {169, 62,155}, { 29,173, 12}, { 15,172,134},
	{ 53, 36,141}, {140, 98, 80}, {139, 26, 15}, {117,155,169}, {105, 58,144}, {132, 41, 42}
	};



	//70×70②
	//--------------------------------------
   // Grade 1
   //--------------------------------------

	//const Pos positionsGrade_1[] = {
	//{ 7, 11 }, { 7, 10 }, { 3,  8 }, { 6, 10 }, { 9,  8 }, { 7,  8 }, { 7, 12 }, { 8,  8 },
	//{ 8, 10 }, { 5, 12 }, { -9, 13 }, { -5, 12 }, { -11, 12 }, { -8, 13 }, { -7, 15 }, { -11, -1 },
	//{ -11,  0 }, { -14, -3 }, { -12,  0 }, { -13,  2 }, { -14,  2 }, { -12,  4 }, { -13, -2 }, { -11,  1 },
	//{ -13,  0 }, { -11,-13 }, { -12,-14 }, { -11,-14 }, { -9,-11 }, { -13, -9 }, { -12,-10 }, { -15,-14 },
	//{ -13,-13 }, { -9,-15 }, { -14,-10 }, { -12,-13 }, { -10,-12 }, { -12,-12 }, { -14,-12 }, { -10,-15 },
	//{ -13,-11 }, { -11,-12 }, { -9,-16 }, { -10,-16 }, { -10,-13 }, { 8,-14 }, { 4,-12 }, { 10,-13 },
	//{ 5,-15 }, { 7,-15 }, { 10, -3 }, { 9, -1 }, { 11, -1 }, { 10, -2 }, { 9, -5 }, { 9,  2 },
	//{ 9,  0 }, { 11, -2 }, { 12, -4 }, { 9, -6 }
	//};

	//const Rot rotationsGrade_1[] = {
	//	  {  0,109,151}, {  5, 79, 70}, {104, 48, 83}, {117,178,127}, {  0, 76, 57}, {104, 61, 64}, {127, 38,158}, {108, 44,  9},
	//{ 85,111,101}, { 74,160, 56}, { 60,102, 44}, {105,155,168}, { 81,134, 39}, {162, 83, 18}, {111,114, 51}, { 41,170,151},
	//{104,130,176}, {158,131, 84}, { 71, 39, 64}, {136, 75, 12}, { 38, 53,128}, { 79, 63,  6}, {170,153, 82}, {145,  9,110},
	//{135, 88, 12}, {166,136,138}, { 18,104, 22}, { 48, 12,149}, {116, 16,163}, { 57, 88, 48}, { 63,  9, 92}, {143,164, 44},
	//{155, 49, 20}, {119,152,130}, { 93, 67, 50}, {133, 52, 79}, {143, 11, 41}, {124, 15, 43}, {121, 29, 60}, {177, 46,126},
	//{171,109, 16}, {126,129,153}, { 91, 90, 62}, { 83, 73,109}, { 61, 30,167}, { 78, 70,100}, { 26, 90, 81}, { 69, 61, 78},
	//{164,175, 65}, { 90, 83,151}, { 27,163, 69}, {135, 77, 97}, { 91, 97,176}, { 30,  6, 36}, { 17, 99, 41}, {163, 87,180},
	//{166,101, 83}, { 73,119, 32}, {156,151,177}, {112,178, 82}
	//};


	//////--------------------------------------
	////// Grade 2
	//////--------------------------------------

	//const Pos positionsGrade_2[] = {
	//	  {-11, 22}, { -7, 25}, {-12, 20}, { -9, 23}, { -6, 22}, {-12, 26}, {-10, 25}, {-13, 22},
	//{-10, 27}, { -8, 21}, {-21, -2}, {-23, -3}, {-22, -5}, {-24,  0}, {-19, -4}, {  0,-21},
	//{ -1,-18}, { -4,-19}, { -2,-21}, {  3,-20}, {  2,-18}, { -1,-25}, { -4,-17}, { -4,-23},
	//{  1,-24}, { 24,-19}, { 25,-14}, { 23,-14}, { 27,-15}, { 25,-17}, { 11, 21}, { 14, 20},
	//{ 14, 18}, { 16, 18}, { 13, 16}, { 17, 21}, { 10, 18}, { 14, 22}, { 12, 18}, { 16, 16}
	//};

	//const Rot rotationsGrade_2[] = {
	//	 { 27, 89,110}, { 25,152, 62}, {150, 62,  0}, {147, 29,174}, { 46, 56,180}, { 27, 22, 75}, { 60,156, 86}, { 70, 56,115},
	//{180,165,116}, { 21,178,  0}, { 53,123, 10}, {172,125,  6}, { 58,132,114}, {122,117, 71}, {116,  1, 42}, {161, 73,144},
	//{ 39,112,132}, { 71, 19,124}, { 25,  8,180}, {134,120,147}, {155,  5,139}, {105,137, 57}, { 69, 48, 47}, {158, 59, 98},
	//{ 31,166,179}, { 48,158,176}, { 62,  6,179}, { 69,153,127}, { 40,163,119}, { 57, 89, 32}, {122, 16,147}, {141, 50, 26},
	//{  5, 41, 97}, { 80,162, 62}, { 42,135, 59}, { 68, 11,110}, { 60,127, 83}, { 89, 85,146}, {142, 37,132}, { 60, 88,  7}
	//};

	//////--------------------------------------
	////// Grade 3
	//////--------------------------------------

	//const Pos positionsGrade_3[] = {
	//	  { 28, 16}, { 29, 19}, { 27, 12}, { 25, 19}, { 30, 13}, {-29, 14}, {-28, 10}, {-30, 17},
	//{-26, 15}, {-27, 18}, {-31, 11}, {-32, 14}, {-25, 12}, {-22,  7}, {-29,  5}, {-23, 22},
	//{-22, 16}, {-24,  3}, {-22, 19}, {-20, 12}, {-32, 22}, {-28, 26}, {-19, 18}, {-32,  6},
	//{-32, 25}, { -6,-29}, { -4,-32}, { -1,-28}, { -1,-31}, { -7,-25}
	//};

	//const Rot rotationsGrade_3[] = {
	//	{102,  4, 41}, { 52,  9,170}, {115, 71, 77}, {140,171, 58}, {106, 60,137}, { 37, 59, 75}, { 99, 20, 79}, {143,128,114},
	//{ 40,152,  1}, {  9, 18,  3}, {169,134,146}, { 47,142,  8}, {127, 46,170}, { 31, 40,175}, {143, 72, 48}, {144, 63,104},
	//{ 36,155,104}, { 35, 37, 88}, { 15,  5,132}, {126,172,130}, { 33,145, 82}, {169, 62,155}, { 29,173, 12}, { 15,172,134},
	//{ 53, 36,141}, {140, 98, 80}, {139, 26, 15}, {117,155,169}, {105, 58,144}, {132, 41, 42}
	//};


	//60×60①
		//--------------------------------------
   // Grade 1
   //--------------------------------------

	//const Pos positionsGrade_1[] = {
	//{ -1,-10}, { -2,-10}, {  0,-11}, { -1,-12}, {  2,-11}, {  0,-10}, { -5, -9}, {  2,-10},
	//{ -5,-10}, { -4,-10}, {  9,-10}, {  7, -7}, {  9, -7}, {  9, -6}, { 12, -5}, {  7,  6},
	//{  6,  7}, {  7,  7}, {  9,  3}, {  8,  3}, {  8,  4}, {  7,  5}, {  6,  8}, {  8,  5},
	//{  9,  5}, { -2, 12}, {  1, 14}, {  4, 18}, { -3, 13}, {  3, 11}, {  4, 17}, {  3, 14},
	//{  2, 14}, {  5, 16}, { -1, 16}, {  0, 18}, {  0, 16}, {  3, 15}, {  3, 16}, {  5, 15},
	//{  2, 12}, {  1, 12}, {  3, 13}, {  2, 15}, {  0, 10}, {-10,  8}, {-10,  1}, {-11,  4},
	//{ -9,  4}, { -8,  7}, {-10, -9}, {-11, -5}, {-11,-11}, {-12, -6}, { -9, -8}, {-11,-10},
	//{-10, -6}, { -9, -7}, {-10, -8}, { -8, -8}
	//};

	//const Rot rotationsGrade_1[] = {
	//	  {  0,109,151}, {  5, 79, 70}, {104, 48, 83}, {117,178,127}, {  0, 76, 57}, {104, 61, 64}, {127, 38,158}, {108, 44,  9},
	//{ 85,111,101}, { 74,160, 56}, { 60,102, 44}, {105,155,168}, { 81,134, 39}, {162, 83, 18}, {111,114, 51}, { 41,170,151},
	//{104,130,176}, {158,131, 84}, { 71, 39, 64}, {136, 75, 12}, { 38, 53,128}, { 79, 63,  6}, {170,153, 82}, {145,  9,110},
	//{135, 88, 12}, {166,136,138}, { 18,104, 22}, { 48, 12,149}, {116, 16,163}, { 57, 88, 48}, { 63,  9, 92}, {143,164, 44},
	//{155, 49, 20}, {119,152,130}, { 93, 67, 50}, {133, 52, 79}, {143, 11, 41}, {124, 15, 43}, {121, 29, 60}, {177, 46,126},
	//{171,109, 16}, {126,129,153}, { 91, 90, 62}, { 83, 73,109}, { 61, 30,167}, { 78, 70,100}, { 26, 90, 81}, { 69, 61, 78},
	//{164,175, 65}, { 90, 83,151}, { 27,163, 69}, {135, 77, 97}, { 91, 97,176}, { 30,  6, 36}, { 17, 99, 41}, {163, 87,180},
	//{166,101, 83}, { 73,119, 32}, {156,151,177}, {112,178, 82}
	//};


	////--------------------------------------
	//// Grade 2
	////--------------------------------------

	//const Pos positionsGrade_2[] = {
	//	 {-21,  4}, {-20,  2}, {-21,  0}, {-19, -4}, {-17,  4}, {-19,  5}, {-15,  0}, {-18,  2},
	//{-21, -3}, {-23,  0}, { -4,-16}, { -8,-15}, { -5,-19}, { -6,-14}, { -7,-19}, { 17,-15},
	//{ 21,-17}, { 18,-20}, { 14,-14}, { 14,-18}, { 16,-17}, { 20,-20}, { 18,-13}, { 19,-18},
	//{ 14,-16}, { 13,  2}, { 13,  7}, { 13,  4}, { 11,  7}, { 15,  6}, { -5, 17}, { -5, 13},
	//{ -5, 15}, { -7, 13}, { -2, 14}, { -1, 19}, { -8, 16}, { -8, 18}, { -3, 17}, { -5, 19}
	//};

	//const Rot rotationsGrade_2[] = {
	//	 { 27, 89,110}, { 25,152, 62}, {150, 62,  0}, {147, 29,174}, { 46, 56,180}, { 27, 22, 75}, { 60,156, 86}, { 70, 56,115},
	//{180,165,116}, { 21,178,  0}, { 53,123, 10}, {172,125,  6}, { 58,132,114}, {122,117, 71}, {116,  1, 42}, {161, 73,144},
	//{ 39,112,132}, { 71, 19,124}, { 25,  8,180}, {134,120,147}, {155,  5,139}, {105,137, 57}, { 69, 48, 47}, {158, 59, 98},
	//{ 31,166,179}, { 48,158,176}, { 62,  6,179}, { 69,153,127}, { 40,163,119}, { 57, 89, 32}, {122, 16,147}, {141, 50, 26},
	//{  5, 41, 97}, { 80,162, 62}, { 42,135, 59}, { 68, 11,110}, { 60,127, 83}, { 89, 85,146}, {142, 37,132}, { 60, 88,  7}
	//};

	////--------------------------------------
	//// Grade 3
	////--------------------------------------

	//const Pos positionsGrade_3[] = {
	//	 { 16, 21}, { 21, 23}, { 18, 18}, { 23, 20}, { 21, 15}, {-20, 16}, {-24, 16}, {-23, 20},
	//{-26, 19}, {-27, 15}, {-19, 19}, {-22, 13}, {-23, 10}, {-20, 10}, {-20, 24}, {-18, 13},
	//{-26, 10}, {-16, 17}, {-23, 24}, {-16, 21}, {-26, 22}, {-17,  7}, {-24,  3}, {-27,  4},
	//{-13, 10}, { 19,-27}, { 10,-27}, { 10,-24}, { 22,-27}, { 18,-23}
	//};

	//const Rot rotationsGrade_3[] = {
	//	{102,  4, 41}, { 52,  9,170}, {115, 71, 77}, {140,171, 58}, {106, 60,137}, { 37, 59, 75}, { 99, 20, 79}, {143,128,114},
	//{ 40,152,  1}, {  9, 18,  3}, {169,134,146}, { 47,142,  8}, {127, 46,170}, { 31, 40,175}, {143, 72, 48}, {144, 63,104},
	//{ 36,155,104}, { 35, 37, 88}, { 15,  5,132}, {126,172,130}, { 33,145, 82}, {169, 62,155}, { 29,173, 12}, { 15,172,134},
	//{ 53, 36,141}, {140, 98, 80}, {139, 26, 15}, {117,155,169}, {105, 58,144}, {132, 41, 42}
	//};


	//60×60②
	
	//--------------------------------------
   // Grade 1
   //--------------------------------------

	//const Pos positionsGrade_1[] = {
	// {  5, -8}, {  3,-12}, {  5,-12}, {  1,-13}, { -2,-10}, {  4,-11}, { -1,-10}, {  0,-13},
	//{  3,-10}, {  2,-12}, { 10,  2}, {  9, -5}, { 10, -1}, {  9,  0}, {  9, -2}, {  7, 11},
	//{  5,  9}, { 10,  6}, {  7,  6}, {  6, 12}, {  7,  7}, {  6, 10}, { 10,  8}, {  7, 10},
	//{  6,  7}, { -3, 11}, { -4, 13}, { -2,  9}, { -7, 12}, { -5, 12}, { -4, 10}, { -1,  9},
	//{ -6, 13}, { -3, 13}, { -6, 15}, { -1, 15}, { -8, 12}, { -4, 12}, { -1, 10}, { -4,  8},
	//{ -5, 10}, { -6,  9}, { -3,  9}, { -5,  9}, { -2, 14}, {-10,  3}, {-13,  1}, {-12,  4},
	//{-10,  5}, {-12,  2}, { -6, -9}, { -9, -7}, { -8, -9}, {-10, -5}, { -8, -7}, { -7, -9},
	//{ -5, -9}, { -9, -4}, { -9, -5}, { -7, -8}
	//};

	//const Rot rotationsGrade_1[] = {
	//	  {  0,109,151}, {  5, 79, 70}, {104, 48, 83}, {117,178,127}, {  0, 76, 57}, {104, 61, 64}, {127, 38,158}, {108, 44,  9},
	//{ 85,111,101}, { 74,160, 56}, { 60,102, 44}, {105,155,168}, { 81,134, 39}, {162, 83, 18}, {111,114, 51}, { 41,170,151},
	//{104,130,176}, {158,131, 84}, { 71, 39, 64}, {136, 75, 12}, { 38, 53,128}, { 79, 63,  6}, {170,153, 82}, {145,  9,110},
	//{135, 88, 12}, {166,136,138}, { 18,104, 22}, { 48, 12,149}, {116, 16,163}, { 57, 88, 48}, { 63,  9, 92}, {143,164, 44},
	//{155, 49, 20}, {119,152,130}, { 93, 67, 50}, {133, 52, 79}, {143, 11, 41}, {124, 15, 43}, {121, 29, 60}, {177, 46,126},
	//{171,109, 16}, {126,129,153}, { 91, 90, 62}, { 83, 73,109}, { 61, 30,167}, { 78, 70,100}, { 26, 90, 81}, { 69, 61, 78},
	//{164,175, 65}, { 90, 83,151}, { 27,163, 69}, {135, 77, 97}, { 91, 97,176}, { 30,  6, 36}, { 17, 99, 41}, {163, 87,180},
	//{166,101, 83}, { 73,119, 32}, {156,151,177}, {112,178, 82}
	//};


	////--------------------------------------
	//// Grade 2
	////--------------------------------------

	//const Pos positionsGrade_2[] = {
	//	 { 16,  5}, { 15,  8}, { 13,  9}, { 11,  9}, { 17,  8}, { 14,  5}, { 19,  8}, { 18,  5},
	//{ 13,  7}, { 19, 10}, { -3, 23}, { -5, 19}, { -1, 22}, { -7, 25}, { -7, 23}, {-19, -5},
	//{-19,  0}, {-21,  0}, {-15,  0}, {-22, -4}, {-18,  2}, {-15, -2}, {-23, -2}, {-23,  0},
	//{-16, -5}, { -9,-15}, { -7,-11}, { -9,-17}, { -6,-16}, {-12,-12}, {  6,-11}, { 10,-12},
	//{ 13,-10}, { 12,-13}, {  9,-14}, { 14,-13}, { 11,-10}, {  8, -9}, { 13, -8}, { 15,-10}
	//};

	//const Rot rotationsGrade_2[] = {
	//	 { 27, 89,110}, { 25,152, 62}, {150, 62,  0}, {147, 29,174}, { 46, 56,180}, { 27, 22, 75}, { 60,156, 86}, { 70, 56,115},
	//{180,165,116}, { 21,178,  0}, { 53,123, 10}, {172,125,  6}, { 58,132,114}, {122,117, 71}, {116,  1, 42}, {161, 73,144},
	//{ 39,112,132}, { 71, 19,124}, { 25,  8,180}, {134,120,147}, {155,  5,139}, {105,137, 57}, { 69, 48, 47}, {158, 59, 98},
	//{ 31,166,179}, { 48,158,176}, { 62,  6,179}, { 69,153,127}, { 40,163,119}, { 57, 89, 32}, {122, 16,147}, {141, 50, 26},
	//{  5, 41, 97}, { 80,162, 62}, { 42,135, 59}, { 68, 11,110}, { 60,127, 83}, { 89, 85,146}, {142, 37,132}, { 60, 88,  7}
	//};

	////--------------------------------------
	//// Grade 3
	////--------------------------------------

	//const Pos positionsGrade_3[] = {
	//	{ 22,-20}, { 20,-24}, { 20,-27}, { 18,-20}, { 23,-27}, { 18, 20}, { 15, 23}, { 12, 21},
	//{ 15, 19}, { 12, 24}, {  8, 21}, { 18, 16}, { 20, 24}, { 13, 16}, {  9, 24}, { 23, 23},
	//{ 10, 17}, {  7, 13}, { 21, 13}, {  5, 22}, { 16, 11}, {  3, 19}, {  7, 18}, { 13, 11},
	//{ 10, 12}, {-22, -7}, {-27,-10}, {-25,  9}, {-20, -3}, {-22,  3}
	//};

	//const Rot rotationsGrade_3[] = {
	//	{102,  4, 41}, { 52,  9,170}, {115, 71, 77}, {140,171, 58}, {106, 60,137}, { 37, 59, 75}, { 99, 20, 79}, {143,128,114},
	//{ 40,152,  1}, {  9, 18,  3}, {169,134,146}, { 47,142,  8}, {127, 46,170}, { 31, 40,175}, {143, 72, 48}, {144, 63,104},
	//{ 36,155,104}, { 35, 37, 88}, { 15,  5,132}, {126,172,130}, { 33,145, 82}, {169, 62,155}, { 29,173, 12}, { 15,172,134},
	//{ 53, 36,141}, {140, 98, 80}, {139, 26, 15}, {117,155,169}, {105, 58,144}, {132, 41, 42}
	//};







   //--------------------------------------
   // Grade 1
   //--------------------------------------
	PrefabId prefabOrder1[] = {
		PrefabId::Swing,
		//PrefabId::Akican,
		//PrefabId::Desktop,
		PrefabId::Gomibukuro,
		PrefabId::Isu,
		//PrefabId::Kuukseizixyoukii,
		//PrefabId::Ningixyou
	};

	const int prefabCount1 = sizeof(prefabOrder1) / sizeof(prefabOrder1[0]);
	const int posCount1 = sizeof(positionsGrade_1) / sizeof(positionsGrade_1[0]);
	const int rotCount1 = sizeof(rotationsGrade_1) / sizeof(rotationsGrade_1[0]);

	for (int i = 0; i < posCount1; i++) {
		PrefabId id = prefabOrder1[i % prefabCount1];
		Rot rot = rotationsGrade_1[i % rotCount1];
		Prefab::Instantiate(
			w,
			id,
			Vector3(positionsGrade_1[i].x, -0.9f, positionsGrade_1[i].z),
			Vector3(rot.x, rot.y, rot.z),
			Vector3::one()
		);
	}

	//--------------------------------------
	// Grade 2
	//--------------------------------------
	PrefabId prefabOrder2[] = {
		PrefabId::Benti,
		//PrefabId::Aircon,
		//PrefabId::Jihanki,
		//PrefabId::Reizouko,
	};

	const int prefabCount2 = sizeof(prefabOrder2) / sizeof(prefabOrder2[0]);
	const int posCount2 = sizeof(positionsGrade_2) / sizeof(positionsGrade_2[0]);
	const int rotCount2 = sizeof(rotationsGrade_2) / sizeof(rotationsGrade_2[0]);

	for (int i = 0; i < posCount2; i++) {
		PrefabId id = prefabOrder2[i % prefabCount2];
		Rot rot = rotationsGrade_2[i % rotCount2];
		Prefab::Instantiate(
			w,
			id,
			Vector3(positionsGrade_2[i].x, -0.9f, positionsGrade_2[i].z),
			Vector3(rot.x, rot.y, rot.z),
			Vector3::one()
		);
	}


	//--------------------------------------
	// Grade 3
	//--------------------------------------
	PrefabId prefabOrder3[] = {
		PrefabId::Ufo,
		//PrefabId::Eisei,
	};

	const int prefabCount3 = sizeof(prefabOrder3) / sizeof(prefabOrder3[0]);
	const int posCount3 = sizeof(positionsGrade_3) / sizeof(positionsGrade_3[0]);
	const int rotCount3 = sizeof(rotationsGrade_3) / sizeof(rotationsGrade_3[0]);

	for (int i = 0; i < posCount3; i++) {
		PrefabId id = prefabOrder3[i % prefabCount3];
		Rot rot = rotationsGrade_3[i % rotCount3];
		Prefab::Instantiate(
			w,
			id,
			Vector3(positionsGrade_3[i].x, -0.9f, positionsGrade_3[i].z),
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

void SceneGame::CreateActionUI()
{
	World& w = GetWorld();

	GameObject& actionManager = w.CreateGameObject();
	actionManager.SetTag(Tag::ActionUI);
	actionManager.AddComponent<ActionUI>();
}

void SceneGame::CreateGlassObjectSpawner()
{
	World& w = GetWorld();

	GameObject& glassObjectSpawner = w.CreateGameObject();
	glassObjectSpawner.AddComponent<GlassObjectSpawner>();

	GameObject& Frost = w.CreateGameObject();
	//Frost.AddComponent<Sprite3DRenderer>(TextureTag::UIFrostNoise, Vector2::zero(), Vector2(12.8f, 7.2f));
}

void SceneGame::CreateWarp()
{
}

void SceneGame::InitPlayerTex()
{
	// プレイヤーモデル取得
	Model * model = ModelManager::Get(ModelTag::Player);
	if (!model)
	{
		return; // 読み込み失敗などの保険
	}

	// 体（lambert1）
	Texture* texBody = new Texture();
	texBody->Create("Assets/Model/playerchara_20251202/lambert1.png");

	if (texBody == NULL)
	{
		MessageBox(NULL, "a", "Error", MB_OK); // エラーメッセージの表示
	}

	// 頭皮・顔上部（lambert4）
	Texture* texFaceA = new Texture();
	texFaceA->Create("Assets/Model/playerchara_20251202/lambert4.png");

	if (texFaceA == NULL)
	{
		MessageBox(NULL, "aaa", "Error", MB_OK); // エラーメッセージの表示
	}

	// 目・口（lambert5）
	Texture* texFaceB = new Texture();
	texFaceB->Create("Assets/Model/playerchara_20251202/lambert5.png");

	if (texFaceB)
	{
		//MessageBox(NULL, "aaaa", "Error", MB_OK); // エラーメッセージの表示
	}

	// MaterialID に応じて割り当てる
	const unsigned int matNum = model->GetMaterialNum();
	for (unsigned int i = 0; i < matNum; ++i)
	{
		// ★ 非 const のマテリアルを取得（関数名は環境に合わせて）
		auto* mat = model->GetMaterialNonConst(i);

		if (!mat) continue;

		switch (i)
		{
		case 0:
			mat->pTexture = texFaceA;   // 頭皮
			break;
		case 1:
			mat->pTexture = texFaceB;   // 顔（目）
			break;
		case 2:
			mat->pTexture = texBody;    // 体
			break;
		}
	}
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


