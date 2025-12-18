#include "SceneGame.h"
#include "GameObject.h"
#include "AllComponent.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "Prefab.h"
#include "PrefabSet.h"
#include "LineManager.h"
#include "Camera.h"
#include "Transform.h"

SceneGame::SceneGame()
{
	using namespace DirectX;

	// ここでは行列の初期化とポインタ初期化だけにしておくイメージ
	XMStoreFloat4x4(&m_world, DirectX::XMMatrixIdentity());
	XMStoreFloat4x4(&m_view, DirectX::XMMatrixIdentity());
	XMStoreFloat4x4(&m_proj, DirectX::XMMatrixIdentity());

	m_pDepthNoWrite = nullptr;
	m_pSkyTex = nullptr;

}

SceneGame::~SceneGame()
{
	if (m_pDepthNoWrite)
	{
		m_pDepthNoWrite->Release();
		m_pDepthNoWrite = nullptr;
	}
}
void SceneGame::Load()
{

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


	// ★ プレイヤーモデルに手動でテクスチャを割り当て
	outTasks.emplace_back([this]
		{
			InitPlayerTex();
		});


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



	// ★ここでスカイドームを初期化するタスクを挟む
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Background_Game3); });

	outTasks.emplace_back([this]
		{
			InitSkyDome();
		});


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
	Animator& anime = player.AddComponent<Animator>();
	Model::AnimeNo animeNo = ModelManager::Get(ModelTag::Player)->AddAnimation("Assets/Model/playerchara_20251202/space_suit_12.fbx");
	anime.Play(animeNo, true, 1.0f);
	//ModelManager::Get(ModelTag::Player)->Play(animeNo, true, 1.0f);

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

void SceneGame::InitPlayerTex()
{
	// プレイヤーモデル取得
	Model* model = ModelManager::Get(ModelTag::Player);
	if (!model)
	{
		return; // 読み込み失敗などの保険
	}

	// 体（lambert1）
	Texture* texBody = new Texture();
	texBody->Create("Assets/Model/playerchara_20251202/lambert1.png");

	if(texBody==NULL)
	{
		MessageBox(NULL, "a", "Error", MB_OK); // エラーメッセージの表示
	}

	// 頭皮・顔上部（lambert4）
	Texture* texFaceA = new Texture();
	texFaceA->Create("Assets/Model/playerchara_20251202/lambert4.png");

	if(texFaceA==NULL)
	{
		MessageBox(NULL, "aaa", "Error", MB_OK); // エラーメッセージの表示
	}

	// 目・口（lambert5）
	Texture* texFaceB = new Texture();
	texFaceB->Create("Assets/Model/playerchara_20251202/lambert5.png");

	if(texFaceB)
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
    m_pSkyTex = TextureManager::Get(TextureTag::Background_Game3);
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

void SceneGame::Update()
{
}

void SceneGame::Draw()
{
	// まずスカイドーム
	SkyDomeDorw();
}


