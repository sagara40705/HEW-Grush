#include "SceneGame.h"
#include "GameObject.h"
#include "AllComponent.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "Prefab.h"
#include "PrefabSet.h"
#include "LineManager.h"
#include "DebugLog.h"

Model* g_pModel;

SceneGame::SceneGame()
{
	g_pModel = new Model();
	const char* fileName = "Assets/Model/playerchara_20251202/space_suit_12.fbx";
	if (!g_pModel->Load(fileName, 0.005))
	{ // 倍率と反転は省略可 
		MessageBox(NULL, "読み込んだファイル名", "Error", MB_OK);
	}

	Model::AnimeNo jumpAnim = g_pModel->AddAnimation("Assets/Model/animation_taiki1.fbx");

	g_pModel->PlayAnime(jumpAnim, true, 1.0f);

}

SceneGame::~SceneGame()
{
	if (g_pModel)
	{
		delete g_pModel;
		g_pModel = nullptr;
	}
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
	outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Benti); });
	outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Ufo); });
	outTasks.emplace_back([this] { ModelManager::Load(ModelTag::ProtPlay); });
	outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Player); });

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

	GameObject& cameraPivot = w.CreateGameObject(Vector3::zero(), Vector3::zero(), Vector3::one());
	cameraPivot.SetTag(Tag::CamreraPivot);
	cameraPivot.AddComponent<CameraControl>();

	GameObject& mainCamera = cameraPivot.CreateChild(Camera::kInitPos, Camera::kInitRotate, Vector3::one());
	mainCamera.SetTag(Tag::MainCamera);
	mainCamera.AddComponent<Camera>();
	mainCamera.AddComponent<CameraShake>();

	GameObject& player = w.CreateGameObject(Vector3(-3.f, 2.f, 0.f), Vector3(0.f, 0.f, 0.f));
	player.SetTag(Tag::Player);
	player.AddComponent<MeshFilter>(ModelTag::Player);
	player.AddComponent<MeshRenderer>();
	player.AddComponent<Rigidbody>();
	player.AddComponent<BoxCollider>();
	player.AddComponent<PlayerState>();
	player.AddComponent<PlayerMove>();
	player.AddComponent<PlayerAction>();
	player.AddComponent<Animator>();


	GameObject& hummer = 
		player.CreateChild(Hammer::BasePos, Vector3::zero(), Vector3(0.3f, 0.3f, 0.3f));
	hummer.SetTag(Tag::Hammer);
	hummer.AddComponent<MeshFilter>(PrimitiveMeshTag::Cube);
	hummer.AddComponent<MeshRenderer>();
	hummer.AddComponent<BoxCollider>();
	hummer.AddComponent<Hammer>();
}

