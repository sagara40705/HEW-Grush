// SceneTitle.cpp
#include "SceneTitle.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "AllComponent.h"
#include "TextureManager.h"
#include "Defines.h"

void SceneTitle::Load()
{
	//テクスチャーロード
	TextureManager::Load(TextureTag::Title_BG);
	TextureManager::Load(TextureTag::GaugeEffect);

	World& w = GetWorld();

	//Canvas
	GameObject& canvas = w.CreateGameObject();
	canvas.AddComponent<Canvas>();

	//背景生成
	GameObject& backGroung = w.CreateGameObject();
	backGroung.AddComponent<UiSpriteRenderer>
		(TextureTag::Title_BG, Vector2(0.f, 0.f),
			Vector2(SCREEN_WIDTH, SCREEN_HEIGHT));

	//プレイヤー画像生成
	GameObject& player = w.CreateGameObject();
	player.AddComponent<UiSpriteRenderer>(TextureTag::GaugeEffect, Vector2(590.f, 310.f), Vector2(320.f, 270.f));
	player.AddComponent<SpriteAnimation>(8, 4, 6, 5, true);

	//タイトルマネージャー生成
	GameObject& manager = w.CreateGameObject();
	manager.AddComponent<TitleManager>(&player);
}

void SceneTitle::Unload()
{
	TextureManager::Clear();
}

void SceneTitle::Update()
{
}

void SceneTitle::Draw()
{
}


