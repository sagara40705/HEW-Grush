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
#include "InputKeyboard.h"
#include "InputGamepad.h"
#include "SceneManager.h"
#include "SceneStageSelect.h"
#include "SceneResult.h"
#include "SceneOption.h"
#include "SoundManager.h"
#include "RenderSystem.h"
#include "DirectX.h"

void SceneGame::Load()
{
	// 従来の同期ロードが必要なら、タスクを構築して全部回す
	std::vector<LoadTask> tasks;
	BuildLoadTasks(tasks);
	for (auto& t : tasks) t();
	CheckStartSE = false;
	CheckGameSound = false;
}

void SceneGame::Unload()
{
	SceneManager::ReleaseFadeAnimations();
	TextureManager::Clear();

	//modelの解放
	StageId stage = GetCurrentStage();
	switch (stage)
	{
	case StageId::Stage1:
		ModelManager::Unload(ModelTag::Isu);
		ModelManager::Unload(ModelTag::Reizouko);
		ModelManager::Unload(ModelTag::Eiseii);
		break;
	case StageId::Stage2:
		ModelManager::Unload(ModelTag::Kuukiseizyouki);
		ModelManager::Unload(ModelTag::SagaraDoll);
		ModelManager::Unload(ModelTag::Jihanki);
		ModelManager::Unload(ModelTag::Star);
		ModelManager::Unload(ModelTag::Warp);
		break;
	case StageId::Stage3a:
		ModelManager::Unload(ModelTag::Akican);
		ModelManager::Unload(ModelTag::Isu);
		ModelManager::Unload(ModelTag::Jihanki);
		ModelManager::Unload(ModelTag::Reizouko);
		ModelManager::Unload(ModelTag::Eiseii);
		ModelManager::Unload(ModelTag::Star);
		ModelManager::Unload(ModelTag::Warp);
		break;
	case StageId::Stage3b:
		ModelManager::Unload(ModelTag::Kuukiseizyouki);
		ModelManager::Unload(ModelTag::SagaraDoll);
		ModelManager::Unload(ModelTag::Jihanki);
		ModelManager::Unload(ModelTag::Reizouko);
		ModelManager::Unload(ModelTag::Eiseii);
		ModelManager::Unload(ModelTag::Star);
		ModelManager::Unload(ModelTag::Warp);
		break;
	case StageId::Stage3c:
		ModelManager::Unload(ModelTag::Akican);
		ModelManager::Unload(ModelTag::Isu);
		ModelManager::Unload(ModelTag::Jihanki);
		ModelManager::Unload(ModelTag::Reizouko);
		ModelManager::Unload(ModelTag::Eiseii);
		ModelManager::Unload(ModelTag::Star);
		ModelManager::Unload(ModelTag::Warp);
		break;
	default:
		break;
	}

	if (CheckGameSound)
	{
		SoundManager::StopSound(SceneGameBGM1);
		SoundManager::StopSound(SceneGameBGM2);
		CheckGameSound = false;
	}

	SetSceneColorRT(nullptr);
	SAFE_DELETE(m_sceneColorRT);

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
	StageId stage = GetCurrentStage();
	switch (stage)
	{
	case StageId::Stage1:
		outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Isu); });
		outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Reizouko); });
		outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Eiseii); });
		break;
	case StageId::Stage2:
		outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Kuukiseizyouki); });
		outTasks.emplace_back([this] { ModelManager::Load(ModelTag::SagaraDoll); });
		outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Jihanki); });
		outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Star); });
		outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Warp); });
		break;
	case StageId::Stage3a:
		outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Akican); });
		outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Isu); });
		outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Jihanki); });
		outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Reizouko); });
		outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Eiseii); });
		outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Star); });
		outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Warp); });
		break;
	case StageId::Stage3b:
		outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Kuukiseizyouki); });
		outTasks.emplace_back([this] { ModelManager::Load(ModelTag::SagaraDoll); });
		outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Jihanki); });
		outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Reizouko); });
		outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Eiseii); });
		outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Star); });
		outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Warp); });
		break;
	case StageId::Stage3c:
		outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Akican); });
		outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Isu); });
		outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Jihanki); });
		outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Reizouko); });
		outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Eiseii); });
		outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Star); });
		outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Warp); });
		break;
	default:
		assert(false && "不正なステージID");
		break;

	}

	// テクスチャーロード
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Game_Sky); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Game_Ground); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Game_3DEffect_GradeUp); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Game_3DEffect_GradeDown); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Game_3DEffect_Hit); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Game_UI_Gauge_Empty); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Game_UI_Gauge_Max); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Game_UI_Gauge_Fever); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Game_UI_Gauge_UpEffect); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Game_UI_Timer_Number); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Game_UI_Timer_Colon); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Game_UI_Score_Number); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Game_UI_Score_Text); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Game_UI_Action_Icon); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Game_UI_Action_SpinIcon); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Game_UI_Action_TextIcon); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Game_UI_Gaug_Lv1); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Game_UI_Gaug_Lv2); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Game_UI_Gaug_Lv3); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Game_GlassShard); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Game_UI_Finish); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Game_Shadow); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Game_KeepOut); });

	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Option_BG); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::option_Back); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::option_Font1); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::option_Font1_Back); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::option_Font1_OK); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::option_Font2); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::option_Font2_Back); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::option_Font2_OK); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::option_Font3); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::option_Font3_Back); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::option_Font3_OK); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::option_Font4); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::option_Font4_OK); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::option_Font5); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::option_Font5_OK); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::option_Font1_Break); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::option_Font2_Break); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::option_Font3_Break); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::option_Font4_screen); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::option_translucent); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Game_UI_Start); });

	//outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Tutorial_UI_Task); });
	//outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Tutorial_Road); });
	//outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Tutorial_Targetlocation); });

	// プレイハブ設定
	//outTasks.emplace_back([this] { RegisterAllPrefabs(); });

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
	outTasks.emplace_back([this] { CreateStartUI(); });

	//ギミック
	outTasks.emplace_back([this] { CreateGlassObjectSpawner(); });
	outTasks.emplace_back([this] { CreateTutorialWall(); });

	//outTasks.emplace_back([this] { InitPlayerTex(); });
	outTasks.emplace_back([this] { InitSkyDome(); });

	outTasks.emplace_back([this] { SceneManager::CreateFadeAnimations(this); });

	outTasks.emplace_back([this]
		{
			// SceneColorRT
			if (m_sceneColorRT)
			{
				return;
			}

			RenderTarget* bb = GetDefaultRTV();
			if (!bb)
			{
				return;
			}

			m_sceneColorRT = new RenderTarget();
			HRESULT hr = m_sceneColorRT->Create(DXGI_FORMAT_R8G8B8A8_UNORM, bb->GetWidth(), bb->GetHeight());
			if (FAILED(hr) || m_sceneColorRT->GetView() == nullptr || m_sceneColorRT->GetResource() == nullptr)
			{
				SAFE_DELETE(m_sceneColorRT);
				SetSceneColorRT(nullptr);
				return;
			}

			// RenderSystem.h 
			SetSceneColorRT(m_sceneColorRT);
		});


	// 最後に Start を流す
	outTasks.emplace_back([this] 
		{
		GetWorld().Start();
		});
}

