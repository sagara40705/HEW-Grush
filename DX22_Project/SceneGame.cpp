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
		{8,6}, {9,-8}, {-4,-25}, {-2,13}, {-5,8}, {8,-6}, {-8,-13}, {3,-10},
{-9,5}, {-5,-6}, {7,-2}, {-3,-10}, {-7,-9}, {-15,3}, {-12,9}, {-7,-7},
{-19,-8}, {-13,-7}, {1,-12}, {20,-18}, {7,10}, {11,-5}, {-11,-12}, {-8,10},
{5,-1}, {6,-13}, {-8,-1}, {12,-8}, {6,4}, {-8,3}, {-15,-1}, {-12,-9},
{8,-11}, {-5,6}, {-11,2}, {1,7}, {9,10}, {3,10}, {-20,18}, {3,6},
{-9,8}, {5,-11}, {-2,-34}, {5,7}, {1,5}, {-6,11}, {-7,6}, {-22,4},
{3,-8}, {13,0}
	};
	
	const Rot rotationsGrade_1[] = {
		{172,141,79}, {79,86,43}, {98,82,52}, {43,115,33}, {11,163,60}, {88,108,39}, {126,34,29},
{169,63,8}, {124,72,87}, {83,10,167}, {99,1,80}, {16,37,157}, {77,80,53}, {105,33,123},
{151,87,132}, {100,66,0}, {67,34,145}, {15,168,110}, {107,150,87}, {157,123,180}, {170,167,140},
{68,177,48}, {19,43,56}, {4,4,107}, {51,164,165}, {134,50,18}, {158,143,35}, {7,158,77},
{18,40,11}, {175,26,124}, {41,27,57}, {162,14,28}, {133,84,92}, {105,73,159}, {127,20,103},
{10,31,67}, {171,28,111}, {54,26,15}, {120,164,45}, {3,38,106}, {62,173,34}, {43,12,91},
{8,126,60}, {158,109,128}, {149,1,10}, {72,67,149}, {163,11,38}, {121,153,36}, {151,45,119},
{89,76,56}
	};*/


	//60×60
	const Pos positionsGrade_1[] = {
		{7,-3}, {18,4}, {-11,-11}, {9,-2}, {-10,7}, {12,-8}, {-11,-1}, {27,11},
{5,4}, {-10,5}, {-9,-3}, {1,7}, {6,9}, {3,10}, {29,4}, {-7,27},
{12,2}, {-13,-15}, {11,-5}, {10,-10}, {-4,5}, {-15,-4}, {-13,-3}, {-9,10},
{8,10}, {-6,5}, {-7,-2}, {11,18}, {3,13}, {20,12}, {17,-9}, {-1,14},
{9,0}, {-5,-9}, {-22,-9}, {-2,-15}, {7,-5}, {5,1}, {0,-11}, {-10,1}
	};

	const Rot rotationsGrade_1[] = {
		{137,28,162}, {57,11,57}, {153,76,7}, {114,179,53}, {30,168,56}, {145,166,39}, {89,11,175},
{53,151,180}, {30,77,65}, {36,72,16}, {25,50,108}, {104,18,135}, {107,133,141}, {140,140,138},
{74,58,119}, {76,13,163}, {28,29,164}, {13,16,85}, {157,130,162}, {158,163,46}, {70,167,68}, {31,47,77},
{143,172,155}, {67,170,34}, {47,13,158}, {23,48,2}, {157,49,38}, {163,93,62}, {2,66,111}, {22,89,65},
{10,86,100}, {152,99,54}, {178,8,52}, {128,164,64}, {115,24,3}, {16,129,61}, {156,25,166}, {49,109,36},
{59,19,130}, {128,97,147}
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
		{-6, -23}, {-24, -1}, {9, -21}, {14, -13}, {26, -1}, {-16, 15}, {-18, -16}, {-15, 8},
{16, 27}, {21, 2}, {-18, -20}, {-14, -15}, {-16, -23}, {6, 23}, {15, 16}, {24, -6},
{11, -11}, {-8, 17}, {-25, 12}, {8, -15}, {-8, -20}, {0, -24}, {1, -31}, {-14, 23},
{-1, -19}, {-32, -32}, {-19, 10}, {27, 3}, {-19, -23}, {16, 13}, {-24, 33}, {-3, -13},
{-9, -16}, {-18, -6}, {10, 24}, {-12, 19}, {-16, 18}, {-21, 1}, {14, -31}
	};

	const Rot rotationsGrade_2[] = {
		{94,63,30}, {151,161,161}, {77,122,1}, {151,88,67}, {17,144,7}, {145,86,40}, {19,8,58},
{13,97,26}, {93,15,11}, {177,178,61}, {91,31,180}, {152,59,66}, {36,23,132}, {43,144,67},
{9,138,111}, {107,28,27}, {71,138,9}, {85,3,127}, {89,174,64}, {5,147,164}, {61,71,71},
{137,2,180}, {45,76,1}, {3,147,67}, {12,111,48}, {46,112,67}, {142,158,18}, {159,136,88},
{69,141,163}, {80,177,5}, {168,82,147}, {16,3,157}, {23,32,106}, {8,154,138}, {58,171,86},
{109,50,145}, {20,134,61}, {80,112,43}, {20,57,94}, {101,51,105}
	};*/


	//60×60
	const Pos positionsGrade_2[] = {
		{1,-27}, {5,17}, {19,16}, {-7,24}, {-20,-29}, {23,4}, {8,-20}, {26,-4},
{26,6}, {-17,8}, {9,13}, {15,-17}, {-12,17}, {-8,-29}, {-6,-20}, {-24,13},
{5,-24}, {3,23}, {-24,0}, {-22,-5}, {-18,-12}, {-8,-23}, {-16,-21}, {17,13},
{14,3}, {-20,7}, {23,-1}, {5,-19}, {-22,16}, {-19,-16}
	};

	const Rot rotationsGrade_2[] = {
		{107,65,116}, {50,146,126}, {29,102,38}, {52,5,24}, {126,7,48}, {91,90,28}, {64,139,173},
{84,18,90}, {38,76,132}, {100,86,34}, {16,7,19}, {143,79,144}, {14,97,45}, {179,104,118},
{170,25,153}, {23,47,59}, {116,100,14}, {122,102,32}, {109,83,152}, {63,48,146}, {34,12,36},
{148,97,151}, {63,102,64}, {8,11,92}, {28,83,113}, {48,180,144}, {66,173,177}, {28,116,162},
{89,178,102}, {42,162,50}

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
		{3,13}, {11,28}, {23,-32}, {10,14}, {-8,-33}, {19,25}, {18,-25}, {-7,21},
{-15,26}, {24,11}, {-31,-26}, {19,17}, {24,16}, {-26,-6}, {-25,17}, {14,-9},
{-16,-29}, {-30,8}, {29,-23}, {-28,28}, {-34,20}, {-11,31}, {-23,-19}, {-28,2},
{-23,-34}, {23,-24}
	};

	const Rot rotationsGrade_3[] = {
		{66,40,55}, {180,70,70}, {90,94,164}, {7,8,139}, {165,43,116}, {90,72,30}, {139,15,128},
{127,0,11}, {151,161,120}, {8,115,126}, {145,123,21}, {25,36,155}, {9,54,30}, {125,117,33}, {81,73,106},
{19,72,52}, {0,157,150}, {143,51,41}, {130,28,92}, {48,0,89}, {10,8,0}, {13,28,12}, {62,50,80},
{131,100,11}, {76,67,137}
	};*/

	//60×60
	const Pos positionsGrade_3[] = {
		{26,-25}, {23,21}, {17,21}, {-19,26}, {-25,-23}, {-25,3}, {25,14}, {-14,24},
{13,-29}, {-22,19}, {-9,-17}, {-18,12}, {18,-25}, {-29,25}, {22,-20}, {0,26},
{26,-14}, {20,26}, {-27,-29}, {12,26}
	};

	const Rot rotationsGrade_3[] = {
		{31,9,76}, {172,121,155}, {114,55,121}, {147,142,176}, {164,104,54}, {173,79,99}, {12,149,107},
{158,164,64}, {156,31,110}, {77,12,29}, {17,44,173}, {53,144,131}, {139,29,98}, {167,38,149},
{51,173,156}, {172,110,168}, {140,155,146}, {85,151,140}, {108,5,18}, {162,65,150},
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