void SceneGame::CreateStage()
{
	World& w = GetWorld();

	GameObject& floor = w.CreateGameObject(Vector3(0.f, -1.f, 0.f), Vector3::zero(), Vector3(50.f, 0.1f, 50.f));
	floor.AddComponent<MeshFilter>(PrimitiveMeshTag::Cube);
	floor.AddComponent<MeshRenderer>();
	BoxCollider& collider = floor.AddComponent<BoxCollider>();
	collider.layer = Layer::Ground;


	//UFO
	Prefab::Instantiate(w, PrefabId::Ufo, Vector3(10, -0.5f, 10), Vector3::zero(), Vector3::one());

	//ベンチ
	Prefab::Instantiate(w, PrefabId::Benti, Vector3(-10, -0.9f, 10), Vector3::zero(), Vector3(1.2f, 1.2f, 1.2f));
	Prefab::Instantiate(w, PrefabId::Benti, Vector3(20, -0.9f, -16), Vector3::zero(), Vector3(1.2f, 1.2f, 1.2f));
	Prefab::Instantiate(w, PrefabId::Benti, Vector3(11, -0.9f, -13), Vector3::zero(), Vector3(1.2f, 1.2f, 1.2f));

	//すぃんぐ
	Prefab::Instantiate(w, PrefabId::Swing, Vector3(-7, -0.9f, -8), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
	Prefab::Instantiate(w, PrefabId::Swing, Vector3(-12, -0.9f, -2), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
	Prefab::Instantiate(w, PrefabId::Swing, Vector3(-22, -0.9f, -13), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));

	//プロト
	Prefab::Instantiate(w, PrefabId::ProtPlay, Vector3(-12, -0.9f, 8), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
	Prefab::Instantiate(w, PrefabId::ProtPlay, Vector3(5, -0.9f, 2), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
	Prefab::Instantiate(w, PrefabId::ProtPlay, Vector3(-20, -0.9f, -20), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
	Prefab::Instantiate(w, PrefabId::ProtPlay, Vector3(-16, -0.9f, -16), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
	Prefab::Instantiate(w, PrefabId::ProtPlay, Vector3(13, -0.9f, 2), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
	Prefab::Instantiate(w, PrefabId::ProtPlay, Vector3(22, -0.9f, 8), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
	Prefab::Instantiate(w, PrefabId::ProtPlay, Vector3(-17, -0.9f, 15), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
	Prefab::Instantiate(w, PrefabId::ProtPlay, Vector3(6, -0.9f, 17), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
	Prefab::Instantiate(w, PrefabId::ProtPlay, Vector3(20, -0.9f, -12), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
	Prefab::Instantiate(w, PrefabId::ProtPlay, Vector3(14, -0.9f, -19), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
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
	//scoreManager.AddComponent<UiSpriteRenderer>(TextureTag::ScoreNumber, Vector2(400.f, 0.f), Vector2(320.f, 270.f));

	GameObject& scoreUI = w.CreateGameObject();
	scoreUI.AddComponent<ScoreUI>();
}

void SceneGame::Update()
{
	g_pModel->Step(1.0f / 60.0f);

}

void SceneGame::Draw()
{
	// 頂点シェーダーに渡す変換行列の変数を宣言 
	DirectX::XMFLOAT4X4 fWVP[3];    // World,View,Projectionの略  
	DirectX::XMMATRIX world, view, proj; // 各変換行列の格納先 


	// 作成した行列を各変数へ格納 
	//ワールド座標用
	//world = DirectX::XMMatrixRotationY(0.0f);

	world = DirectX::XMMatrixIdentity();

	//ビュー座標用

	//view = DirectX::XMMatrixLookAtLH(
	//	DirectX::XMVectorSet(0.0f, 2.0f, -3.0f, 0.0f),
	//	DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
	//	DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	//);
	view = DirectX::XMLoadFloat4x4(&Camera::Main()->GetView());

	//プロジェクション座標用
	//proj = DirectX::XMMatrixPerspectiveFovLH(70.0f, 16.0f / 9.0f, 0.1f, 10.0f);
	proj = DirectX::XMLoadFloat4x4(&Camera::Main()->GetProjection());

	// 計算用のデータから読み取り用のデータに変換 
	DirectX::XMStoreFloat4x4(&fWVP[0], DirectX::XMMatrixTranspose(world));
	DirectX::XMStoreFloat4x4(&fWVP[1], DirectX::XMMatrixTranspose(view));
	DirectX::XMStoreFloat4x4(&fWVP[2], DirectX::XMMatrixTranspose(proj));
	
	//ボーン行列をシェーダーに設定
	//200はModelクラスの最大ボーン数に合わせる
	for (unsigned int i = 0; i < g_pModel->GetMeshNum(); ++i)
	{
		const Model::Mesh* mesh = g_pModel->GetMesh(i);
		if (!mesh) continue;

		// ボーン行列をシェーダーに設定
		DirectX::XMFLOAT4X4 bones[200]; // Model::MAX_BONE を使ってもOK
		for (int boneIdx = 0; boneIdx < mesh->bones.size(); ++boneIdx)
		{
			const Model::Bone& bone = mesh->bones[boneIdx];
			DirectX::XMStoreFloat4x4(
				&bones[boneIdx],
				DirectX::XMMatrixTranspose(
					bone.invOffset * g_pModel->GetBoneMatrix(bone.nodeIndex)
				)
			);
		}

		ShaderList::SetBones(bones);

		// メッシュ描画（必要ならここで）
		//g_pModel->Draw(i);
	}

	// モデルに使用する頂点シェーダー、ピクセルシェーダーを設定 
	g_pModel->SetVertexShader(ShaderList::GetVS(ShaderList::VS_ANIME));
	g_pModel->SetPixelShader(ShaderList::GetPS(ShaderList::PS_LAMBERT));


	// マテリアル別にメッシュを表示 
	for (unsigned int i = 0; i < g_pModel->GetMeshNum(); ++i) {
		// モデルのメッシュを取得 
		const Model::Mesh* mesh = g_pModel->GetMesh(i);
		// メッシュに割り当てられているマテリアルを取得 
		Model::Material material = *g_pModel->GetMaterial(mesh->materialID);
		// シェーダーへマテリアルを設定 
		ShaderList::SetMaterial(material);
		// モデルの描画 
		g_pModel->Draw(i);
	}
}