void SceneGame::OnReturnFromOption()
{
	// オプション画面から戻ってきたときの処理
	SceneManager::CreateFadeAnimations(this); // フェードアニメーションを再作成
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

	//ステージ１
	//GameObject& cameraPivot = w.CreateGameObject(Vector3::zero(), Vector3(0.f, 180.f, 0.f), Vector3::one());

	//ステージ2
	GameObject& cameraPivot = w.CreateGameObject(Vector3::zero(), Vector3(0.f, -90.f, 0.f), Vector3::one());

	cameraPivot.SetTag(Tag::CamreraPivot);
	cameraPivot.AddComponent<CameraControl>();

	GameObject& mainCamera = cameraPivot.CreateChild(Camera::kInitPos, Camera::kInitRotate, Vector3::one());
	mainCamera.SetTag(Tag::MainCamera);
	mainCamera.AddComponent<Camera>();
	mainCamera.AddComponent<CameraShake>();

	//70×70 ①
	//ステージ１
	//GameObject& player = w.CreateGameObject(Vector3(0.f, 2.f, 0.f), Vector3(0.f, 0.f, 0.f));]

	constexpr Vector3 KPlayerInitPos[MAX_STAGE] = {
		Vector3(0.f, 2.f, 0.f),        // Stage1
		Vector3(110.f, 2.f, 0.f),     // Stage2
		Vector3(0.f, 2.f, 0.f),     // Stage3a
		Vector3(0.f, 2.f, 0.f),     // Stage3b
		Vector3(0.f, 2.f, 0.f)      // Stage3c
	};

	//ステージ2
	GameObject& player = w.CreateGameObject(KPlayerInitPos[ToIndex(GetCurrentStage())], Vector3(0.f, 90.f, 0.f));
 
	player.SetTag(Tag::Player);
	player.AddComponent<MeshFilter>(ModelTag::Player_1);
	player.AddComponent<MeshRenderer>();
	player.AddComponent<Rigidbody>();
	player.AddComponent<BoxCollider>(false, Vector3(0.f, 0.5f, -0.1f), Vector3(0.7f, 1.f, 0.7f));
	player.AddComponent<PlayerState>();
	player.AddComponent<PlayerMove>();
	player.AddComponent<PlayerAction>();
	player.AddComponent<Animator>();
	player.AddComponent<PlayerAnimation>();

	GameObject& hammerPivot = player.CreateChild(Vector3::zero(), Vector3::zero(), Vector3::one());
	hammerPivot.SetTag(Tag::HammerPivot);

	GameObject& hummer = hammerPivot.CreateChild(Hammer::BasePos, Hammer::BaseRota, Vector3(0.2f, 0.2f, 0.2f));
	hummer.SetTag(Tag::Hammer);
	hummer.AddComponent<MeshFilter>(ModelTag::Hammer_3);
	hummer.AddComponent<MeshRenderer>();
	hummer.AddComponent<BoxCollider>(false, Vector3(-3.f, 2.f, -3.f), Vector3(5.f, 5.f, 2.f));
	hummer.AddComponent<Hammer>();
	hummer.AddComponent<HammerEffects>();

	GameObject& shadow = player.CreateChild(Vector3(0.f, 0.1f, 0.f), Vector3(90.f, 0.f, 0.f), Vector3::one());
	shadow.AddComponent<Sprite3DRenderer>(TextureTag::Game_Shadow, Vector2::zero(), Vector2(2.f, 4.f));

	GameObject& sample = w.CreateGameObject(Vector3(0.f, 2.f, 4.f), Vector3(0.f, 0.f, 0.f));
	//sample.AddComponent<Sprite3DRenderer>(TextureTag::Game_KeepOut, Vector2::zero(), Vector2::zero());


	GameObject& glassShardPool = w.CreateGameObject();
	glassShardPool.AddComponent<GlassShardPool>();

	//GameObject& hummerC = w.CreateGameObject(Vector3(0.f, 2.f, 4.f), Vector3(0.f, 0.f, 0.f));
	//hummerC.AddComponent<MeshFilter>(ModelTag::Hammer_1);
	//hummerC.AddComponent<MeshRenderer>();
	//hummerC.AddComponent<Animator>();
}

void SceneGame::CreateStage()
{
	StageId stage = GetCurrentStage();

	// デバック用
	//stage = StageId::Stage1;

	switch (stage)
	{
	case StageId::Stage1:	CreateStage1();
		break;
	case StageId::Stage2:	CreateStage2();
		break;
	case StageId::Stage3a:	CreateStage3();
		break;
	case StageId::Stage3b:	CreateStage3();
		break;
	case StageId::Stage3c:	CreateStage3();
		break;
	default:
		assert(false && "不正なステージID");
		break;
	}
}

