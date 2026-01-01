#include "Gauge.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "AllComponent.h"
#include "TextureManager.h"
#include "Defines.h"
#include "InputKeyboard.h"
#include "SceneGame.h"


//void GaugeUI::Load()
//{
//	//テクスチャーロード
//	TextureManager::Load(TextureTag::Frame);
//	TextureManager::Load(TextureTag::Gauge);
//
//	World& w = GetWorld();
//
//	//Canvas
//	GameObject& canvas = w.CreateGameObject();
//	canvas.AddComponent<Canvas>();
//
//	//背景生成
//	GameObject& backGroung = w.CreateGameObject();
//	backGroung.AddComponent<UiSpriteRenderer>(TextureTag::Frame, Vector2(0.f, 0.f), Vector2(100,100));
//
//	//プレイヤー画像生成
//	GameObject& player = w.CreateGameObject();
//	player.AddComponent<UiSpriteRenderer>(TextureTag::Gauge, Vector2(590.f, 310.f), Vector2(320.f, 270.f));
////	player.AddComponent<SpriteAnimation>(8, 4, 6, 5, true);
//
//	//タイトルマネージャー生成
//	GameObject& manager = w.CreateGameObject();
//	manager.AddComponent<TitleManager>(&player);
//
//
//}
//void GaugeUI::Draw()
//{
//}
//
//void GaugeUI::Unload()
//{
//}
//
//
//
//void GaugeUI::Update()
//{
//}
