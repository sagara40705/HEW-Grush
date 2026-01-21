#include "LoadingScene.h"
#include "GameObject.h"
#include "AllComponent.h"
#include "TextureManager.h"
#include "Effekseer.h"
#include "ModelManager.h"

SpriteAnimation* loadingAnim = nullptr;

void LoadingScene::OriginalLoad()
{
	// テクスチャのロード
	TextureManager::Load(TextureTag::LoadGauge);
	TextureManager::Load(TextureTag::Loading_BG);
	TextureManager::Load(TextureTag::Loading_Txt);
	TextureManager::Load(TextureTag::Loading_Character);
	
	// ★重要: 3Dモデルのロード
	ModelManager::Load(ModelTag::Player);

	World& w = GetWorld();

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

	// キャラクター
	GameObject& loadingCharacter = w.CreateGameObject();
	loadingCharacter.AddComponent<UiSpriteRenderer>(TextureTag::Loading_Character, Vector2(860.0f, 400.0f), Vector2(400.0f, 300.0f));
	loadingAnim = &loadingCharacter.AddComponent<SpriteAnimation>(1, 10, 3, 10, true);
	
	loadingAnim->Play();// アニメーション再生開始
	
}
