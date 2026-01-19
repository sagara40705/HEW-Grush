#include "LoadingScene.h"
#include "GameObject.h"
#include "AllComponent.h"
#include "TextureManager.h"
#include "Effekseer.h"
#include "ModelManager.h"

void LoadingScene::OriginalLoad()
{
	// テクスチャのロード
	TextureManager::Load(TextureTag::LoadGauge);
	TextureManager::Load(TextureTag::Loading_BG);
	TextureManager::Load(TextureTag::Loading_Txt);
	
	// ★重要: 3Dモデルのロード
	ModelManager::Load(ModelTag::Player);

	World& w = GetWorld();

	//// ====================
	//// カメラを先に作成
	//// ====================
	//GameObject& camera = w.CreateGameObject
	//(
	//	Vector3(0.0f, 1.0f, -8.0f),      // カメラ位置
	//	Vector3(0.0f, 0.0f, 0.0f)        // カメラ回転
	//);
	//Camera& camRef = camera.AddComponent<Camera>();
	//Camera* cam = &camRef;
	//cam->tag = CameraTag::Main;  // メインカメラとして設定
	//cam->SetPerspective(
	//	DirectX::XMConvertToRadians(60.0f),  // FOV
	//	16.0f / 9.0f,                         // アスペクト比
	//	0.1f,                                 // Near
	//	1000.0f                               // Far
	//);

	//// ====================
	//// 背景（3D空間の奥に配置）
	//// ====================
	//GameObject& bgPlane = w.CreateGameObject
	//(
	//	Vector3(0.0f, 0.0f, 20.0f),   // カメラの奥に配置（カメラからZ+方向）
	//	Vector3(0.0f, 0.0f, 0.0f),     // 回転なし
	//	Vector3(20.0f, 12.0f, 1.0f)    // 16:9の比率で大きなサイズ
	//);
	//// 背景用のプリミティブメッシュとテクスチャを設定
	//bgPlane.AddComponent<MeshFilter>(PrimitiveMeshTag::Quad); // 平面メッシュ
	//auto& bgRenderer = bgPlane.AddComponent<MeshRenderer>();

	//// ====================
	//// 3Dキャラクター (背景より手前に配置)
	//// ====================
	//GameObject& character3D = w.CreateGameObject(
	//	Vector3(2.0f, 0.0f, 0.0f),       // 画面右側、カメラの近くに配置
	//	Vector3(0.0f, 0.0f, 0.0f),     // 少し回転させておく
	//	Vector3(1.5f, 1.5f, 1.5f)        // スケール
	//);
	//character3D.AddComponent<MeshFilter>(ModelTag::Player);
	//character3D.AddComponent<MeshRenderer>();
	//character3D.AddComponent<LoadingCharacterAnimation>();

	// ====================
	// UI要素（最前面）
	// ====================
	//Canvas
	GameObject& canvas = w.CreateGameObject();
	canvas.AddComponent<Canvas>();

	// 背景
	GameObject& loadBG = w.CreateGameObject();
	loadBG.AddComponent<UiSpriteRenderer>(TextureTag::Loading_BG, Vector2(0.0f, .0f), Vector2(1280.0f, 720.0f));


	// ローディング進捗ゲージ
	GameObject& loadGauge = w.CreateGameObject();
	loadGauge.AddComponent<UiSpriteRenderer>(TextureTag::LoadGauge, Vector2(0.0f, 0.0f), Vector2(500.0f, 50.0f));
	loadGauge.AddComponent<LoadingGauge>(this);

	// ロード中
	GameObject& loadingText = w.CreateGameObject();
	loadingText.AddComponent<UiSpriteRenderer>(TextureTag::Loading_Txt, Vector2(0.0f, 100.0f), Vector2(400.0f, 400.0f));

	
}