// ステージ１生成
void SceneGame::CreateStage1()
{
	World& w = GetWorld();

	//80端
	constexpr float SIZE = 80.f;
	constexpr float HALF = SIZE / 2.f;
	constexpr float OFFSET = HALF + 0.5f;
	
	//80フィールド
	GameObject& floor = w.CreateGameObject(Vector3(0.f, -1.0f, 0.f), Vector3::zero(), Vector3(80.f, 0.1f, 80.f));
	MeshFilter& mf = floor.AddComponent<MeshFilter>(PrimitiveMeshTag::Plane); mf.boundsRadius = 0.f;
	floor.AddComponent<GroundRenderer>();
	BoxCollider& collider = floor.AddComponent<BoxCollider>();
	collider.layer = Layer::Ground;

	//壁
	//GameObject& wT = w.CreateGameObject(Vector3(0.f, 1.f, OFFSET), Vector3(90.f, 0.f, 0.f), Vector3(SIZE, 1.f, 4.f));
	//MeshFilter& mfT = wT.AddComponent<MeshFilter>(PrimitiveMeshTag::Cube); mfT.boundsRadius = 0.f;
	//wT.AddComponent<GroundRenderer>();
	//BoxCollider& bcT = wT.AddComponent<BoxCollider>(); bcT.layer = Layer::Ground;
	//wT.AddComponent<StageWall>();
	//
	//GameObject& wB = w.CreateGameObject(Vector3(0.f, 1.f, -OFFSET), Vector3(90.f, 0.f, 0.f), Vector3(SIZE, 1.f, 4.0f));
	//MeshFilter& mfB = wB.AddComponent<MeshFilter>(PrimitiveMeshTag::Cube); mfB.boundsRadius = 0.f;
	//wB.AddComponent<GroundRenderer>();
	//BoxCollider& bcB = wB.AddComponent<BoxCollider>(); bcB.layer = Layer::Ground;
	//wB.AddComponent<StageWall>();
	//
	//GameObject& wR = w.CreateGameObject(Vector3(OFFSET, 1.f, 0.f), Vector3(0.f, 0.f, 90.f), Vector3(4.0f, 1.f, SIZE));
	//MeshFilter& mfR = wR.AddComponent<MeshFilter>(PrimitiveMeshTag::Cube); mfR.boundsRadius = 0.f;
	//wR.AddComponent<GroundRenderer>();
	//BoxCollider& bcR = wR.AddComponent<BoxCollider>(); bcR.layer = Layer::Ground;
	//wR.AddComponent<StageWall>();
	//
	//GameObject& wL = w.CreateGameObject(Vector3(-OFFSET, 1.f, 0.f), Vector3(0.f, 0.f, 90.f), Vector3(4.0f, 1.f, SIZE));
	//MeshFilter& mfL = wL.AddComponent<MeshFilter>(PrimitiveMeshTag::Cube); mfL.boundsRadius = 0.f;
	//wL.AddComponent<GroundRenderer>();
	//BoxCollider& bcL = wL.AddComponent<BoxCollider>(); bcL.layer = Layer::Ground;
	//wL.AddComponent<StageWall>();

	CreateStageFromCSV("CSV/StageData_Grade1.csv");
	CreateStageFromCSV("CSV/StageData_Grade2.csv");
	CreateStageFromCSV("CSV/StageData_Grade3.csv");

	CreateStage1Wall();

	return;
}

