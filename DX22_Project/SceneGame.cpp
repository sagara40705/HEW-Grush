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
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Spin); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::SpinMono); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Dash); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::DashMono); });

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

	GameObject& floor = w.CreateGameObject(Vector3(0.f, -1.f, 0.f), Vector3::zero(), Vector3(100.f, 0.1f, 100.f));
	MeshFilter& mf = floor.AddComponent<MeshFilter>(PrimitiveMeshTag::Cube); mf.boundsRadius = 0.f;
	floor.AddComponent<MeshRenderer>();
	BoxCollider& collider = floor.AddComponent<BoxCollider>();
	collider.layer = Layer::Ground;

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
		PrefabId::Akican,
		PrefabId::Desktop,
		PrefabId::Gomibukuro,
		PrefabId::Isu,
		PrefabId::Kuukseizixyoukii,
		PrefabId::Ningixyou
	};

	const Pos positionsGrade_1[] = {
		{19,48}, {46,26}, {36,35}, {11,33}, {-4,24}, {20,-37}, {-9,-27}, {14,-43},
		{-38,-7}, {21,-49}, {-30,33}, {-28,6}, {34,8}, {8,-47}, {-28,29}, {43,40},
		{-3,-31}, {-47,-13}, {-15,-29}, {-20,-29}, {16,-44}, {42,-14}, {26,28}, {30,5},
		{-19,46}, {-34,-41}, {-4,19}, {27,-24}, {19,-14}, {26,-17}, {26,-34}, {47,-36},
		{31,29}, {-22,8}, {33,22}, {-13,-40}, {-10,41}, {-41,3}, {35,-13}, {-13,-9},
		{22,-29}, {12,7}, {-48,-3}, {22,17}, {-18,6}, {-22,-11}, {-14,19}, {10,28},
		{16,16}, {-12,25}, {-28,-46}, {24,33}, {41,-33}, {42,-49}, {-25,-37}, {18,43},
		{17,-48}, {-49,-25}, {-31,26}, {36,-21}, {-44,-11}, {25,-39}, {-8,48}, {47,20},
		{7,-43}, {-44,-31}, {48,-8}, {-45,41}, {47,-16}, {-7,-46}, {-42,6}, {-29,44},
		{-46,43}, {48,8}, {-45,20}, {-50,-15}, {42,16}, {-41,46}, {-32,39}, {-14,-45},
		{41,-39}, {-12,46}, {-2,-49}, {-44,-27}, {-48,11}, {44,6}, {44,24}, {39,-35},
		{-49,-32}, {-9,-46}, {-47,-47}, {34,27}, {-36,-44}, {-43,-6}, {-19,43}, {-44,-15},
		{3,42}, {46,-6}, {27,-36}, {-6,-43}, {29,44}, {-33,-31}, {24,48}, {-47,28},
		{-43,-33}, {42,37}, {38,-12}, {38,-23}, {35,23}, {-10,44}, {37,46}, {35,46},
		{29,48}, {-41,39}, {-24,-45}, {23,-48}, {-41,-37}, {43,-7}, {39,22}, {-47,6},
		{46,-38}, {44,-48}, {-12,40}, {46,29}, {38,40}, {-38,25}, {-46,-49}, {42,-24},
		{33,-31}, {30,-34}, {-15,48}, {46,-13}, {33,-44}, {14,48}, {20,-41}, {-45,-42},
		{-32,-34}, {40,-23}, {-39,-35}, {-33,-25}, {17,46}, {-3,48}, {-42,-26}, {-2,-43},
		{-42,-44}, {-25,42}, {5,40}, {-6,45}, {41,-44}, {39,18}, {44,-5}, {39,32},
		{34,-46}, {42,-42}, {13,-47}, {41,-27}, {-44,6}, {23,-36}, {-49,-12}, {2,-42},
		{-46,34}, {45,-17}, {-36,-30}, {3,48}, {-47,-27}, {31,-42}, {-25,40}, {-40,-45},
		{-17,42}, {46,-42}, {-49,47}, {-34,46}, {23,-42}, {-22,41}, {-44,37}, {-50,-9},
		{-1,46}, {-42,22}, {37,43}, {22,33}, {-50,9}, {-50,-37}, {-22,37}, {38,37},
		{-31,28}, {46,-45}, {-48,32}, {37,-50}, {-49,20}, {48,-21}, {32,42}, {34,-23},
		{30,39}, {39,-21}, {-45,-7}, {-47,-35}, {-15,-40}, {45,-34}, {30,42}, {-39,39}
	};

	const Rot rotationsGrade_1[] = {
		{114,168,160}, {87,149,140}, {75,99,162}, {14,106,100}, {107,151,56}, {60,130,44}, {36,161,51}, {12,76,28},
		{62,104,114}, {168,81,178}, {180,27,5}, {55,59,10}, {11,20,78}, {169,155,32}, {83,104,91}, {70,158,136},
		{87,38,45}, {151,27,145}, {100,144,157}, {60,119,83}, {100,152,50}, {103,62,176}, {46,70,82}, {27,167,4},
		{119,127,113}, {30,55,56}, {82,80,141}, {145,25,65}, {19,118,106}, {21,62,120}, {130,86,55}, {58,123,102},
		{143,129,6}, {169,148,177}, {131,105,29}, {99,117,156}, {114,163,81}, {28,172,50}, {119,67,22}, {10,3,40},
		{74,100,35}, {14,32,145}, {175,116,73}, {45,79,85}, {26,0,76}, {74,127,39}, {47,141,101}, {179,13,112},
		{129,109,45}, {47,146,173}, {52,173,74}, {129,71,67}, {105,151,154}, {132,155,15}, {94,136,96}, {75,179,47},
		{58,170,38}, {161,106,117}, {8,33,19}, {117,84,55}, {47,5,147}, {11,74,49}, {91,149,0}, {166,150,79},
		{119,33,81}, {57,20,30}, {118,148,83}, {139,95,56}, {149,118,95}, {175,36,163}, {130,164,16}, {154,103,18},
		{105,7,46}, {54,58,141}, {171,98,12}, {142,35,54}, {30,6,5}, {78,0,167}, {7,36,122}, {9,12,82},
		{48,102,103}, {64,172,73}, {169,16,94}, {127,94,157}, {57,121,84}, {72,13,4}, {143,141,30}, {86,0,151},
		{67,27,87}, {60,112,34}, {151,77,72}, {176,24,65}, {130,165,94}, {121,126,85}, {153,43,79}, {169,11,94},
		{145,175,64}, {174,142,117}, {162,101,98}, {120,116,168}, {9,13,40}, {114,8,20}, {69,47,40}, {73,12,1},
		{28,144,146}, {10,32,154}, {120,87,45}, {106,177,133}, {178,102,17}, {115,66,135}, {157,16,64}, {68,24,67},
		{26,172,83}, {137,3,133}, {43,173,35}, {69,158,15}, {3,138,176}, {25,32,108}, {122,93,34}, {122,74,89},
		{172,105,97}, {33,152,155}, {97,170,20}, {41,158,19}, {108,140,38}, {122,122,169}, {74,128,12}, {88,59,126},
		{120,117,136}, {172,137,149}, {16,129,49}, {167,34,21}, {78,120,115}, {38,91,139}, {180,14,118}, {83,12,88},
		{35,121,35}, {103,129,116}, {62,55,106}, {160,20,5}, {3,9,111}, {68,110,43}, {156,36,52}, {81,115,9},
		{70,99,146}, {139,153,53}, {130,114,23}, {158,172,129}, {96,74,19}, {119,180,119}, {85,55,166}, {84,176,22},
		{119,118,158}, {48,177,91}, {110,16,134}, {38,129,65}, {175,177,98}, {41,151,68}, {92,66,56}, {31,142,137},
		{111,41,65}, {22,137,140}, {32,139,116}, {27,49,100}, {68,20,28}, {135,111,162}, {83,37,176}, {17,96,1},
		{89,36,109}, {180,36,132}, {152,61,65}, {22,17,62}, {165,55,34}, {126,2,10}, {85,11,159}, {81,43,36},
		{162,4,23}, {151,96,147}, {28,48,156}, {58,104,31}, {136,160,70}, {85,178,144}, {90,157,145}, {20,178,23},
		{21,45,33}, {143,158,38}, {53,31,169}, {122,132,148}, {175,114,176}, {128,127,82}, {166,3,137}, {146,1,86},
		{0,94,128}, {92,169,159}, {137,145,64}, {122,119,94}, {125,63,52}, {40,47,174}, {35,176,118}, {2,76,72}
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
		PrefabId::Aircon,
		PrefabId::Jihanki,
		PrefabId::Reizouko,
	};

	const Pos positionsGrade_2[] = {
		{-4,-31}, {11,7}, {-18,5}, {-16,-30}, {-31,12}, {-38,2}, {-12,24}, {12,-6},
		{-14,18}, {-29,-26}, {11,15}, {-36,9}, {17,-31}, {-21,19}, {-22,-11}, {-39,-25},
		{5,22}, {-2,4}, {-39,18}, {-7,32}, {33,8}, {16,15}, {-13,8}, {3,-33},
		{-4,18}, {8,-16}, {-38,-8}, {38,9}, {25,-18}, {-14,-10}, {-10,-21}, {-20,-22},
		{-13,34}, {-9,-28}, {19,-45}, {-44,15}, {-35,-6}, {42,40}, {-32,-5}, {35,-13},
		{29,-28}, {26,-34}, {-34,-41}, {39,24}, {-44,-40}, {20,-50}, {-29,1}, {-37,27},
		{26,28}, {19,-37}, {-41,2}, {39,-3}, {-18,32}, {-25,20}, {-14,31}, {-13,-41},
		{-32,-46}, {-20,-30}, {30,4}, {26,-24}, {-19,-43}, {-29,5}, {41,8}, {45,-20},
		{-50,0}, {-13,-36}, {17,32}, {30,-32}, {-45,23}, {-49,-4}, {-4,-46}, {45,26},
		{38,28}, {19,47}, {-19,45}, {9,27}, {35,35}, {22,16}, {47,-36}, {18,-14},
		{42,-14}, {32,22}, {30,28}, {-44,-37}, {10,33}, {22,-30}, {-2,-38}, {-45,-45},
		{-9,35}, {-31,32}, {-15,-24}, {-1,-30}, {-10,41}, {37,-43}, {-4,24}, {-28,29},
		{-47,-13}, {42,-3}, {23,39}, {41,5}, {16,-44}, {37,-31}, {13,-44}, {-19,9},
		{7,-47}, {18,-10}, {-23,7}, {-26,14}, {23,23}, {10,36}
	};

	const Rot rotationsGrade_2[] = {
		{116,170,61}, {134,73,100}, {147,91,30}, {56,49,3}, {62,4,12}, {27,100,92}, {165,20,27}, {74,179,47},
		{39,73,76}, {75,148,1}, {15,13,61}, {171,157,123}, {140,145,124}, {93,38,11}, {60,28,151}, {137,180,82},
		{139,20,5}, {146,1,121}, {142,177,73}, {53,63,4}, {62,175,22}, {66,35,48}, {148,158,75}, {37,1,5},
		{177,18,99}, {140,92,53}, {160,51,70}, {126,139,133}, {65,25,167}, {128,70,36}, {158,150,109}, {82,67,64},
		{77,110,110}, {180,54,2}, {166,8,36}, {129,122,82}, {50,15,81}, {9,17,115}, {100,83,65}, {56,49,109},
		{93,180,133}, {14,142,47}, {2,104,123}, {11,50,31}, {101,70,93}, {77,97,91}, {31,82,17}, {76,170,104},
		{8,142,155}, {69,130,73}, {126,121,101}, {154,113,16}, {103,10,41}, {40,49,163}, {4,38,4}, {5,132,2},
		{0,96,164}, {23,139,7}, {17,124,180}, {31,179,161}, {65,149,121}, {148,76,168}, {15,17,142}, {96,65,158},
		{80,127,89}, {178,141,72}, {3,81,88}, {163,93,152}, {152,91,71}, {107,74,66}, {20,171,5}, {153,155,27},
		{126,118,92}, {173,61,53}, {85,103,122}, {46,177,152}, {132,3,48}, {139,165,114}, {72,76,38}, {169,128,73},
		{28,5,74}, {147,165,108}, {112,140,113}, {27,18,37}, {171,123,24}, {73,127,8}, {180,11,6}, {34,16,78},
		{180,21,74}, {0,170,145}, {7,123,98}, {145,100,115}, {54,150,8}, {144,94,119}, {81,123,43}, {71,62,59},
		{118,153,67}, {2,35,138}, {126,85,177}, {179,78,163}, {92,57,114}, {175,131,130}, {72,2,6}, {139,171,60},
		{139,44,178}, {111,30,88}, {144,41,80}, {66,18,52}, {54,160,104}, {155,142,158}

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
		PrefabId::Eisei,
	};

	const Pos positionsGrade_3[] = {
		{-40,-25}, {21,0}, {20,8}, {-32,11}, {7,41}, {-6,17}, {-24,-13}, {36,9},
		{28,-4}, {-10,-40}, {-15,-10}, {-31,-28}, {-12,22}, {-38,1}, {17,-20}, {2,-34},
		{-22,17}, {10,14}, {-6,-31}, {-40,-10}, {5,20}, {-8,30}, {-38,7}, {-15,7},
		{-15,17}, {15,36}, {-42,-17}, {-17,-32}, {23,-18}, {6,-16}, {16,-31}, {16,15},
		{-39,17}, {11,-6}, {-2,4}, {31,8}, {-20,4}, {-3,36}, {10,6}, {29,-12}
	};

	const Rot rotationsGrade_3[] = {
		{8,74,158}, {77,127,152}, {4,110,95}, {167,63,116}, {29,107,60}, {3,29,167}, {65,85,50}, {116,33,25},
		{125,68,113}, {140,135,55}, {160,108,51}, {131,128,142}, {76,26,169}, {51,61,128}, {156,91,77}, {83,59,165},
		{0,171,53}, {148,43,65}, {47,116,94}, {70,175,44}, {153,129,125}, {115,88,115}, {149,127,19}, {120,154,11},
		{21,8,78}, {146,11,51}, {97,85,153}, {76,118,105}, {81,9,23}, {11,115,149}, {2,97,48}, {152,30,43},
		{130,47,7}, {93,86,35}, {31,64,93}, {89,4,153}, {101,48,42}, {14,97,117}, {57,37,94}, {77,154,0}
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

void SceneGame::Update()
{
}

void SceneGame::Draw()
{
}


