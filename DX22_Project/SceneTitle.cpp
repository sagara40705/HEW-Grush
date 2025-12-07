// SceneTitle.cpp
#include "SceneTitle.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "AllComponent.h"
#include "TextureManager.h"
#include "Defines.h"
#include"BreakButtonRenderer.h"

void SceneTitle::Load()
{
	//テクスチャーロード
	TextureManager::Load(TextureTag::Title_BG);
	TextureManager::Load(TextureTag::UI_font);
	TextureManager::Load(TextureTag::UI_Frame);
	TextureManager::Load(TextureTag::UI_Break);
	TextureManager::Load(TextureTag::GaugeEffect);


	World& w = GetWorld();

	//Canvas
	GameObject& canvas = w.CreateGameObject();
	canvas.AddComponent<Canvas>();

	////背景生成
	//GameObject& backGroung = w.CreateGameObject();
	//backGroung.AddComponent<UiSpriteRenderer>(TextureTag::Title_BG, Vector2(0.f, 0.f), Vector2(SCREEN_WIDTH, SCREEN_HEIGHT));


	//プレイヤー画像生成
	GameObject& player = w.CreateGameObject();
	player.AddComponent<UiSpriteRenderer>(TextureTag::GaugeEffect, Vector2(590.f, 310.f), Vector2(320.f, 270.f));
	player.AddComponent<SpriteAnimation>(8, 4, 6, 5, true);

	//フレーム
	GameObject& CanvasFrame = w.CreateGameObject();
	CanvasFrame.AddComponent<UiSpriteRenderer>(TextureTag::UI_Frame, Vector2(500.0f, 250.0f), Vector2(200.0f, 100.0f));

	auto& btn = CanvasFrame.AddComponent<BreakUiRenderer>(
		Vector2(500, 250),
		Vector2(200, 100),
		TextureTag::UI_Frame,
		TextureTag::UI_Break,
		TextureTag::UI_BreakID
	);

	// ボタンが押された瞬間に
	btn.StartBreak();

	////割れたフレーム
	//GameObject& CanvasFont = w.CreateGameObject();
	//CanvasFont.AddComponent<UiSpriteRenderer>(TextureTag::UI_font, Vector2(5.0f, 5.0f), Vector2(100.0f, 100.0f));

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