// ステージ１の壁生成
void SceneGame::CreateStage1Wall()
{
	World& w = GetWorld();
	// 初期位置からみて前の壁
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-37.99f, -0.75f, -39.35f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-34.0f, -0.75f, -39.35f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-30.01f, -0.75f, -39.35f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-26.02f, -0.75f, -39.35f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-22.03f, -0.75f, -39.35f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-18.04f, -0.75f, -39.35f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-14.05f, -0.75f, -39.35f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-10.06f, -0.75f, -39.35f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-6.07f, -0.75f, -39.35f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-2.08f, -0.75f, -39.35f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(1.91f, -0.75f, -39.35f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(5.90f, -0.75f, -39.35f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(9.89f, -0.75f, -39.35f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(13.88f, -0.75f, -39.35f), Vector3(0.f,180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(17.87f, -0.75f, -39.35f), Vector3(0.f,180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(21.86f, -0.75f, -39.35f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(25.85f, -0.75f, -39.35f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(29.84f, -0.75f, -39.35f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(33.84f, -0.75f, -39.35f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(37.82f, -0.75f, -39.35f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));

	// 初期位置から見て後ろの壁
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-37.99f, -0.75f, 39.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-34.0f, -0.75f, 39.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-30.01f, -0.75f, 39.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-26.02f, -0.75f, 39.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-22.03f, -0.75f, 39.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-18.04f, -0.75f, 39.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-14.05f, -0.75f, 39.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-10.06f, -0.75f, 39.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-6.07f, -0.75f, 39.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-2.08f, -0.75f, 39.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(1.91f, -0.75f, 39.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(5.90f, -0.75f, 39.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(9.89f, -0.75f, 39.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(13.88f, -0.75f, 39.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(17.87f, -0.75f, 39.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(21.86f, -0.75f, 39.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(25.85f, -0.75f, 39.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(29.84f, -0.75f, 39.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(33.84f, -0.75f, 39.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(37.82f, -0.75f, 39.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));

	// 左の横壁
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(39.8f, -0.75f, 37.5f), Vector3(0.f, 90.0f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(39.8f, -0.75f, 33.5f), Vector3(0.f, 90.0f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(39.8f, -0.75f, 29.5f), Vector3(0.f, 90.0f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(39.8f, -0.75f, 25.5f), Vector3(0.f, 90.0f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(39.8f, -0.75f, 21.5f), Vector3(0.f, 90.0f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(39.8f, -0.75f, 17.5f), Vector3(0.f, 90.0f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(39.8f, -0.75f, 13.5f), Vector3(0.f, 90.0f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(39.8f, -0.75f, 9.5f), Vector3(0.f, 90.0f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(39.8f, -0.75f, 5.5f), Vector3(0.f, 90.0f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(39.8f, -0.75f, 1.5f), Vector3(0.f, 90.0f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(39.8f, -0.75f, -2.5f), Vector3(0.f, 90.0f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(39.8f, -0.75f, -6.5f), Vector3(0.f, 90.0f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(39.8f, -0.75f, -10.5f), Vector3(0.f, 90.0f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(39.8f, -0.75f, -14.5f), Vector3(0.f, 90.0f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(39.8f, -0.75f, -18.5f), Vector3(0.f, 90.0f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(39.8f, -0.75f, -22.5f), Vector3(0.f, 90.0f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(39.8f, -0.75f, -26.5f), Vector3(0.f, 90.0f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(39.8f, -0.75f, -30.5f), Vector3(0.f, 90.0f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(39.8f, -0.75f, -34.5f), Vector3(0.f, 90.0f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(39.8f, -0.75f, -37.88f), Vector3(0.f, 90.0f, 0.f), Vector3(0.3f, 0.4f, 1.0f));

	// 右の横壁
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.0f, -0.75f, 37.5f), Vector3(0.f, -90.0f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.0f, -0.75f, 33.5f), Vector3(0.f, -90.0f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.0f, -0.75f, 29.5f), Vector3(0.f, -90.0f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.0f, -0.75f, 25.5f), Vector3(0.f, -90.0f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.0f, -0.75f, 21.5f), Vector3(0.f, -90.0f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.0f, -0.75f, 17.5f), Vector3(0.f, -90.0f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.0f, -0.75f, 13.5f), Vector3(0.f, -90.0f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.0f, -0.75f, 9.5f), Vector3(0.f, -90.0f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.0f, -0.75f, 5.5f), Vector3(0.f, -90.0f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.0f, -0.75f, 1.5f), Vector3(0.f, -90.0f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.0f, -0.75f, -2.5f), Vector3(0.f, -90.0f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.0f, -0.75f, -6.5f), Vector3(0.f, -90.0f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.0f, -0.75f, -10.5f), Vector3(0.f, -90.0f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.0f, -0.75f, -14.5f), Vector3(0.f, -90.0f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.0f, -0.75f, -18.5f), Vector3(0.f, -90.0f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.0f, -0.75f, -22.5f), Vector3(0.f, -90.0f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.0f, -0.75f, -26.5f), Vector3(0.f, -90.0f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.0f, -0.75f, -30.5f), Vector3(0.f, -90.0f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.0f, -0.75f, -34.5f), Vector3(0.f, -90.0f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.0f, -0.75f, -37.88f), Vector3(0.f, -90.0f, 0.f), Vector3(0.33f, 0.4f, 1.0f));
}

// ステージ２生成
void SceneGame::CreateStage2(){
	World& w = GetWorld();

	//80 * 50 フィールド1
	GameObject& floor1 = w.CreateGameObject(Vector3(0.f, -2.5f, 0.f), Vector3::zero(), Vector3(85.f, 3.0f, 55.f));
	MeshFilter& mf1 = floor1.AddComponent<MeshFilter>(PrimitiveMeshTag::Cube); mf1.boundsRadius = 0.f;
	floor1.AddComponent<GroundRenderer>();
	BoxCollider& collider1 = floor1.AddComponent<BoxCollider>();
	collider1.layer = Layer::Ground;

	//80 * 50 フィールド1
	GameObject& floor2 = w.CreateGameObject(Vector3(90.f, -2.5f, 0.f), Vector3::zero(), Vector3(85.f, 3.0f, 55.f));
	MeshFilter& mf2 = floor2.AddComponent<MeshFilter>(PrimitiveMeshTag::Cube); mf2.boundsRadius = 0.f;
	floor2.AddComponent<GroundRenderer>();
	BoxCollider& collider2 = floor2.AddComponent<BoxCollider>();
	collider2.layer = Layer::Ground;

	CreateWarpStage2();
	CreateStage2Wall();

	CreateStageFromCSV("CSV/Stage2LeftData_Grade1.csv");
	CreateStageFromCSV("CSV/Stage2LeftData_Grade2.csv");
	CreateStageFromCSV("CSV/Stage2RightData_Grade1.csv");
	CreateStageFromCSV("CSV/Stage2RightData_Grade2.csv");
	CreateStageFromCSV("CSV/Stage2RightData_Grade3.csv");
}

// ステージ２の壁生成
void SceneGame::CreateStage2Wall()
{
	World& w = GetWorld();
	// hidariのステージのmaeの壁
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(41.f, -0.75f, 27.0f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(37.f, -0.75f, 27.0f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(33.f, -0.75f, 27.0f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(29.f, -0.75f, 27.0f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(25.f, -0.75f, 27.0f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(21.f, -0.75f, 27.0f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(17.f, -0.75f, 27.0f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(13.f, -0.75f, 27.0f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(9.f, -0.75f, 27.0f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(5.f, -0.75f, 27.0f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(1.f, -0.75f, 27.0f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-3.f, -0.75f, 27.0f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-7.f, -0.75f, 27.0f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-11.f, -0.75f, 27.0f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-15.f, -0.75f, 27.0f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-19.f, -0.75f, 27.0f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-23.f, -0.75f, 27.0f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-27.f, -0.75f, 27.0f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-31.f, -0.75f, 27.0f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-35.f, -0.75f, 27.0f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-39.7f, -0.75f, 27.0f), Vector3(0.f, 0.f, 0.f), Vector3(0.52f, 0.4f, 1.0f));

	// hidariのステージのusiroの壁
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(41.f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(37.f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(33.f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(29.f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(25.f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(21.f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(17.f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(13.f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(9.f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(5.f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(1.f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-3.f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-7.f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-11.f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-15.f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-19.f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-23.f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-27.f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-31.f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-35.f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-39.7f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.52f, 0.4f, 1.0f));

	// 左ステージの右壁
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(43.0f, -0.75f, -25.5f), Vector3(0.f, 90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(43.0f, -0.75f, -21.5f), Vector3(0.f, 90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(43.0f, -0.75f, -17.5f), Vector3(0.f, 90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(43.0f, -0.75f, -13.5f), Vector3(0.f, 90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(43.0f, -0.75f, -9.5f), Vector3(0.f, 90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(43.0f, -0.75f, -5.5f), Vector3(0.f, 90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(43.0f, -0.75f, -1.f), Vector3(0.f, 90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(43.0f, -0.75f, 3.5f), Vector3(0.f, 90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(43.0f, -0.75f, 7.5f), Vector3(0.f, 90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(43.0f, -0.75f, 11.5f), Vector3(0.f, 90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(43.0f, -0.75f, 15.5f), Vector3(0.f, 90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(43.0f, -0.75f, 19.5f), Vector3(0.f, 90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(43.0f, -0.75f, 23.5f), Vector3(0.f, 90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(43.0f, -0.75f, 26.3f), Vector3(0.f, 90.f, 0.f), Vector3(0.2f, 0.4f, 1.0f));

	// hidariのステージの左壁
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-42.3f, -0.75f, -25.5f), Vector3(0.f, -90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-42.3f, -0.75f, -21.5f), Vector3(0.f, -90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-42.3f, -0.75f, -17.5f), Vector3(0.f, -90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-42.3f, -0.75f, -13.5f), Vector3(0.f, -90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-42.3f, -0.75f, -9.5f), Vector3(0.f, -90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-42.3f, -0.75f, -5.5f), Vector3(0.f, -90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-42.3f, -0.75f, -1.f),     Vector3(0.f, -90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));//
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-42.3f, -0.75f, 3.5f), Vector3(0.f, -90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-42.3f, -0.75f, 7.5f), Vector3(0.f, -90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-42.3f, -0.75f, 11.5f), Vector3(0.f, -90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-42.3f, -0.75f, 15.5f), Vector3(0.f, -90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-42.3f, -0.75f, 19.5f), Vector3(0.f, -90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-42.3f, -0.75f, 23.5f), Vector3(0.f, -90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-42.3f, -0.75f, 26.3f), Vector3(0.f, -90.f, 0.f), Vector3(0.2f, 0.4f, 1.0f));


	// migiのステージのusiroの壁
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(50.f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(54.f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(58.f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(62.f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(66.f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(70.f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(74.f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(78.f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(82.f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(86.f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(90.f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(94.f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(98.f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(102.f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(106.f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(110.f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(114.f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(118.f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(122.f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(126.f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(130.f, -0.75f, -27.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));

	// migiのステージのmaeの壁
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(50.f, -0.75f, 27.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(54.f, -0.75f, 27.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(58.f, -0.75f, 27.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(62.f, -0.75f, 27.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(66.f, -0.75f, 27.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(70.f, -0.75f, 27.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(74.f, -0.75f, 27.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(78.f, -0.75f, 27.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(82.f, -0.75f, 27.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(86.f, -0.75f, 27.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(90.f, -0.75f, 27.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(94.f, -0.75f, 27.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(98.f, -0.75f, 27.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(102.f, -0.75f, 27.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(106.f, -0.75f, 27.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(110.f, -0.75f, 27.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(114.f, -0.75f, 27.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(118.f, -0.75f, 27.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(122.f, -0.75f, 27.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(126.f, -0.75f, 27.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(130.f, -0.75f, 27.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));

	// migiのステージのhidariの壁
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(48.0f, -0.75f, -25.8f), Vector3(0.f, -90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(48.0f, -0.75f, -21.8f), Vector3(0.f, -90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(48.0f, -0.75f, -17.8f), Vector3(0.f, -90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(48.0f, -0.75f, -13.8f), Vector3(0.f, -90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(48.0f, -0.75f, -9.8f), Vector3(0.f, -90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(48.0f, -0.75f, -5.8f), Vector3(0.f, -90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(48.0f, -0.75f, -1.45f), Vector3(0.f, -90.f, 0.f), Vector3(0.48f, 0.4f, 1.0f));//
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(48.0f, -0.75f, 2.8f), Vector3(0.f, -90.f, 0.f), Vector3(0.45f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(48.0f, -0.75f, 6.8f), Vector3(0.f, -90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(48.0f, -0.75f, 10.8f), Vector3(0.f, -90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(48.0f, -0.75f, 14.8f), Vector3(0.f, -90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(48.0f, -0.75f, 18.8f), Vector3(0.f, -90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(48.0f, -0.75f, 22.8f), Vector3(0.f, -90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(48.0f, -0.75f, 26.2f), Vector3(0.f, -90.f, 0.f), Vector3(0.3f, 0.4f, 1.0f));

	// migiのステージのmigiの壁
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(132.0f, -0.75f, -25.8f), Vector3(0.f, 90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(132.0f, -0.75f, -21.8f), Vector3(0.f, 90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(132.0f, -0.75f, -17.8f), Vector3(0.f, 90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(132.0f, -0.75f, -13.8f), Vector3(0.f, 90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(132.0f, -0.75f, -9.8f), Vector3(0.f, 90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(132.0f, -0.75f, -5.8f), Vector3(0.f, 90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(132.0f, -0.75f, -1.45f), Vector3(0.f, 90.f, 0.f), Vector3(0.48f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(132.0f, -0.75f, 2.8f), Vector3(0.f, 90.f, 0.f), Vector3(0.45f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(132.0f, -0.75f, 6.8f), Vector3(0.f, 90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(132.0f, -0.75f, 10.8f), Vector3(0.f, 90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(132.0f, -0.75f, 14.8f), Vector3(0.f, 90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(132.0f, -0.75f, 18.8f), Vector3(0.f, 90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(132.0f, -0.75f, 22.8f), Vector3(0.f, 90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(132.0f, -0.75f, 26.2f), Vector3(0.f, 90.f, 0.f), Vector3(0.3f, 0.4f, 1.0f));
}

// ステージ3の作成
void SceneGame::CreateStage3()
{
	World& w = GetWorld();

	//80 * 50 フィールド
	GameObject& floor1 = w.CreateGameObject(Vector3(0.f, -3.5f, 0.f), Vector3::zero(), Vector3(80.f, 5.1f, 80.f));
	MeshFilter& mf1 = floor1.AddComponent<MeshFilter>(PrimitiveMeshTag::Cube); mf1.boundsRadius = 0.f;
	floor1.AddComponent<GroundRenderer>();
	BoxCollider& collider1 = floor1.AddComponent<BoxCollider>();
	collider1.layer = Layer::Ground;

	//50 * 50 フィールド1
	GameObject& floor2 = w.CreateGameObject(Vector3(0.f, -3.5f, 70.f), Vector3::zero(), Vector3(80.f, 5.1f, 60.f));
	MeshFilter& mf2 = floor2.AddComponent<MeshFilter>(PrimitiveMeshTag::Cube); mf2.boundsRadius = 0.f;
	floor2.AddComponent<GroundRenderer>();
	BoxCollider& collider2 = floor2.AddComponent<BoxCollider>();
	collider2.layer = Layer::Ground;

	//50 * 50 フィールド2
	GameObject& floor3 = w.CreateGameObject(Vector3(100.f, -3.5f, 0.f), Vector3::zero(), Vector3(60.f, 5.1f, 60.f));
	MeshFilter& mf3 = floor3.AddComponent<MeshFilter>(PrimitiveMeshTag::Cube); mf3.boundsRadius = 0.f;
	floor3.AddComponent<GroundRenderer>();
	BoxCollider& collider3 = floor3.AddComponent<BoxCollider>();

	CreateWarpStage3();

	StageId stage = GetCurrentStage();

	switch (stage)
	{
	case StageId::Stage3a:
		CreateStageFromCSV("CSV/Stage3-1LeftData_Grade1.csv");
		CreateStageFromCSV("CSV/Stage3-1LeftData_Grade2.csv");
		CreateStageFromCSV("CSV/Stage3-1LeftData_Grade3.csv");
		CreateStageFromCSV("CSV/Stage3-1RightData_Grade1.csv");
		CreateStageFromCSV("CSV/Stage3-1RightData_Grade2.csv");
		CreateStageFromCSV("CSV/Stage3-1RightData_Grade3.csv");
		CreateStageFromCSV("CSV/Stage3-1UEData_Grade1.csv");
		CreateStageFromCSV("CSV/Stage3-1UEData_Grade2.csv");
		CreateStageFromCSV("CSV/Stage3-1UEData_Grade3.csv");
		break;
	case StageId::Stage3b:	
		CreateStageFromCSV("CSV/Stage3-2LeftData_Grade1.csv");
		CreateStageFromCSV("CSV/Stage3-2LeftData_Grade2.csv");
		CreateStageFromCSV("CSV/Stage3-2LeftData_Grade3.csv");
		CreateStageFromCSV("CSV/Stage3-2RightData_Grade1.csv");
		CreateStageFromCSV("CSV/Stage3-2RightData_Grade2.csv");
		CreateStageFromCSV("CSV/Stage3-2RightData_Grade3.csv");
		CreateStageFromCSV("CSV/Stage3-2UEData_Grade1.csv");
		CreateStageFromCSV("CSV/Stage3-2UEData_Grade2.csv");
		CreateStageFromCSV("CSV/Stage3-2UEData_Grade3.csv");
		break;
	case StageId::Stage3c:
		CreateStageFromCSV("CSV/Stage3-3LeftData_Grade1.csv");
		CreateStageFromCSV("CSV/Stage3-3LeftData_Grade2.csv");
		CreateStageFromCSV("CSV/Stage3-3LeftData_Grade3.csv");
		CreateStageFromCSV("CSV/Stage3-3RightData_Grade1.csv");
		CreateStageFromCSV("CSV/Stage3-3RightData_Grade2.csv");
		CreateStageFromCSV("CSV/Stage3-3RightData_Grade3.csv");
		CreateStageFromCSV("CSV/Stage3-3UEData_Grade1.csv");
		CreateStageFromCSV("CSV/Stage3-3UEData_Grade2.csv");
		CreateStageFromCSV("CSV/Stage3-3UEData_Grade3.csv");
		break;
	default:
		assert(false && "不正なステージID");
		break;
	}

	CreateStage3Wall();

}

// ステージ3の壁の作成
void SceneGame::CreateStage3Wall()
{
	World& w = GetWorld();
	//後ろの壁
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(40.f, -0.75f, 97.0f), Vector3(0.f, 90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(40.f, -0.75f, 92.0f), Vector3(0.f, 90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(40.f, -0.75f, 87.0f), Vector3(0.f, 90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(40.f, -0.75f, 82.0f), Vector3(0.f, 90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(40.f, -0.75f, 77.0f), Vector3(0.f, 90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(40.f, -0.75f, 72.0f), Vector3(0.f, 90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(40.f, -0.75f, 67.0f), Vector3(0.f, 90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(40.f, -0.75f, 62.0f), Vector3(0.f, 90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(40.f, -0.75f, 57.0f), Vector3(0.f, 90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(40.f, -0.75f, 52.0f), Vector3(0.f, 90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(40.f, -0.75f, 47.0f), Vector3(0.f, 90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(40.f, -0.75f, 42.0f), Vector3(0.f, 90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(40.f, -0.75f, 37.0f), Vector3(0.f, 90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(40.f, -0.75f, 32.0f), Vector3(0.f, 90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(40.f, -0.75f, 27.5f), Vector3(0.f, 90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f)); //14
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(40.f, -0.75f, 23.0f), Vector3(0.f, 90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(40.f, -0.75f, 18.0f), Vector3(0.f, 90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(40.f, -0.75f, 13.0f), Vector3(0.f, 90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(40.f, -0.75f, 8.0f), Vector3(0.f, 90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(40.f, -0.75f, 3.0f), Vector3(0.f, 90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(40.f, -0.75f, -2.0f), Vector3(0.f, 90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(40.f, -0.75f, -7.0f), Vector3(0.f, 90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(40.f, -0.75f, -12.0f), Vector3(0.f, 90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(40.f, -0.75f, -17.0f), Vector3(0.f, 90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(40.f, -0.75f, -22.0f), Vector3(0.f, 90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(40.f, -0.75f, -27.0f), Vector3(0.f, 90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(40.f, -0.75f, -32.0f), Vector3(0.f, 90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(40.f, -0.75f, -37.0f), Vector3(0.f, 90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));

	//ステージの前の壁
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.f, -0.75f, 97.0f), Vector3(0.f, -90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.f, -0.75f, 92.0f), Vector3(0.f, -90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.f, -0.75f, 87.0f), Vector3(0.f, -90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.f, -0.75f, 82.0f), Vector3(0.f, -90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.f, -0.75f, 77.0f), Vector3(0.f, -90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.f, -0.75f, 72.0f), Vector3(0.f, -90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.f, -0.75f, 67.0f), Vector3(0.f, -90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.f, -0.75f, 62.0f), Vector3(0.f, -90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.f, -0.75f, 57.0f), Vector3(0.f, -90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.f, -0.75f, 52.0f), Vector3(0.f, -90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.f, -0.75f, 47.0f), Vector3(0.f, -90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.f, -0.75f, 42.0f), Vector3(0.f, -90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.f, -0.75f, 37.0f), Vector3(0.f, -90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.f, -0.75f, 32.0f), Vector3(0.f, -90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.f, -0.75f, 27.5f), Vector3(0.f, -90.f, 0.f), Vector3(0.4f, 0.4f, 1.0f)); //14
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.f, -0.75f, 23.0f), Vector3(0.f, -90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.f, -0.75f, 18.0f), Vector3(0.f, -90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.f, -0.75f, 13.0f), Vector3(0.f, -90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.f, -0.75f, 8.0f), Vector3(0.f, -90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.f, -0.75f, 3.0f), Vector3(0.f, -90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.f, -0.75f, -2.0f), Vector3(0.f, -90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.f, -0.75f, -7.0f), Vector3(0.f, -90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.f, -0.75f, -12.0f), Vector3(0.f, -90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.f, -0.75f, -17.0f), Vector3(0.f, -90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.f, -0.75f, -22.0f), Vector3(0.f, -90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.f, -0.75f, -27.0f), Vector3(0.f, -90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.f, -0.75f, -32.0f), Vector3(0.f, -90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-40.f, -0.75f, -37.0f), Vector3(0.f, -90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));

	//ステージの左壁
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-37.5f, -0.75f, -39.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-32.5f, -0.75f, -39.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-27.5f, -0.75f, -39.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-22.5f, -0.75f, -39.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-17.5f, -0.75f, -39.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-12.5f, -0.75f, -39.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-7.5f, -0.75f, -39.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-2.5f, -0.75f, -39.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(2.5f, -0.75f, -39.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(7.5f, -0.75f, -39.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(12.5f, -0.75f, -39.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(17.5f, -0.75f, -39.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(22.5f, -0.75f, -39.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(27.5f, -0.75f, -39.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(32.5f, -0.75f, -39.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(37.5f, -0.75f, -39.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));

	// ステージの右壁
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-37.5f, -0.75f, 99.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-32.5f, -0.75f, 99.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-27.5f, -0.75f, 99.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-22.5f, -0.75f, 99.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-17.5f, -0.75f, 99.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-12.5f, -0.75f, 99.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-7.5f, -0.75f, 99.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(-2.5f, -0.75f, 99.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(2.5f, -0.75f, 99.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.48f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(7.5f, -0.75f, 99.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(12.5f, -0.75f, 99.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(17.5f, -0.75f, 99.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(22.5f, -0.75f, 99.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(27.5f, -0.75f, 99.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(32.5f, -0.75f, 99.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(37.5f, -0.75f, 99.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));



	// 上のステージの後ろの壁
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(70.f, -0.75f, -28.f), Vector3(0.f, -90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(70.f, -0.75f, -23.f), Vector3(0.f, -90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(70.f, -0.75f, -18.f), Vector3(0.f, -90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(70.f, -0.75f, -13.f), Vector3(0.f, -90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(70.f, -0.75f, -8.f), Vector3(0.f, -90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(70.f, -0.75f, -3.f), Vector3(0.f, -90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(70.f, -0.75f, 2.f), Vector3(0.f, -90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(70.f, -0.75f, 7.f), Vector3(0.f, -90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(70.f, -0.75f, 12.f), Vector3(0.f, -90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(70.f, -0.75f, 17.f), Vector3(0.f, -90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(70.f, -0.75f, 22.f), Vector3(0.f, -90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(70.f, -0.75f, 27.f), Vector3(0.f, -90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));

	// 上のステージのueの壁
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(129.6f, -0.75f, -28.f), Vector3(0.f, 90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(129.6f, -0.75f, -23.f), Vector3(0.f, 90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(129.6f, -0.75f, -18.f), Vector3(0.f, 90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(129.6f, -0.75f, -13.f), Vector3(0.f, 90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(129.6f, -0.75f, -8.f), Vector3(0.f, 90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(129.6f, -0.75f, -3.f), Vector3(0.f, 90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(129.6f, -0.75f, 2.f), Vector3(0.f, 90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(129.6f, -0.75f, 7.f), Vector3(0.f, 90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(129.6f, -0.75f, 12.f), Vector3(0.f, 90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(129.6f, -0.75f, 17.f), Vector3(0.f, 90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(129.6f, -0.75f, 22.f), Vector3(0.f, 90.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(129.6f, -0.75f, 27.f), Vector3(0.f, 90.f, 0.f), Vector3(0.55f, 0.4f, 1.0f));

	// 上ステージの右壁
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(127.3f, -0.75f, -30.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(122.3f, -0.75f, -30.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(117.3f, -0.75f, -30.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(112.3f, -0.75f, -30.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(107.3f, -0.75f, -30.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(102.3f, -0.75f, -30.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(97.3f, -0.75f, -30.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(92.3f, -0.75f, -30.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(87.3f, -0.75f, -30.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(82.3f, -0.75f, -30.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(77.3f, -0.75f, -30.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(72.5f, -0.75f, -30.5f), Vector3(0.f, 180.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));

	// 上ステージnohidari壁
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(127.3f, -0.75f, 29.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(122.3f, -0.75f, 29.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(117.3f, -0.75f, 29.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(112.3f, -0.75f, 29.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(107.3f, -0.75f, 29.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(102.3f, -0.75f, 29.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(97.3f, -0.75f, 29.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(92.3f, -0.75f, 29.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(87.3f, -0.75f, 29.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(82.3f, -0.75f, 29.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(77.3f, -0.75f, 29.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
	Prefab::Instantiate(w, PrefabId::StageWall, Vector3(72.5f, -0.75f, 29.5f), Vector3(0.f, 0.f, 0.f), Vector3(0.5f, 0.4f, 1.0f));
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

void SceneGame::CreateStartUI()
{
	World& w = GetWorld();

	GameObject& startObj = w.CreateGameObject();
	uiStart_ = &startObj.AddComponent<UiSpriteRenderer>(TextureTag::Game_UI_Start, Vector2(0.f, -20.f), Vector2(1280.f, 720.f));
	uiStart_->SetVisible(false);

	waitTimer_.Start();
	SetGameLock(true);
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
	glassObjectSpawner.AddComponent<GlassObjectSpawner>();
}

void SceneGame::CreateWarpStage2()
{
	World& w = GetWorld();

	GameObject& warpA = w.CreateGameObject(Vector3(37.f, 0.f, 0.f), Vector3::zero(), Vector3::one());
	warpA.AddComponent< MeshFilter >(ModelTag::Warp);
	warpA.AddComponent<MeshRenderer>();
	BoxCollider& bcA = warpA.AddComponent<BoxCollider>(true); bcA.isTrigger = true;
	warpA.AddComponent<WarpPoint>(Point::AtoB);

	GameObject& warpB = w.CreateGameObject(Vector3(53.f, 0.f, 0.f), Vector3::zero(), Vector3::one());
	warpB.AddComponent< MeshFilter >(ModelTag::Warp);
	warpB.AddComponent<MeshRenderer>();
	BoxCollider& bcB = warpB.AddComponent<BoxCollider>(true); bcB.isTrigger = true;
	warpB.AddComponent<WarpPoint>(Point::BtoA);
}

void SceneGame::CreateWarpStage3()
{
	World& w = GetWorld();

	GameObject& warpC = w.CreateGameObject(Vector3(37.f, 0.f, 0.f), Vector3::zero(), Vector3::one());
	warpC.AddComponent< MeshFilter >(ModelTag::Warp);
	warpC.AddComponent<MeshRenderer>();
	BoxCollider& bcC = warpC.AddComponent<BoxCollider>(true); bcC.isTrigger = true;
	warpC.AddComponent<WarpPoint>(Point::CtoD);

	GameObject& warpD = w.CreateGameObject(Vector3(73.f, 0.f, 0.f), Vector3::zero(), Vector3::one());
	warpD.AddComponent< MeshFilter >(ModelTag::Warp);
	warpD.AddComponent<MeshRenderer>();
	BoxCollider& bcD = warpD.AddComponent<BoxCollider>(true); bcD.isTrigger = true;
	warpD.AddComponent<WarpPoint>(Point::DtoC);
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
	m_pSkyTex = TextureManager::Get(TextureTag::Game_Sky);
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
	
	if (IsGameLocked()) {

		if (waitTimer_.ElapsedRawSeconds() < WAIT_TIME) return;

		if(!CheckStartSE)
		{
			SoundManager::SoundPlay(GameStartSE,1.0f);
			CheckStartSE = true;
		}

		// スタートUIアニメーション
		if (m_animationTime < 1.0f)
		{
			uiStart_->SetVisible(true);
			m_animationTime += 1.0f / 80.0f; // 約1.3秒
			if (m_animationTime > 1.0f) m_animationTime = 1.0f;

			float bounce = Easing::EaseOutBounce(m_animationTime);
			float yPos = -800.0f + (800.0f * bounce);
			uiStart_->SetOffset({ 30.0f, yPos });
		}
		else
		{
			uiStart_->SetVisible(false);
			SetGameLock(false);
		}
		return;
	}

	if (!CheckGameSound)
	{
//		SoundManager::SoundPlay(SceneGameBGM, 0.4f);
		int GameBGM = rand() % 2;
		switch (GameBGM)
		{
		case 0:
			SoundManager::SoundPlay(SceneGameBGM1,0.4f);
			break;
		case 1:
			SoundManager::SoundPlay(SceneGameBGM2, 0.5f);
			break;
		}
		CheckGameSound = true;
	}

	// オプション画面へ
	if (InputKeyboard::GetKeyDown('O') || InputGamepad::GetButtonDown(XINPUT_GAMEPAD_START))
	{
		SceneManager::PushScene<SceneOption>();
	}
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
	if (name == "Kuukiseizyouki") return PrefabId::Kuukiseizyouki;
	if (name == "Ningixyou") return PrefabId::Ningixyou;
	if(name == "SagaraDoll") return PrefabId::SagaraDoll;

	if (name == "Benti") return PrefabId::Benti;
	if (name == "Aircon") return PrefabId::Aircon;
	if (name == "Jihanki") return PrefabId::Jihanki;
	if (name == "Reizouko") return PrefabId::Reizouko;

	if (name == "Ufo") return PrefabId::Ufo;
	if (name == "Eisei") return PrefabId::Eisei;
	if (name == "Star") return PrefabId::Star;

	// 一致するものがなかった場合、勝手にSwingにせず、エラーを出す！
	std::string errorMsg = "Unknown Prefab Name: [" + name + "]\n(Fallback to Swing)";
	MessageBox(NULL, errorMsg.c_str(), "Prefab ID Error", MB_OK);

	// とりあえず無効な値を返すか、Swing等をデフォルトにする
	return PrefabId::Swing;
}
