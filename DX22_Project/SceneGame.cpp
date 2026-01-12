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
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::GlassShard); });


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
    { -8,-0.9,-11}, {  6,-0.6,-12}, { 34,-0.6, -9}, {  5,-0.5,-14}, {11,-0.5, -3}, {-3,-1.2, 13}, {20,-1.1, 39}, {-10,-1.1 ,7},
	 {  5, -0.9,11}, {  1,-0.9, -13}, {  3,-1.1, 14}, { 11,-1.1, 14}, {  3,-1.2,  7}, { -7,-1.2, -5}, {  8,-1.1,  4}, {  6,-1.1,  9},
    {  2,-0.9, 11}, {  8,-0.9,  0}, {  1,-1.1,  9}, {  3,-1.1, 12}, {  8,-1.2,  9}, {-12, -1.2, 4}, {  2,-1.1,-13}, { -8,-1.1,-13},
    {  7,-0.9,  1}, { -2,-0.9, 14}, { -6,-1.1,-27}, { 12,-1.1, -5}, { 10,-1.2,  2}, {  1,-1.2, 12}, {-12,-1.1,  9}, {  6,-1.1,  7},
    {  5,-0.9, -8}, {  5,-0.9,-26}, {  3,-1.1,-14}, { 14,-1.1,  0}, { -9,-1.2,  5}, {  2,-1.2,  9}, { -4,-1.1, -6}, { -8,-1.1, -1},
    { 14,-0.9, -1}, {  7,-0.9, -8}, {  6,-1.1, 10}, { -9,-1.1,  4}, {  6,-1.2, -7}, { -6,-1.2, 12}, {-11,-1.1,  4}, { -6,-1.1, 13},
    {  7,-0.9, -5}, { -5,-0.9, -7}, {  2,-1.1,-15}, { 12,-1.1, -4}, { -7,-1.2, -2}, {-30,-1.2,-25}, { -6,-1.1,  6}, { -3,-1.1,-10},
    {  0,-0.9, -11}, { -2,-0.9,  6}, {  7,-1.1,-11}, {-11,-1.1, -7}
    };

	
	const Rot rotationsGrade_1[] = {
	{ 0,0,0},{-90,0,0}, {  90, 0, 0}, {0, 0, -90},  {  0, 0, 90}, {-15, 0, -15}, {30, 0,30}, {-30, 0,-30},
	{ 45,0,0},{-45,0,0}, {  0, 0, 45}, {0, 0, -45},  {  15, 0, 15}, {-15, 0, -15}, {30, 0,30}, {-30, 0,-30},
	{  45,0,0},{-45,0,0}, {  0, 0, 45}, {0, 0, -45},  {  15, 0, 15}, {-15, 0, -15}, {30, 0,30}, {-30, 0,-30},
	{  45,0,0},{-45,0,0}, {  0, 0, 45}, {0, 0, -45},  {  15, 0, 15}, {-15, 0, -15}, {30, 0,30}, {-30, 0,-30},
	{  45,0,0},{-45,0,0}, {  0, 0, 45}, {0, 0, -45},  {  15, 0, 15}, {-15, 0, -15}, {30, 0,30}, {-30, 0,-30},
	{  45,0,0},{-45,0,0}, {  0, 0, 45}, {0, 0, -45},  {  15, 0, 15}, {-15, 0, -15}, {30, 0,30}, {-30, 0,-30},
	{  45,0,0},{-45,0,0}, {  0, 0, 45}, {0, 0, -45},  {  15, 0, 15}, {-15, 0, -15}, {30, 0,30}, {-30, 0,-30},
	{  45,0,0},{-45,0,0}, {  0, 0, 45}, {0, 0, -45}
	};


	//--------------------------------------
	// Grade 2
	//--------------------------------------

	const Pos positionsGrade_2[] = {

	{ -8,-0.9, 17}, {-21,-0.4,  0}, {-27,-0.4,  8}, { -9,-0.4,-21}, {-17,-0.4,-21}, { 12,-1.0,-16}, {-27,-1.0, 15},
	{  7,-0.9,-15},{-10,-0.5,-25}, { 14,-0.5, -7}, {  0,-0.5,-16}, { 11, -0.5, 9}, { 18,-1.4, -6}, { 15,-1.4,-13},
	{ -6,-0.9,-33}, { 16,-0.5,  3},{-15,-0.5,-15}, {  6,-0.5, 18}, {-22,-0.5,-11}, { 10,-1.4,-18}, { 21,-1.4,-18},
	{ 15,-0.9,-11}, { -5,-0.5,-18}, { 25,-0.5,  3},{  6,-0.5, 22}, { -6,-0.5,-24}, {  8,-1.4, 22}, { 19,-1.4,  9},
	{ 33,-0.9,-17}, {-25,-0.5,  8}, {-12,-0.5, 26}, {-29,-0.5,-16},{  3,-0.5, 25}, {-16,-1.4,  6}, { 21,-1.4, 15},
	{-12,-0.9,-13}, {-14,-0.5,-18}, {-25,-0.5, -8}, {-28,-0.5, -1}, {-27,-0.5,  2},{ 13,-1.4, -9}, {-20,-1.4,-10},
	{-20,-0.9, -2}, { 24,-0.5, -7}, {  1,-0.5,-21}, {-21,-0.5, 18}, {-20,-0.5,  9}, { 14,-1.4,-22},	{-17,-1.4, 18},
	{-10,-0.9, 16}
	};

	const Rot rotationsGrade_2[] = {
	{  0,0,0}, {90,0,0},  {  0, 0, 90}, {90,0,45},  {  45, 0, 90},{  0,0,10}, {  0,0,-10}, 
	{  0,0,0}, {90,0,0},  {  0, 0, 90}, {90,0,45},  {  45, 0, 90},{  0,0,10}, {  0,0,-10},
	{  0,0,0}, {90,0,0},  {  0, 0, 90}, {90,0,45},  {  45, 0, 90},{  0,0,10}, {  0,0,-10},
	{  0,0,0}, {90,0,0},  {  0, 0, 90}, {90,0,45},  {  45, 0, 90},{  0,0,10}, {  0,0,-10},
	{  0,0,0}, {90,0,0},  {  0, 0, 90}, {90,0,45},  {  45, 0, 90},{  0,0,10}, {  0,0,-10},
	{  0,0,0}, {90,0,0},  {  0, 0, 90}, {90,0,45},  {  45, 0, 90},{  0,0,10}, {  0,0,-10},
	{  0,0,0}, {90,0,0},  {  0, 0, 90}, {90,0,45},  {  45, 0, 90},{  0,0,10}, {  0,0,-10},
	{  0,0,0}
	};

	//--------------------------------------
	// Grade 3
	//--------------------------------------

	const Pos positionsGrade_3[] = {
	{-29,2,-25}, { 28,2,-23}, { 12,-0.9,-32}, { 17,3, 20}, { -3,3, 36}, 
	{-33,1,-38}, { 22,1,-21}, { 19,-0.9,-30},{-28,2, 17}, {-33, 2,-5},
	{ 30,1, 11}, {  9,1, -8}, { 27,-0.9, 17}, { -8,2, 37}, { 15,2, 33},
	{ 21,1, 29},{-31,1,  9}, {  1, -0.9,27}, { 31,2, 21}, {-25,2,-21},
	{ 28,1,-13}, { 30,1,-18}, { 36,-0.9, -2}, {-15,2, 34},{ 23,2, 21},
	{ 17,1, 15}, {  6,1, 27}, {-32,-0.9,-10}, { 23,2, 24}, { 29,2,  6},
	{ -5,1,-36}, { 32,1,  3},{-17,-0.9, 25}, { 33,2, 11}, {-26,2,-17}
	};

	const Rot rotationsGrade_3[] = {
	{125,  0, 0}, {0,  0, 125}, {0,  0, 0}, {0,  0, 150}, {150,  0, 0}, 
    {125,  0, 0}, {0,  0, 125}, {0,  0, 0}, {0,  0, 150}, {150,  0, 0},
	{125,  0, 0}, {0,  0, 125}, {0,  0, 0}, {0,  0, 150}, {150,  0, 0},
	{125,  0, 0}, {0,  0, 125}, {0,  0, 0}, {0,  0, 150}, {150,  0, 0},
	{125,  0, 0}, {0,  0, 125}, {0,  0, 0}, {0,  0, 150}, {150,  0, 0},
	{125,  0, 0}, {0,  0, 125}, {0,  0, 0}, {0,  0, 150}, {150,  0, 0},
	{125,  0, 0},{ 0,  0, 125 }, {0,  0, 0}, {0,  0, 150}, {150,  0, 0}
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

void SceneGame::CreateGlassObjectSpawner()
{
	World& w = GetWorld();

	GameObject& glassObjectSpawner = w.CreateGameObject();
	//glassObjectSpawner.AddComponent<GlassObjectSpawner>();

	GameObject& Frost = w.CreateGameObject();
	//Frost.AddComponent<Sprite3DRenderer>(TextureTag::UIFrostNoise, Vector2::zero(), Vector2(12.8f, 7.2f));
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


