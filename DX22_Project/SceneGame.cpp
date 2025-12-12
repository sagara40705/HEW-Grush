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
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Dash); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::DashMono); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::DashMoji); });

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
	outTasks.emplace_back([this] { CreateActionUI(); });
	outTasks.emplace_back([this] { InitPlayerTex(); });
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
	//70×70
	//GameObject& cameraPivot = w.CreateGameObject(Vector3::zero(), Vector3(0.f, 180.f, 0.f), Vector3::one());
	//60×60
	GameObject& cameraPivot = w.CreateGameObject(Vector3::zero(), Vector3(0.f, 0.f, 0.f), Vector3::one());
	cameraPivot.SetTag(Tag::CamreraPivot);
	cameraPivot.AddComponent<CameraControl>();

	GameObject& mainCamera = cameraPivot.CreateChild(Camera::kInitPos, Camera::kInitRotate, Vector3::one());
	mainCamera.SetTag(Tag::MainCamera);
	mainCamera.AddComponent<Camera>();
	mainCamera.AddComponent<CameraShake>();
	//70×70
	//GameObject& player = w.CreateGameObject(Vector3(0.f, 2.f, 0.f), Vector3(0.f, 0.f, 0.f));
	//60×60
	GameObject& player = w.CreateGameObject(Vector3(0.f, 2.f, 0.f), Vector3(0.f, 180.f, 0.f));
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

	constexpr float SIZE = 80.f;
	constexpr float HALF = 80.f / 2.f;
	//GameObject& floor = w.CreateGameObject(Vector3(0.f, -1.f, 0.f), Vector3::zero(), Vector3(70.f, 0.1f, 70.f));
	GameObject& floor = w.CreateGameObject(Vector3(0.f, -1.f, 0.f), Vector3::zero(), Vector3(60.f, 0.1f, 60.f));
	MeshFilter& mf = floor.AddComponent<MeshFilter>(PrimitiveMeshTag::Plane); mf.boundsRadius = 0.f;
	floor.AddComponent<GroundRenderer>();
	BoxCollider& collider = floor.AddComponent<BoxCollider>();
	collider.layer = Layer::Ground;

	GameObject& wT = w.CreateGameObject(Vector3(0.f, 0.f,  HALF), Vector3::zero(), Vector3(SIZE, 4.f, 0.6f));
	MeshFilter& mfT = wT.AddComponent<MeshFilter>(PrimitiveMeshTag::Cube); mfT.boundsRadius = 0.f;
	wT.AddComponent<GroundRenderer>();
	BoxCollider& bcT = wT.AddComponent<BoxCollider>(); bcT.layer = Layer::Ground;
	wT.AddComponent<StageWall>();

	GameObject& wB = w.CreateGameObject(Vector3(0.f, 0.f, -HALF), Vector3::zero(), Vector3(SIZE, 4.f, 0.2f));
	MeshFilter& mfB = wB.AddComponent<MeshFilter>(PrimitiveMeshTag::Cube); mfB.boundsRadius = 0.f;
	wB.AddComponent<GroundRenderer>();
	BoxCollider& bcB = wB.AddComponent<BoxCollider>(); bcB.layer = Layer::Ground;
	wB.AddComponent<StageWall>();

	GameObject& wR = w.CreateGameObject(Vector3( HALF, 0.f, 0.f), Vector3::zero(), Vector3(0.2f, 4.f, SIZE));
	MeshFilter& mfR = wR.AddComponent<MeshFilter>(PrimitiveMeshTag::Cube); mfR.boundsRadius = 0.f;
	wR.AddComponent<GroundRenderer>();
	BoxCollider& bcR = wR.AddComponent<BoxCollider>(); bcR.layer = Layer::Ground;
	wR.AddComponent<StageWall>();

	GameObject& wL = w.CreateGameObject(Vector3(-HALF, 0.f, 0.f), Vector3::zero(), Vector3(0.2f, 4.f, SIZE));
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

	//70×70
	/*const Pos positionsGrade_1[] = {
	{  8,  6}, {  9, -8}, { -4, -25}, { -2, 13}, { -5,  8}, {  8, -6}, { -8, -13}, {  3, -10},
    { -9,  5}, { -5, -6}, {  7, -2}, { -3, -10}, { -7, -9}, { -15,  3}, { -12,  9}, { -7,  -7},
    {-19, -8}, {-13, -7}, {  1, -12}, { 20, -18}, {  7, 10}, { 11,  -5}, {-11, -12}, { -8, 10},
    {  5, -1}, {  6, -13}, { -8, -1}, { 12,  -8}, {  6,  4}, { -8,  3}, {-15,  -1}, {-12,  -9},
    {  8, -11}, { -5,  6}, {-11,   2}, {  1,  7}, {  9, 10}, {  3, 10}, {-20,  18}, {  3,  6},
    { -9,  8}, {  5, -11}, { -2, -34}, {  5,  7}, {  1,  5}, { -6, 11}, { -7,  6}, {-22,   4},
    {  3, -8}, { 13,  0}
	};
	
	const Rot rotationsGrade_1[] = {
		 {172,141, 79}, { 79, 86, 43}, { 98, 82, 52}, { 43,115, 33}, { 11,163, 60}, { 88,108, 39}, {126, 34, 29}, {169, 63,  8},
    {124, 72, 87}, { 83, 10,167}, { 99,  1, 80}, { 16, 37,157}, { 77, 80, 53}, {105, 33,123}, {151, 87,132}, {100, 66,  0},
    { 67, 34,145}, { 15,168,110}, {107,150, 87}, {157,123,180}, {170,167,140}, { 68,177, 48}, { 19, 43, 56}, {  4,  4,107},
    { 51,164,165}, {134, 50, 18}, {158,143, 35}, {  7,158, 77}, { 18, 40, 11}, {175, 26,124}, { 41, 27, 57}, {162, 14, 28},
    {133, 84, 92}, {105, 73,159}, {127, 20,103}, { 10, 31, 67}, {171, 28,111}, { 54, 26, 15}, {120,164, 45}, {  3, 38,106},
    { 62,173, 34}, { 43, 12, 91}, {  8,126, 60}, {158,109,128}, {149,  1, 10}, { 72, 67,149}, {163, 11, 38}, {121,153, 36},
    {151, 45,119}, { 89, 76, 56}
	};*/


	//60×60
	const Pos positionsGrade_1[] = {
		{ -2, 11}, {  9,  5}, { 25,-10}, { -6,-12}, { -5,  5}, { 13,  4}, { -3,-14}, {  8, -8},
	{ 13, -2}, { -9, -8}, {-12, -2}, {-10,  1}, { -2, -9}, {-10, -6}, { -8,  3}, {  5, -4},
	{  6,  4}, { -1, 12}, { -9, -1}, { 29, 14}, {-10, 19}, { -5, -6}, { -4,-10}, {-19,-10},
	{  6, 13}, { -2, 13}, {-12,  9}, {-10, 10}, {-12,-18}, {  6, -7}, { -8, -9}, {-13,  3},
	{ -4,  8}, { -9,  7}, {-25,  4}, { -6,  5}, {-10,  2}, {-15,-18}, { 11,  0}, {  5,  5},
	{-13,  7}, {-14,  3}, {  6,-13}, {  3,-15}, { -8,  9}, {  1,  8}, {-29, 12}, { -5, -8},
	{  5,  6}, {-15,  3}, { 11, -5}, { -3, 14}, {  1,-13}, { 26, 18}, {-12, -9}, { 14,  0},
	{-11, 28}, {  1,  6}, {  7, -9}, {-14, -4}
	};

	const Rot rotationsGrade_1[] = {
		  { 21,120,137}, { 36, 45,134}, { 37,138, 89}, {155, 96,156}, {107, 21, 47}, { 84, 86,134}, {153,115, 87}, { 32,156,141},
	{  9,165,153}, { 12, 59, 65}, {142, 98, 87}, { 29,139,128}, {100, 98, 13}, { 86,100,126}, { 20, 47,134}, {119,136,102},
	{153, 22,148}, {  3, 63,146}, {174, 14, 73}, {126,143, 75}, {163, 22, 79}, {166, 27, 65}, { 18, 98, 67}, { 58,115,128},
	{ 38, 52,115}, {138, 73, 86}, {152, 37, 19}, {164, 27, 65}, {166,121,  2}, { 75,151,165}, {140,  3,103}, {155, 51,134},
	{ 74,115, 29}, {175,117,124}, {140,161, 19}, { 73, 78,118}, {153,136, 55}, { 66,  4,155}, {115,116,133}, { 28, 79, 76},
	{ 93, 42, 47}, { 75,135, 55}, {125, 52, 64}, { 13,172,116}, {156, 40, 77}, { 84,179, 47}, { 86,129,130}, {146,136, 91},
	{ 74,109, 86}, { 99,167, 12}, {130, 72,167}, {113,159,115}, { 84,105,110}, { 85, 58,175}, {169,104, 58}, { 40, 27, 75},
	{180,107,116}, { 44, 37, 86}, { 46,158,153}, { 36,138, 53}
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

	//70×70
	/*const Pos positionsGrade_2[] = {
		{ -6,-23}, {-24, -1}, {  9,-21}, { 14,-13}, { 26, -1}, {-16, 15}, {-18,-16}, {-15,  8},
    { 16, 27}, { 21,  2}, {-18,-20}, {-14,-15}, {-16,-23}, {  6, 23}, { 15, 16}, { 24, -6},
    { 11,-11}, { -8, 17}, {-25, 12}, {  8,-15}, { -8,-20}, {  0,-24}, {  1,-31}, {-14, 23},
    { -1,-19}, {-32,-32}, {-19, 10}, { 27,  3}, {-19,-23}, { 16, 13}, {-24, 33}, { -3,-13},
    { -9,-16}, {-18, -6}, { 10, 24}, {-12, 19}, {-16, 18}, {-21,  1}, { 14,-31}
	};

	const Rot rotationsGrade_2[] = {
		{ 94, 63, 30}, {151,161,161}, { 77,122,  1}, {151, 88, 67}, { 17,144,  7}, {145, 86, 40}, { 19,  8, 58}, { 13, 97, 26},
    { 93, 15, 11}, {177,178, 61}, { 91, 31,180}, {152, 59, 66}, { 36, 23,132}, { 43,144, 67}, {  9,138,111}, {107, 28, 27},
    { 71,138,  9}, { 85,  3,127}, { 89,174, 64}, {  5,147,164}, { 61, 71, 71}, {137,  2,180}, { 45, 76,  1}, {  3,147, 67},
    { 12,111, 48}, { 46,112, 67}, {142,158, 18}, {159,136, 88}, { 69,141,163}, { 80,177,  5}, {168, 82,147}, { 16,  3,157},
    { 23, 32,106}, {  8,154,138}, { 58,171, 86}, {109, 50,145}, { 20,134, 61}, { 80,112, 43}, { 20, 57, 94}, {101, 51,105}
	};*/


	//60×60
	const Pos positionsGrade_2[] = {
		{-23,  2}, { 16,  6}, { 22, 12}, {-27,  4}, {-10, 15}, {-18, 18}, {  8,-23}, {-16, -5},
	{ 17, 18}, {  2,-25}, {-10,-23}, { 13,-17}, {-13,-11}, { 20,-18}, { 10,-26}, {-21, 12},
	{-17,-26}, { -2, 24}, { -5,-24}, {-26, -6}, {-21,  4}, { 15, -2}, { -5, 22}, { 25,  1},
	{ -5,-27}, {-20,-15}, { 14, 22}, { -8, 14}, { 20,  0}, { 18,-19}, { 12, -9}, { 11,-24},
	{ 23, -6}, {-14, -9}, { -1,-21}, {-12, 12}, { 10, -9}, { -5,-16}, { -4, 26}, {-24, -2}
	};

	const Rot rotationsGrade_2[] = {
	{  5, 78,168}, {131, 49,143}, { 31,  3, 24}, { 49, 12,143}, { 79,134, 29}, {151, 62, 81}, { 42, 79,104}, { 40,149, 62},
	{103, 89, 10}, {  8,172,142}, { 10, 88,159}, {116,163,155}, {103,164,175}, {119,161,143}, {169,149, 99}, {131,136,141},
	{139,119,149}, {160,131,157}, {119,163, 61}, {  8, 43,127}, { 46, 87, 63}, { 58,175,149}, {156, 91,112}, {151, 50,130},
	{ 67,180,126}, { 97,135,154}, { 75,107,115}, {158, 58, 70}, { 16,153, 74}, { 57,136,146}, { 50,  2,126}, {156, 33, 55},
	{ 12,114,113}, {117, 50, 50}, {102, 44,139}, {180, 49,153}, {149,  5,167}, {126, 32,  9}, { 44,178, 36}, { 91,107,108}
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

	//70×70
	/*const Pos positionsGrade_3[] = {
		{  3, 13}, { 11, 28}, { 23,-32}, { 10, 14}, { -8,-33}, { 19, 25}, { 18,-25}, { -7, 21},
    {-15, 26}, { 24, 11}, {-31,-26}, { 19, 17}, { 24, 16}, {-26, -6}, {-25, 17}, { 14, -9},
    {-16,-29}, {-30,  8}, { 29,-23}, {-28, 28}, {-34, 20}, {-11, 31}, {-23,-19}, {-28,  2},
    {-23,-34}, { 23,-24}
	};

	const Rot rotationsGrade_3[] = {
		{ 66, 40, 55}, {180, 70, 70}, { 90, 94,164}, {  7,  8,139}, {165, 43,116}, { 90, 72, 30}, {139, 15,128}, {127,  0, 11},
    {151,161,120}, {  8,115,126}, {145,123, 21}, { 25, 36,155}, {  9, 54, 30}, {125,117, 33}, { 81, 73,106}, { 19, 72, 52},
    {  0,157,150}, {143, 51, 41}, {130, 28, 92}, { 48,  0, 89}, { 10,  8,  0}, { 13, 28, 12}, { 62, 50, 80}, {131,100, 11},
    { 76, 67,137}
	};*/

	//60×60
	const Pos positionsGrade_3[] = {
		 {-29,-12}, { 23,-22}, {-17,  6}, { 14,-11}, { 25,-15}, { 17,-29}, {-21, 24}, {  5,-29},
	{-29,-19}, { 18, 22}, { 27,-23}, {-16, 28}, {-17, 25}, { 27,  4}, {-11, 22}, { 23, 15},
	{ 24, 19}, { 21,-27}, { 23, 28}, { -1, 28}, { 19, 28}, { 25, 25}, { 13, 25}, {-29,-28},
	{-25,-22}, { 16,-26}, { -8, 25}, { 12, 28}, {-28, 22}, { 25,-28}
	};

	const Rot rotationsGrade_3[] = {
		 {116,135,166}, {101, 22, 49}, {135,175, 70}, {100, 28,161}, {  5, 50,135}, { 59, 51,100}, {  6,110,101}, {111,  1, 15},
	{  3,128, 75}, { 63,107,174}, { 77, 90,124}, {  5,119, 81}, { 30,179,171}, {175, 61, 10}, {177,172, 26}, {151, 67, 80},
	{137,118,166}, { 79, 47,124}, {150, 31,  5}, { 21, 98,153}, {170,180,111}, {151,107, 82}, { 91,176,  9}, {128,121, 26},
	{149,156, 55}, {147,162, 12}, { 73, 34, 29}, {141, 31,108}, {172,174,125}, {142, 17, 55}
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


