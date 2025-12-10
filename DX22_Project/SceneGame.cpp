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

	GameObject& player = w.CreateGameObject(Vector3(-23.f, 2.f, 33.f), Vector3(0.f, 0.f, 0.f));
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

	GameObject& floor = w.CreateGameObject(Vector3(0.f, -1.f, 0.f), Vector3::zero(), Vector3(80.f, 0.1f, 80.f));
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

	Pos positionsGrade_1[] = {
		{-2,18}, {19,15}, {-7,-22}, {11,-23}, {-20,-3}, {13,4}, {-16,-19}, {14,0},
{-4,20}, {22,-9}, {24,0}, {23,24}, {8,9}, {-21,-16}, {-3,-7}, {10,4},
{-9,16}, {-20,4}, {-15,22}, {4,-6}, {-11,-23}, {5,7}, {-23,-7}, {-18,-24},
{0,11}, {-10,-14}, {16,21}, {18,2}, {-12,12}, {-16,1}, {-7,-10}, {-16,-5},
{23,6}, {2,-20}, {8,-10}, {14,-21}, {21,-4}, {-8,-17}, {1,6}, {-9,22},
{-16,-10}, {-11,-6}, {19,8}, {8,-23}, {18,23}, {11,22}, {-4,14}, {-12,19},
{-22,9}, {-17,-7}, {2,15}, {24,-22}, {-5,6}, {-23,11}, {8,19}, {11,-20},
{3,8}, {-7,5}, {10,11}, {-22,-18}, {5,11}, {20,-11}, {-23,-11}, {-10,-16},
{-8,18}, {-9,6}, {-20,-22}, {-9,-8}, {10,18}, {22,-24}, {5,-18}, {7,5},
{-3,-17}, {14,7}, {6,-5}, {-20,8}, {24,-15}, {-3,-23}, {-17,-2}, {-10,19},
{23,-6}, {10,-17}, {20,-2}, {-6,-13}, {-23,24}, {12,7}, {18,-8}, {-2,-21},
{3,-13}, {21,-13}, {22,-15}, {24,-8}, {-3,-19}, {24,2}, {21,12}, {15,24},
{-5,-17}, {21,24}, {0,-6}, {-17,24}, {8,11}, {3,-9}, {-6,18}, {11,-13},
{-22,-24}, {-2,22}, {1,-12}, {24,-20}, {7,-7}, {-5,-7}, {24,20}, {10,-15},
{0,-8}, {-19,-11}, {24,-24}, {23,-18}, {-24,-24}, {9,-21}, {5,5}, {-4,12},
{24,4}, {24,12}, {-13,-21}, {-24,17}, {-21,-11}, {6,-24}, {-1,15}, {7,-21},
{3,24}, {-11,16}, {14,-24}, {-24,-3}, {-17,22}, {-9,-11}, {-18,-5}, {-10,-21},
{5,-10}, {-5,-15}, {-24,0}, {-3,-15}, {8,22}, {-21,11}, {17,-6}, {-5,-5},
{19,5}, {-7,-5}, {-20,6}, {-4,18}, {-18,-13}
	};

	Rot rotationsGrade_1[] = {
		{163,53,174}, {137,97,32}, {60,40,51}, {128,10,48}, {22,154,24}, {104,36,63}, {77,30,49},
{155,75,47}, {48,97,45}, {170,88,155}, {115,74,79}, {151,55,113}, {14,131,37}, {10,83,166},
{54,28,73}, {122,156,125}, {34,136,142}, {179,64,48}, {130,45,161}, {112,176,146}, {46,73,160},
{61,5,35}, {152,85,165}, {147,152,143}, {55,147,73}, {32,165,97}, {135,45,133}, {128,138,76},
{176,48,71}, {41,156,160}, {133,176,2}, {21,92,131}, {155,141,170}, {104,28,111}, {87,176,173},
{104,98,87}, {179,110,95}, {50,145,57}, {149,122,175}, {138,107,11}, {95,1,5}, {8,123,149},
{25,30,169}, {61,47,108}, {20,47,173}, {19,30,83}, {75,100,70}, {6,158,81}, {56,39,71},
{87,50,144}, {174,179,24}, {115,24,156}, {146,99,97}, {170,8,82}, {68,72,8}, {99,30,45},
{114,151,31}, {12,41,100}, {49,154,103}, {87,62,7}, {88,38,107}, {114,74,162}, {1,68,147},
{98,116,11}, {82,109,2}, {91,20,90}, {64,152,67}, {63,139,27}, {47,55,121}, {109,109,134},
{78,55,176}, {134,83,68}, {75,58,124}, {111,33,72}, {22,38,156}, {62,136,8}, {59,127,83},
{127,14,113}, {104,27,137}, {81,176,30}, {174,29,59}, {111,73,83}, {28,94,128}, {91,74,9},
{29,70,100}, {149,51,142}, {84,58,38}, {41,77,140}, {60,101,90}, {88,67,33}, {71,83,172},
{175,35,123}, {100,143,117}, {40,70,126}, {171,52,110}, {135,52,37}, {130,103,85}, {17,101,127},
{126,38,166}, {17,28,44}, {4,35,15}, {164,139,142}, {70,138,2}, {57,54,21}, {12,110,82},
{152,38,77}, {41,31,70}, {63,101,165}, {40,171,95}, {103,164,88}, {67,124,53}, {132,30,37},
{19,155,92}, {65,17,154}, {1,162,125}, {58,61,87}, {85,41,24}, {113,172,86}, {51,24,3},
{14,153,23}, {69,130,38}, {80,22,127}, {126,138,68}, {156,3,146}, {118,99,172}, {120,157,129},
{12,99,79}, {91,22,4}, {92,3,7}, {59,100,8}, {160,98,156}, {45,111,108}, {69,173,56},
{31,143,158}, {84,107,63}, {153,61,163}, {13,8,73}, {146,20,22}, {137,52,76}, {145,3,16},
{105,39,147}, {179,37,92}, {149,52,126}, {174,128,115}, {136,168,21}, {86,58,79}, {128,52,47},
{78,49,97}, {74,57,96}, {35,161,170}, {8,74,158}, {77,127,152}, {4,110,95}, {167,63,116},
{29,107,60}, {3,29,167}, {65,85,50}, {116,33,25}, {125,68,113}, {140,135,55}, {160,108,51},
{131,128,142}, {76,26,169}, {51,61,128}, {156,91,77}, {83,59,165}, {0,171,53}, {148,43,65},
{47,116,94}, {70,175,44}, {153,129,125}, {115,88,115}, {149,127,19}, {120,154,11}, {21,8,78},
{146,11,51}, {97,85,153}, {76,118,105}, {81,9,23}, {11,115,149}, {2,97,48}, {152,30,43},
{130,47,7}, {93,86,35}, {31,64,93}, {89,4,153}, {101,48,42}, {14,97,117}, {57,37,94},
{77,154,0}
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

	Pos positionsGrade_2[] = {
		{-22,6}, {0,6}, {14,16}, {-2,-13}, {0,10}, {20,20}, {-9,15}, {-21,3},
{-5,20}, {10,22}, {2,-21}, {-10,22}, {-16,-14}, {-23,-8}, {5,-15}, {-2,17},
{-22,-16}, {7,13}, {17,2}, {-7,-23}, {-18,-24}, {23,-12}, {-4,14}, {13,4},
{-15,22}, {5,7}, {11,-23}, {20,-5}, {8,-24}, {-12,12}, {-10,-14}, {14,0},
{-24,-21}, {-7,-10}, {10,14}, {7,-19}, {22,15}, {-8,-17}, {-13,18}, {0,22},
{-16,-5}, {-12,-24}, {7,-10}, {16,20}, {-12,-19}, {-23,8}, {8,8}, {23,-1},
{14,-22}, {-3,-7}, {18,10}, {3,-7}, {23,23}, {5,23}, {-18,-8}, {-16,-19},
{17,23}, {23,9}, {-21,-3}, {16,-4}, {-11,-6}, {9,4}, {19,14}, {22,6},
{-17,-11}, {18,7}, {-17,0}, {12,-18}, {21,-9}, {-24,14}, {-19,-19}
	};

	Rot rotationsGrade_2[] = {
		{50,4,0}, {42,97,122}, {164,17,112}, {120,36,98}, {178,101,161}, {110,26,22}, {36,18,73},
{94,13,93}, {17,67,88}, {141,135,168}, {71,81,17}, {103,141,6}, {17,180,148}, {22,13,145},
{179,126,114}, {122,9,132}, {129,180,140}, {37,163,140}, {73,124,144}, {125,84,63}, {123,174,158},
{7,34,70}, {160,108,115}, {178,142,6}, {91,46,82}, {9,48,6}, {92,121,32}, {13,11,41},
{170,9,121}, {153,147,106}, {144,51,145}, {87,172,37}, {133,76,49}, {149,75,59}, {138,70,125},
{167,100,172}, {123,59,40}, {53,164,137}, {114,30,179}, {18,35,27}, {136,145,40}, {142,9,46},
{134,45,66}, {28,117,170}, {92,49,32}, {156,38,39}, {175,42,50}, {70,16,14}, {130,105,107},
{154,94,101}, {51,161,74}, {117,33,142}, {33,48,161}, {72,105,114}, {110,154,152}, {88,141,26},
{167,163,173}, {141,150,35}, {97,150,88}, {160,58,153}, {66,144,131}, {105,133,175}, {103,114,137},
{43,38,173}, {110,120,128}, {14,74,76}, {8,78,58}, {150,148,13}, {95,125,12}, {116,174,168}


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

	Pos positionsGrade_3[] = {
		{-4,-13}, {-24,-9}, {10,21}, {-9,13}, {-7,20}, {-1,10}, {-1,-18}, {-12,21},
{-2,17}, {-14,-11}, {-2,5}, {3,18}, {-9,8}, {-8,-23}, {-23,-16}, {19,18},
{12,9}, {-22,1}, {-16,-16}, {17,0}, {-18,-24}, {13,15}, {5,13}, {5,-15},
{0,-23}
	};

	Rot rotationsGrade_3[] = {
		{24,105,31}, {143,67,34}, {128,111,76}, {39,28,139}, {2,84,76}, {177,19,41}, {96,98,11},
{88,120,3}, {154,67,172}, {76,52,50}, {91,155,29}, {175,177,179}, {79,10,77}, {94,114,16},
{171,71,108}, {153,140,51}, {79,17,87}, {165,4,16}, {176,130,165}, {33,136,154}, {114,172,128},
{109,53,83}, {117,173,145}, {96,35,42}, {2,21,81}
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

void SceneGame::Update()
{
}

void SceneGame::Draw()
{
}


