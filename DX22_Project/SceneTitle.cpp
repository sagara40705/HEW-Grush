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
	//テクスチャーロード
	TextureManager::Load(TextureTag::Title_BG);
	TextureManager::Load(TextureTag::UI_font);
	TextureManager::Load(TextureTag::UI_Frame);
	TextureManager::Load(TextureTag::UI_Break);
	TextureManager::Load(TextureTag::GaugeEffect);
	TextureManager::Load(TextureTag::FadeOut);

	//InputKeyboard* Input = new InputKeyboard();

	World& w = GetWorld();

	//Canvas
	GameObject& canvas = w.CreateGameObject();
	canvas.AddComponent<Canvas>();

	////背景生成
	//GameObject& backGroung = w.CreateGameObject();
	//backGroung.AddComponent<UiSpriteRenderer>(TextureTag::Title_BG, Vector2(0.f, 0.f), Vector2(SCREEN_WIDTH, SCREEN_HEIGHT));


	//プレイヤー画像生成
	GameObject& player = w.CreateGameObject();

	////フレーム
	GameObject& CanvasFrame = w.CreateGameObject();
	CanvasFrame.AddComponent<UiSpriteRenderer>(TextureTag::UI_Frame, Vector2(500.0f, 250.0f), Vector2(200.0f, 100.0f));

	// ★ BreakUiRenderer を追加
	auto& btn = CanvasFrame.AddComponent<BreakUiRenderer>(
		Vector2(500, 250),
		Vector2(200, 100),
		TextureTag::UI_Frame,
		TextureTag::UI_Break,
		TextureTag::UI_BreakID
	);

	// ★ Update から触れるように保存
	m_breakButton = &btn;

	////割れたフレーム
	GameObject& CanvasFont = w.CreateGameObject();
	CanvasFont.AddComponent<UiSpriteRenderer>(TextureTag::UI_font, Vector2(5.0f, 5.0f), Vector2(100.0f, 100.0f));

	//タイトルマネージャー生成
	GameObject& manager = w.CreateGameObject();
	manager.AddComponent<TitleManager>();
}

void SceneTitle::Unload()
{
	TextureManager::Clear();
}

void SceneTitle::Update()
{
	// BreakUiRenderer が存在しない場合は何もしない
	if (!m_breakButton) return;

	// エンターキーが「押された瞬間」
	if (InputKeyboard::GetKeyDown(VK_RETURN))
	{
		// すでに割れていなければ割る
		m_breakButton->StartBreak();
	}
}

void SceneTitle::Draw()
{
}


