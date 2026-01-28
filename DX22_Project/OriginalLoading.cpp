#include "LoadingScene.h"
#include "GameObject.h"
#include "AllComponent.h"
#include "TextureManager.h"
#include "Effekseer.h"
#include "ModelManager.h"

SpriteAnimation* loadChara = nullptr;

void LoadingScene::OriginalLoad()
{
	// テクスチャのロード
	TextureManager::Load(TextureTag::Load_BG);
	TextureManager::Load(TextureTag::Load_Character);
	TextureManager::Load(TextureTag::Load_Gauge_Bar);
	TextureManager::Load(TextureTag::Load_Gauge_Frame);
	TextureManager::Load(TextureTag::Load_Text);
	
	// ★重要: 3Dモデルのロード
	ModelManager::Load(ModelTag::Player);

	World& w = GetWorld();

	//Canvas
	GameObject& canvas = w.CreateGameObject();
	canvas.AddComponent<Canvas>();

	// 背景
	GameObject& loadBG = w.CreateGameObject();
	loadBG.AddComponent<UiSpriteRenderer>(TextureTag::Load_BG, Vector2(0.0f, .0f), Vector2(1280.0f, 720.0f));

	// ロード中
	GameObject& loadingText = w.CreateGameObject();
	loadingText.AddComponent<UiSpriteRenderer>(TextureTag::Load_Text, Vector2(0.0f,0.0f), Vector2(1280.0f, 720.0f));

	// ローディング進捗ゲージ
	GameObject& loadGauge = w.CreateGameObject();
	loadGauge.AddComponent<UiSpriteRenderer>(TextureTag::Load_Gauge_Bar, Vector2(0.0f, 50.0f), Vector2(500.0f, 100.0f));
	loadGauge.AddComponent<LoadingGauge>(this);
	
	// ロード中キャラアニメーション
	GameObject& loadCharaObj = w.CreateGameObject();
	loadCharaObj.AddComponent<UiSpriteRenderer>(TextureTag::Load_Character, Vector2(860.0f,400.0f), Vector2(400.0f, 300.0f));
	loadChara = &loadCharaObj.AddComponent<SpriteAnimation>(1, 10, 1, 10, true);

	loadChara->Play();// アニメーション再生

}
