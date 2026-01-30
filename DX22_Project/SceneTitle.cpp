#include "SceneTitle.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "AllComponent.h"
#include "TextureManager.h"
#include "Defines.h"
#include "InputKeyboard.h"
#include "InputGamePad.h"
#include "SoundManager.h"

void SceneTitle::BuildLoadTasks(std::vector<LoadTask>& outTasks)
{
	// World の中身を消す
	outTasks.emplace_back([this]
		{
			GetWorld().ClearAll();        // 既存の World API に合わせる
		});

	outTasks.emplace_back([this] { ModelManager::Clear(); });	// モデルマネージャー初期化
	outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Player); });
	outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Hammer_3); });
	outTasks.emplace_back([this] { ModelManager::Load(ModelTag::SagaraDoll); });
	outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Ufo); });
	outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Swing); });

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
	outTasks.emplace_back([this] { CreateObject(); });

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
	SoundManager::StopSound(TitleBGM);
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
	player.AddComponent<MeshFilter>(ModelTag::Player);
	player.AddComponent<MeshRenderer>();
	player.AddComponent<PlayerState>();
	//player.AddComponent<Animator>();
	//player.AddComponent<PlayerAnimation>();

	// 保存
	m_player = &player;

	// ハンマー
	GameObject& hummer = player.CreateChild(Hammer::BasePos, Hammer::BaseRota, Vector3(0.2f, 0.2f, 0.2f));
	hummer.AddComponent<MeshFilter>(ModelTag::Hammer_3);
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
	m_titleFontObj->AddComponent<Sprite3DRenderer>(TextureTag::Title_Text, Vector2(8.5f, -3.5f), Vector2(17.0f, 17.0f));
}

void SceneTitle::CreateTitleLogo()
{
	World& w = GetWorld();
	m_titleLogoObj = &w.CreateGameObject(Vector3(0.f, 0.f, 40.f));
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

void SceneTitle::CreateObject()
{
	World& w = GetWorld();
	go1 = &Prefab::Instantiate(w, PrefabId::SagaraDoll, Vector3(-4.7f, -50.f, 1.f), Vector3(0.f, 45.f, 0.f), Vector3(4.f, 4.f, 4.f));
	World& u = GetWorld();
	go2 = &Prefab::Instantiate(u, PrefabId::Ufo, Vector3(-4.7f, 0.f, 1.f), Vector3(0.f, 20.f, -10.f), Vector3(2.f, 2.f, 2.f));
	World& v = GetWorld();
	go3 = &Prefab::Instantiate(v, PrefabId::Swing, Vector3(15.f, 0.f, 1.f), Vector3(0.f, -20.f, 10.f), Vector3(3.f, 3.f, 3.f));

}

void SceneTitle::Update()
{
	//if (!m_breakButton) return;

	// タイトルBGM再生
	if (!CheckTitleSound)
	{
		SoundManager::SoundPlay(TitleBGM, 0.6f);
		CheckTitleSound = true;
	}

	if (!m_isStarting && ( InputKeyboard::GetKeyDown(VK_SPACE) || InputGamepad::GetButtonDown(XINPUT_GAMEPAD_X)))
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
			SoundManager::SoundPlay(TitleStartSE, 1.0f);
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

	if (m_titleFontObj) {

		auto* trans = m_titleFontObj->GetComponent<Transform>();
		float yOffset = std::sin(m_animTimer * 1.5f) * 0.3f; // 1.5fが速さ、0.3fが振幅
		trans->SetPosition(Vector3(2.f, yOffset, 15.f));
	}
	const float ySpeed = 1.5f;   // 上下の速さ
	const float yRange = 0.3f;   // 上下の幅
	const float xSpeed = 0.8f;   // 左右の揺れの速さ
	const float xRange = 0.5f;   // 左右の揺れの幅
	if (go2)
	{
		auto* trans = go2->GetComponent<Transform>();

		// 常に右へ移動し続け、画面端でループさせる例（-20 から 20 まで）
		static float moveX = 10.0f;
		moveX += 0.05f; // 移動速度
		if (moveX > 20.0f) moveX = -20.0f;

		float y = std::sin(m_animTimer * 2.0f) * 0.5f; // UFOなので少し速めに揺らす

		trans->SetPosition(Vector3(moveX, y + 2.0f, 30.0f));
	}
	
	// オブジェクトごとの動きを計算するラムダ関数
	// 引数：(対象, 元の座標, 移動の幅, 揺れの速さ, 位相のズレ)
	auto ApplySpaceFloat = [&](GameObject* obj, Vector3 basePos, float range, float speed, float offset) {
		if (!obj) return;
		auto* trans = obj->GetComponent<Transform>();

		// 1. 座標の計算 (XとYで周期を変えて「8の字」や「円」を描くようにする)
		float x = basePos.x + std::sin(m_animTimer * speed + offset) * range;
		float y = basePos.y + std::cos(m_animTimer * (speed * 0.7f) + offset) * (range * 0.8f);
		float z = basePos.z + std::sin(m_animTimer * (speed * 0.5f) + offset) * (range * 0.5f);
		trans->SetPosition(Vector3(x, y, z));

		// 2. 回転の計算 (ここが宇宙っぽさのキモ)
		// 常に同じ軸で回るのではなく、ゆらゆらと傾かせる
		float rotX = std::sin(m_animTimer * (speed * 0.4f) + offset) * 10.0f;
		float rotY = m_animTimer * 5.0f + offset; // ゆっくり自転させる
		float rotZ = std::cos(m_animTimer * (speed * 0.3f) + offset) * 5.0f;
		trans->SetRotation(Vector3(rotX, rotY, rotZ));
		};

	// 各オブジェクトに宇宙漂流アニメーションを適用
	// 位相(最後の引数)をバラバラにすることで、別々の動きに見えるようになります
	ApplySpaceFloat(go1, Vector3(15.f, -8.f, 30.f), 2.0f, 0.5f, 0.0f);   // 人形
	ApplySpaceFloat(go3, Vector3(-15.f, -15.f, 30.f), 3.0f, 0.4f, 5.0f); // スイング
}

