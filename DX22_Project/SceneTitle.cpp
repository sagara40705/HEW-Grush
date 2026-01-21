//// SceneTitle.cpp
//#include "SceneTitle.h"
//#include "SceneManager.h"
//#include "GameObject.h"
//#include "AllComponent.h"
//#include "TextureManager.h"
//#include "Defines.h"
//
//void SceneTitle::Load()
//{
//	//テクスチャーロード
//	TextureManager::Load(TextureTag::Title_BG);
//	TextureManager::Load(TextureTag::GaugeEffect);
//
//	World& w = GetWorld();
//
//	//Canvas
//	GameObject& canvas = w.CreateGameObject();
//	canvas.AddComponent<Canvas>();
//
//	//背景生成
//	GameObject& backGroung = w.CreateGameObject();
//	backGroung.AddComponent<UiSpriteRenderer>(TextureTag::Title_BG, Vector2(0.f, 0.f), Vector2(SCREEN_WIDTH, SCREEN_HEIGHT));
//
//	//プレイヤー画像生成
//	GameObject& player = w.CreateGameObject();
//	player.AddComponent<UiSpriteRenderer>(TextureTag::GaugeEffect, Vector2(590.f, 310.f), Vector2(320.f, 270.f));
//	player.AddComponent<SpriteAnimation>(8, 4, 6, 5, true);
//
//	//タイトルマネージャー生成
//	GameObject& manager = w.CreateGameObject();
//	manager.AddComponent<TitleManager>(&player);
//}
//
//void SceneTitle::Unload()
//{
//	TextureManager::Clear();
//}
//
//void SceneTitle::Update()
//{
//}
//
//void SceneTitle::Draw()
//{
//}

//SceneTitle.cpp
#include "SceneTitle.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "AllComponent.h"
#include "TextureManager.h"
#include "Defines.h"
#include "InputKeyboard.h"

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
	//InitPlayerTex();

	// カメラ生成時に指定した座標と同じ値を手動で入れる
	m_camStartPos = Vector3(0.f, 2.0f, -5.0f);


	World& w = GetWorld();

	////Canvas
	GameObject& canvas = w.CreateGameObject();
	canvas.AddComponent<Canvas>();

	//背景生成
	//3Dオブジェクトとして描画
	GameObject& backGround = w.CreateGameObject
	(Vector3(0.f, 0.f, 50.f));
	backGround.AddComponent<Sprite3DRenderer>(TextureTag::Title_Main, Vector2(0.f, -10.f), Vector2(115.0f, 60.0f));

	//文字生成
	GameObject& player = w.CreateGameObject();

	////===================
	////文字を出力する
	////===================
	GameObject& titleFont = w.CreateGameObject(
		Vector3(0.f, 0.f, 5.0f));

	titleFont.AddComponent<Sprite3DRenderer>(
		TextureTag::Title_Font,
		Vector2(7.5f, -2.5f),
		Vector2(11.0f, 11.0f)//100差
	);

	GameObject& TitleBreak = w.CreateGameObject();

	///////==========================
	//////ここからロゴ
	//////==========================
	//GameObject&TitleLogo= w.CreateGameObject();

	//TitleLogo.AddComponent<UiSpriteRenderer>(
	//	TextureTag::Title_Loge,
	//	Vector2(1.0f, -100.0f),
	//	Vector2(650.0f, 650.0f)//100差
	//);
	GameObject& TitleLogo = w.CreateGameObject(
		Vector3(0.f, 0.f, 10.f)    // 座標
	);

	TitleLogo.AddComponent<Sprite3DRenderer>(
		TextureTag::Title_Loge,
		Vector2(-4.7f, 5.0f),
		Vector2(20.0f, 20.0f)//100差
	);

	/*====================
	画面を割る処理を行う
	====================*/

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

	// カメラ
	GameObject& cameraPivot = w.CreateGameObject(Vector3(0.f, 2.0f, -5.0f), Vector3(0.f, 0.f, 0.f));
	/*GameObject& cameraPivot = w.CreateGameObject(Vector3(0.f, 0.5f, 1.f), Vector3(0.f, 0.f, 0.f));*/
	GameObject& mainCamera = cameraPivot.CreateChild(Camera::kInitPos, Camera::kInitRotate);
	mainCamera.AddComponent<Camera>();

	// ★ 保存
	m_mainCamera = &mainCamera;

	// プレイヤー
	GameObject& player = w.CreateGameObject(Vector3(0.f, 0.f, 0.f), Vector3(0.f, 0.f, 0.f), Vector3(2.f, 2.f, 2.f));
	player.AddComponent<MeshFilter>(ModelTag::Player);
	player.AddComponent<MeshRenderer>();
	player.AddComponent<PlayerState>();
	player.AddComponent<Animator>();
	player.AddComponent<PlayerAnimation>();

	// ★ 保存
	m_player = &player;

	// ハンマー
	GameObject& hummer = player.CreateChild(Hammer::BasePos, Hammer::BaseRota, Vector3(0.2f, 0.2f, 0.2f));
	hummer.AddComponent<MeshFilter>(ModelTag::Hammer);
	hummer.AddComponent<MeshRenderer>();

}

void SceneTitle::Update()
{

	//if (!m_breakButton) return;

	if (!m_isStarting && InputKeyboard::GetKeyDown(VK_SPACE))
	{
		m_isStarting = true;
		m_zoomTimer = 0.0f;
		//m_breakButton->StartBreak();

		// ★ 保存しておいた m_player を使う
		if (m_player) {
			auto* pState = m_player->GetComponent<PlayerState>();
			if (pState) pState->actionedThisFrame[ToIndex(ActionId::SpinAttack)] = true;
		}


	}

	if (m_isStarting)
	{
		m_zoomTimer += (1.0f / 60.0f);
		float t = m_zoomTimer / ZOOM_DURATION;
		if (t > 1.0f) t = 1.0f;

		float easeT = t * t * t;

		if (m_mainCamera)
		{
			// 1. Transformコンポーネントを取得
			auto* transform = m_mainCamera->GetComponent<Transform>();

			if (transform)
			{
				Vector3 currentPos = Vector3::Lerp(m_camStartPos, m_camEndPos, easeT);

				// 2. 座標を設定 (SetPosition または pos への代入)
				transform->SetPosition(currentPos);
			}
		}

		if (t >= 1.0f) m_isStarting = false;
	}

}

void SceneTitle::Draw()
{
}

