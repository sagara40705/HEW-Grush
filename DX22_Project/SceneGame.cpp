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

	GameObject& cameraPivot = w.CreateGameObject(Vector3::zero(), Vector3(0.f, 180.f, 0.f), Vector3::one());
	cameraPivot.SetTag(Tag::CamreraPivot);
	cameraPivot.AddComponent<CameraControl>();

	GameObject& mainCamera = cameraPivot.CreateChild(Camera::kInitPos, Camera::kInitRotate, Vector3::one());
	mainCamera.SetTag(Tag::MainCamera);
	mainCamera.AddComponent<Camera>();
	mainCamera.AddComponent<CameraShake>();

	GameObject& player = w.CreateGameObject(Vector3(-10.f, 2.f, 33.f), Vector3(0.f, 0.f, 0.f));
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
	GameObject& floor = w.CreateGameObject(Vector3(0.f, -1.f, 0.f), Vector3::zero(), Vector3(80.f, 0.1f, 80.f));
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

	const Pos positionsGrade_1[] = {
		{-20,1}, {29,7}, {-38,-34}, {31,-33}, {-12,-25}, {26,9}, {-13,13}, {1,-19},
		{-22,-14}, {11,-4}, {35,-12}, {-15,-10}, {6,-1}, {37,7}, {6,-35}, {28,14},
		{27,-22}, {-8,-4}, {32,5}, {23,16}, {28,-27}, {-17,-2}, {-18,-18}, {32,-18},
		{-39,13}, {-28,0}, {-35,12}, {-24,-10}, {-34,-7}, {6,12}, {0,5}, {22,-15},
		{34,-24}, {1,-9}, {-10,-16}, {-23,2}, {-17,-9}, {-16,-36}, {31,-3}, {-4,-30},
		{-16,1}, {5,6}, {38,-7}, {24,-37}, {-30,-2}, {-20,-8}, {10,2}, {-8,7},
		{-37,-22}, {6,-11}, {-11,36}, {-21,20}, {-2,22}, {-22,34}, {26,20}, {-1,28},
		{-20,17}, {33,29}, {2,37}, {14,19}, {-26,15}, {21,33}, {1,31}, {27,35},
		{1,23}, {30,23}, {-33,34}, {33,20}, {-1,25}, {-29,35}, {-5,11}, {-36,18},
		{-6,13}, {35,10}, {-5,37}, {-6,23}, {20,17}, {10,23}, {22,19}, {32,10},
		{-18,26}, {10,30}, {13,29}, {-8,21}, {-36,15}, {4,35}, {25,13}, {30,32},
		{37,24}, {-13,34}, {-40,16}, {-39,10}, {-15,25}, {28,22}, {-16,21}, {-25,35},
		{-33,26}, {13,26}, {-9,16}, {-4,19}, {3,31}, {-10,14}, {12,18}, {8,33},
		{11,16}, {-12,30}, {-39,31}, {-37,34}, {33,18}, {11,33}, {19,33}, {32,14},
		{5,15}, {-27,35}, {17,17}, {16,28}, {-28,18}, {-5,9}, {-24,21}, {-28,32},
		{-9,29}, {-35,9}, {19,27}, {-40,36}, {6,27}, {27,31}, {-36,23}, {-33,9},
		{4,19}, {-26,20}, {-22,30}, {15,9}, {16,31}, {-5,35}, {-19,21}, {10,19},
		{37,34}, {-32,12}, {8,25}, {38,36}, {-9,35}, {0,18}, {6,30}, {-34,28},
		{-13,37}, {-30,20}, {24,32}, {-29,25}, {-6,21}, {33,22}, {-15,12}, {33,27},
		{9,35}, {21,24}, {-25,31}, {14,31}, {14,33}, {14,22}, {36,21}, {-27,23},
		{-34,17}, {20,12}, {35,18}, {-18,36}, {32,35}, {-29,37}, {-17,34}, {-20,36},
		{1,26}, {-11,17}
	};

	const Rot rotationsGrade_1[] = {
		{109,166,20}, {122,41,85}, {81,114,170}, {178,128,103}, {52,82,36}, {118,164,20},
		{147,180,179}, {21,126,166}, {130,155,80}, {69,49,150}, {112,155,8}, {8,3,30},
		{45,95,30}, {111,106,63}, {152,147,153}, {9,55,160}, {150,21,159}, {100,135,86},
		{139,85,13}, {112,108,88}, {46,157,121}, {96,79,113}, {57,150,16}, {166,94,70},
		{151,51,81}, {70,124,162}, {161,32,57}, {112,172,35}, {101,68,104}, {110,28,144},
		{73,54,27}, {40,168,67}, {124,128,167}, {156,2,161}, {67,25,174}, {60,165,0},
		{68,174,114}, {90,33,14}, {172,154,161}, {151,4,139}, {66,97,176}, {103,59,124},
		{61,168,7}, {101,145,126}, {148,7,148}, {121,30,42}, {65,69,63}, {67,163,91},
		{104,133,179}, {29,144,33}, {81,90,3}, {176,14,69}, {130,96,156}, {164,64,34},
		{62,59,161}, {114,103,124}, {160,90,7}, {12,21,176}, {161,15,114}, {130,164,54},
		{163,162,88}, {11,18,174}, {167,83,157}, {123,158,109}, {15,83,165}, {112,92,111},
		{46,62,25}, {133,69,129}, {0,3,78}, {153,87,141}, {150,135,18}, {131,28,55},
		{67,107,82}, {135,39,127}, {170,61,116}, {72,144,20}, {120,76,73}, {95,159,146},
		{128,20,109}, {61,18,139}, {97,163,106}, {18,170,147}, {27,124,5}, {179,115,7},
		{165,128,88}, {151,42,20}, {154,66,12}, {166,72,62}, {135,125,79}, {9,90,106},
		{118,134,102}, {61,4,44}, {81,17,119}, {154,59,79}, {108,169,1}, {2,124,122},
		{134,83,63}, {3,148,65}, {49,25,163}, {180,50,79}, {176,88,82}, {156,151,53},
		{6,0,35}, {179,131,167}, {171,3,52}, {109,9,115}, {105,80,107}, {2,124,119},
		{158,26,13}, {2,131,95}, {112,148,121}, {8,115,148}, {122,51,3}, {7,56,77},
		{103,82,98}, {99,111,85}, {36,165,88}, {25,87,161}, {132,27,53}, {70,100,169},
		{45,179,90}, {16,3,170}, {167,37,12}, {40,176,49}, {37,176,42}, {166,179,106},
		{54,174,90}, {134,6,179}, {173,55,67}, {138,46,1}, {102,1,0}, {168,59,110},
		{170,4,14}, {150,54,138}, {165,4,116}, {37,18,134}, {59,81,172}, {44,98,16},
		{162,3,43}, {3,36,174}, {39,113,116}, {73,0,60}, {82,140,109}, {113,45,141},
		{77,34,16}, {117,173,42}, {89,25,163}, {87,67,39}, {118,18,35}, {125,129,6},
		{35,54,156}, {103,146,53}, {75,81,129}, {81,20,32}, {169,28,24}, {39,172,21},
		{77,169,51}, {48,121,68}, {100,105,111}, {25,101,125}, {4,163,70}, {54,38,103},
		{160,105,7}, {12,3,37}, {89,94,123}, {120,85,26}, {1,153,174}, {62,168,69},
		{4,49,111}, {127,124,178}
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

	const Pos positionsGrade_2[] = {
		{-31,-36}, {-19,-32}, {18,-24}, {-38,-23}, {-16,-36}, {-4,-17}, {-13,-26}, {-19,-18},
		{0,-19}, {32,-18}, {-9,-38}, {-14,-31}, {27,-14}, {36,-35}, {31,-34}, {-1,-27},
		{-34,-21}, {28,-28}, {-22,-25}, {-11,-17}, {5,-35}, {-34,-28}, {-28,-29}, {-39,-35},
		{-5,-31}, {-6,-22}, {24,-38}, {17,-28}, {27,-22}, {33,-25}, {17,11}, {-31,-11},
		{-32,0}, {-30,12}, {10,-4}, {33,7}, {-21,-8}, {0,12}, {-23,15}, {5,-11},
		{-8,-4}, {37,-7}, {16,-5}, {21,-15}, {-20,1}, {-15,-11}, {6,11}, {-39,12},
		{-5,4}, {20,-7}, {-18,-3}, {36,7}, {32,4}, {-23,2}, {-27,-11}, {27,14},
		{-38,0}, {-22,-15}, {-12,-3}, {-5,-6}, {-1,-15}, {-17,0}, {-28,0}, {-35,12},
		{35,-12}, {9,2}, {-5,-11}, {1,-12}, {8,-12}, {-35,-8}, {-16,9}, {-18,-9},
		{31,-3}, {6,-2}, {23,-9}, {-13,13}, {-8,6}, {23,-12}, {28,6}, {21,2},
		{-30,-3}, {-3,14}, {-6,15}, {22,16}, {37,14}, {4,5}, {14,13}, {0,-9},
		{21,-2}, {17,5}, {35,1}, {-28,9}, {22,7}, {35,-2}, {25,8}, {-24,11},
		{0,4}, {-24,-11}, {15,0}, {4,-14}
	};

	const Rot rotationsGrade_2[] = {
		{83,135,164}, {108,125,165}, {99,131,3}, {122,138,88}, {67,171,2}, {20,171,39},
		{151,71,163}, {98,93,125}, {80,45,15}, {0,99,23}, {129,112,26}, {65,3,81},
		{20,58,128}, {147,28,52}, {108,149,130}, {37,111,68}, {174,130,144}, {17,127,105},
		{180,173,155}, {149,59,1}, {68,29,174}, {126,116,39}, {129,175,91}, {127,5,154},
		{71,0,55}, {160,122,108}, {45,31,150}, {165,147,68}, {166,89,118}, {57,64,122},
		{123,114,78}, {133,155,16}, {168,58,128}, {157,44,15}, {44,20,7}, {84,26,133},
		{40,13,42}, {24,106,145}, {16,72,34}, {141,136,99}, {67,25,51}, {114,102,156},
		{129,12,171}, {118,127,2}, {79,135,21}, {150,2,154}, {128,119,157}, {114,14,94},
		{139,142,17}, {46,129,100}, {38,86,5}, {141,32,166}, {35,99,54}, {0,180,169},
		{165,56,91}, {41,89,31}, {49,112,157}, {3,111,15}, {81,168,31}, {26,17,150},
		{84,136,41}, {126,155,116}, {130,97,20}, {133,20,163}, {171,149,105}, {26,79,162},
		{137,38,163}, {110,93,34}, {55,18,64}, {62,132,122}, {131,15,135}, {51,72,71},
		{4,63,74}, {107,139,42}, {67,28,173}, {118,84,22}, {21,67,49}, {50,4,153},
		{113,114,30}, {144,77,115}, {175,64,82}, {90,35,70}, {89,114,23}, {119,156,135},
		{143,161,78}, {126,108,1}, {94,83,144}, {51,94,123}, {152,92,172}, {81,151,163},
		{68,80,135}, {122,164,140}, {130,171,10}, {135,106,156}, {152,103,179}, {53,153,116},
		{22,68,121}, {166,144,147}, {146,24,128}, {9,55,156}
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

	const Pos positionsGrade_3[] = {
		{15,-30}, {-24,-25}, {-20,-32}, {-1,-21}, {-24,-20}, {-6,-17}, {-7,-24}, {9,-34},
		{28,-29}, {21,-20}, {23,-29}, {-15,-27}, {-34,-22}, {-21,-38}, {-39,-24}, {-40,-35},
		{-6,-33}, {29,-36}, {-13,-17}, {31,-18}, {-19,-19}, {-16,-38}, {33,-26}, {4,-37},
		{-33,-36}, {-28,-39}, {26,-16}, {34,-35}, {-28,-30}, {18,-35}, {-14,-32}, {7,-17},
		{-29,-19}, {-36,-17}, {27,-24}, {-34,-29}, {18,-25}, {-9,-39}, {23,-39}, {-1,-27}
	};

	const Rot rotationsGrade_3[] = {
		{84,48,43}, {138,175,48}, {161,32,130}, {96,163,65}, {103,56,26}, {27,93,77},
		{24,95,31}, {29,95,73}, {147,31,72}, {38,34,81}, {39,162,107}, {160,118,142},
		{178,3,44}, {175,96,76}, {134,169,126}, {48,105,109}, {34,103,75}, {144,51,14},
		{130,48,118}, {176,163,118}, {116,75,118}, {53,81,175}, {2,129,49}, {59,134,18},
		{53,81,161}, {11,114,67}, {52,80,156}, {180,93,88}, {126,78,52}, {47,38,73},
		{73,71,11}, {9,76,68}, {49,10,109}, {26,72,143}, {37,10,169}, {16,78,9},
		{28,138,1}, {11,21,30}, {101,105,148}, {62,21,155}
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


