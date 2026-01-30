#include "SceneTitle.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "AllComponent.h"
#include "TextureManager.h"
#include "Defines.h"
#include "InputKeyboard.h"

void SceneTitle::BuildLoadTasks(std::vector<LoadTask>& outTasks)
{
	// World の中身を消す
	outTasks.emplace_back([this]
		{
			GetWorld().ClearAll();        // 既存の World API に合わせる
		});

	outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Player2); });
	outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Hammer); });

	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Title_BG); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Title_Text); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Title_Logo); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Title_Fadein); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Title_FadeinID); });
	
	// 各種オブジェクト生成タスクを追加
	outTasks.emplace_back([this] { CreateCameraAndPlayer(); });
	outTasks.emplace_back([this] { CreateCanvas(); });
	outTasks.emplace_back([this] { CreateBackGround(); });
	outTasks.emplace_back([this] { CreateTitleFont(); });
	outTasks.emplace_back([this] { CreateTitleLogo(); });
	outTasks.emplace_back([this] { CreateBreakTexture(); });
	outTasks.emplace_back([this] { CreateTitleManager(); });
	outTasks.emplace_back([this] { SceneManager::CreateFadeAnimations(this); });

	// 最後に Start を流す
	outTasks.emplace_back([this]
		{
			GetWorld().Start();
		});
		
}

void SceneTitle::Load()
{
	// 従来の同期ロードが必要なら、タスクを構築して全部回す
	std::vector<LoadTask> tasks;
	BuildLoadTasks(tasks);
	for (auto& t : tasks) t();
}

void SceneTitle::Unload()
{
	SceneManager::ReleaseFadeAnimations();  // フェードアニメーション解放
	TextureManager::Clear();
}

void SceneTitle::CreateCameraAndPlayer()
{
	World& w = GetWorld();

	// カメラ
	GameObject& cameraPivot = w.CreateGameObject(Vector3(0.f, 2.0f, -5.0f), Vector3(0.f, 0.f, 0.f));
	GameObject& mainCamera = cameraPivot.CreateChild(Camera::kInitPos, Camera::kInitRotate);
	mainCamera.AddComponent<Camera>();

	// カメラ生成時に指定した座標と同じ値を手動で入れる
	m_camStartPos = Vector3(0.f, 2.0f, -5.0f); // カメラの初期座標

	//保存
	m_mainCamera = &mainCamera;

	// プレイヤー
	GameObject& player = w.CreateGameObject(Vector3(0.f, 0.f, 0.f), Vector3(0.f, 0.f, 0.f), Vector3(2.f, 2.f, 2.f));
	player.AddComponent<MeshFilter>(ModelTag::Player2);
	player.AddComponent<MeshRenderer>();
	player.AddComponent<PlayerState>();
	//player.AddComponent<Animator>();
	//player.AddComponent<PlayerAnimation>();

	// 保存
	m_player = &player;

	// ハンマー
	GameObject& hummer = player.CreateChild(Hammer::BasePos, Hammer::BaseRota, Vector3(0.2f, 0.2f, 0.2f));
	hummer.AddComponent<MeshFilter>(ModelTag::Hammer);
	hummer.AddComponent<MeshRenderer>();
	//hummer.AddComponent<Animator>();

}

void SceneTitle::CreateCanvas()
{
	World& w = GetWorld();
	//Canvas
	GameObject& canvas = w.CreateGameObject();
	canvas.AddComponent<Canvas>();
}

void SceneTitle::CreateBackGround()
{
	World& w = GetWorld();
	//背景生成
	//3Dオブジェクトとして描画
	GameObject& backGround = w.CreateGameObject(Vector3(0.f, 0.f, 50.f));
	backGround.AddComponent<Sprite3DRenderer>(TextureTag::Title_BG, Vector2(0.f, -10.f), Vector2(115.0f, 60.0f));
}

void SceneTitle::CreateTitleFont()
{
	World& w = GetWorld();
	////文字を出力する
	m_titleFontObj = &w.CreateGameObject(Vector3(0.f, 0.f, 5.0f));
	m_titleFontObj->AddComponent<Sprite3DRenderer>(TextureTag::Title_Text,Vector2(7.5f, -2.5f),Vector2(11.0f, 11.0f)/*100差*/);
}

void SceneTitle::CreateTitleLogo()
{
	World& w = GetWorld();
	m_titleLogoObj = &w.CreateGameObject(Vector3(0.f, 0.f, 10.f));
	m_titleLogoObj->AddComponent<Sprite3DRenderer>(TextureTag::Title_Logo, Vector2(-4.7f, 5.0f), Vector2(20.0f, 20.0f));
}

void SceneTitle::CreateBreakTexture()
{
	World& w = GetWorld();
	GameObject& breakTexture = w.CreateGameObject(Vector3(0.f, -3.f, 5.0f));
	breakTexture.AddComponent<UiSpriteRenderer>(TextureTag::Title_Fadein, Vector2(0.0f, 0.0f), Vector2(1500.0f, 900.0f));
	break_ = &breakTexture.AddComponent<SpriteAnimation>(7, 7, 2, 48, false);

}

void SceneTitle::CreateTitleManager()
{
	World& w = GetWorld();
	//タイトルマネージャー生成
	GameObject& manager = w.CreateGameObject();
	manager.AddComponent<TitleManager>();
}

void SceneTitle::Update()
{
	//if (!m_breakButton) return;

	if (!m_isStarting && InputKeyboard::GetKeyDown(VK_SPACE))
	{
		m_isStarting = true;
		m_zoomTimer = 0.0f;
		
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

		if (t >= 1.0f)
		{
			m_isStarting = false;
			break_->Play();
		}
	}
	if(!m_isStarting)
	{
		UpdateTitleAnimations();
	}

}

void SceneTitle::Draw()
{
}


void SceneTitle::UpdateTitleAnimations()
{
	m_animTimer += (1.0f / 60.0f);

	//(上下揺れ)
	if (m_titleLogoObj) {
		auto* trans = m_titleLogoObj->GetComponent<Transform>();
		float yOffset = std::sin(m_animTimer * 1.5f) * 0.3f; // 1.5fが速さ、0.3fが振幅
		trans->SetPosition(Vector3(0.f, yOffset, 10.f));
	}

	//(拡大縮小)
	if (m_titleFontObj) {
		auto* trans = m_titleFontObj->GetComponent<Transform>();
		float scale = 1.0f + std::sin(m_animTimer * 2.5f) * 0.01f; // 5%くらい伸び縮み
		trans->SetScale(Vector3(scale, scale, 1.0f));
	}
}
