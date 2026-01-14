// SceneTitle.cpp
#include "SceneTitle.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "AllComponent.h"
#include "TextureManager.h"
#include "Defines.h"
#include"InputKeyboard.h"

void SceneTitle::Load()
{
	ModelManager::Load(ModelTag::Player);
	ModelManager::Load(ModelTag::Hammer);
	//テクスチャーロード
	TextureManager::Load(TextureTag::Title_BG);
	//TextureManager::Load(TextureTag::UI_font);
	//TextureManager::Load(TextureTag::UI_Frame);
	//TextureManager::Load(TextureTag::UI_Break);
	TextureManager::Load(TextureTag::GaugeEffect);
	//TextureManager::Load(TextureTag::FadeOut);
	TextureManager::Load(TextureTag::Title_Main);
	TextureManager::Load(TextureTag::Title_Font);
	TextureManager::Load(TextureTag::Title_Break);
	TextureManager::Load(TextureTag::Title_BreakID);
	TextureManager::Load(TextureTag::Title_Loge);
	TextureManager::Load(TextureTag::Title_Loge2);
	InputKeyboard* Input = new InputKeyboard();

	CreateCameraAndPlayer();
	InitPlayerTex();

	World& w = GetWorld();

	////Canvas
	GameObject& canvas = w.CreateGameObject();
	canvas.AddComponent<Canvas>();

	//背景生成
	//3Dオブジェクトとして描画
	GameObject& backGround = w.CreateGameObject(Vector3(0.f, 0.f,50.f));
	backGround.AddComponent<Sprite3DRenderer>(TextureTag::Title_Main, Vector2(0.f, -10.f), Vector2(96.0f, 54.0f));

	//文字生成
	GameObject& player = w.CreateGameObject();

	////===================
	////文字を出力する
	////===================
	GameObject& titleFont = w.CreateGameObject();

	/*titleFont.AddComponent<UiSpriteRenderer>(
		TextureTag::Title_Font,
		Vector2(700.0f, 60.0f),
		Vector2(550.0f, 550.0f)
	);*/

	titleFont.AddComponent<UiSpriteRenderer>(
		TextureTag::Title_Loge2 ,
		Vector2(750.0f, 5.0f),
		Vector2(470.0f, 400.0f)
	);

	GameObject& TitleBreak = w.CreateGameObject();
	
	///////==========================
	//////ここからロゴ
	//////==========================
	GameObject&TitleLogo= w.CreateGameObject();

	TitleLogo.AddComponent<UiSpriteRenderer>(
		TextureTag::Title_Loge,
		Vector2(1.0f, -100.0f),
		Vector2(650.0f, 650.0f)//100差
	);

	/*====================
	画面を割る処理を行う
	====================*/
	auto& breakUi =
		titleFont.AddComponent<BreakUiRenderer>
		(TextureTag::Title_BreakID);
	
	// Update から触れるように保存
	m_breakButton = &breakUi;

	//タイトルマネージャー生成
	GameObject& manager = w.CreateGameObject();
	manager.AddComponent<TitleManager>();

}

void SceneTitle::Unload()
{
	TextureManager::Clear();
}


void SceneTitle::CreateCameraAndPlayer()
{
	World& w = GetWorld();

	// カメラ設定（Gameと同じでOKですが、操作不能ならCameraControlは不要かも）

	//GameObject& cameraPivot = w.CreateGameObject(Vector3::zero(), Vector3(0.f, 180.f, 0.f));
	GameObject& cameraPivot = w.CreateGameObject(Vector3(0.f, 1.0f, -5.0f), Vector3(0.f, 0.f, 0.f));
	GameObject& mainCamera = cameraPivot.CreateChild(Camera::kInitPos, Camera::kInitRotate);
	mainCamera.AddComponent<Camera>();

	// プレイヤー本体（見た目とアニメーションに特化）
	GameObject& player = w.CreateGameObject(Vector3(0.f, 0.f, 0.f), Vector3(0.f, 0.f, 0.f));
	//GameObject& player = w.CreateGameObject(Vector3(0.f, 2.f, 0.f), Vector3(0.f, 0.f, 0.f));
	player.AddComponent<MeshFilter>(ModelTag::Player);
	player.AddComponent<MeshRenderer>();
	player.AddComponent<PlayerState>();
	player.AddComponent<Animator>();        // モーション再生用
	player.AddComponent<PlayerAnimation>(); // アニメーションの管理用

	// 演出用の専用スクリプト（もし自作するなら）
	// player.AddComponent<TitlePlayerEffect>(); 

	// ハンマー（見た目だけ）

	GameObject& hummer = player.CreateChild(Hammer::BasePos, Hammer::BaseRota, Vector3(0.2f, 0.2f, 0.2f));
	hummer.AddComponent<MeshFilter>(ModelTag::Hammer);
	hummer.AddComponent<MeshRenderer>();
}

void SceneTitle::InitPlayerTex()
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

void SceneTitle::Update()
{
	// breakuirenderer が存在しない場合は何もしない
	if (!m_breakButton) return;

	//// エンターキーが「押された瞬間
	if (InputKeyboard::GetKeyDown(VK_RETURN))
	{
		m_breakButton->StartBreak();
	}
}

void SceneTitle::Draw()
{
}


