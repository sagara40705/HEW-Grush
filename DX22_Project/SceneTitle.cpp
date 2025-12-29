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
	//TextureManager::Load(TextureTag::UI_font);
	//TextureManager::Load(TextureTag::UI_Frame);
	//TextureManager::Load(TextureTag::UI_Break);
	TextureManager::Load(TextureTag::GaugeEffect);
	TextureManager::Load(TextureTag::FadeOut);
	TextureManager::Load(TextureTag::Title_Main);
	TextureManager::Load(TextureTag::Title_Font);
	TextureManager::Load(TextureTag::Title_Break);
	TextureManager::Load(TextureTag::Title_BreakID);
	InputKeyboard* Input = new InputKeyboard();

	World& w = GetWorld();

	////Canvas
	GameObject& canvas = w.CreateGameObject();
	canvas.AddComponent<Canvas>();

	////背景生成
	//GameObject& backGroung = w.CreateGameObject();
	//backGroung.AddComponent<UiSpriteRenderer>(TextureTag::Title_Main , Vector2(0.f, 0.f), Vector2(SCREEN_WIDTH, SCREEN_HEIGHT));

	////文字生成
	//GameObject& player = w.CreateGameObject();

	/*GameObject& titleFont = w.CreateGameObject();

	titleFont.AddComponent<UiSpriteRenderer>(
		TextureTag::Title_Font,
		Vector2(450.0f, 530.0f),
		Vector2(350.0f, 80.0f)
	);*/

	GameObject& TitleBreak = w.CreateGameObject();

	auto& breakUi =
		TitleBreak.AddComponent<BreakUiRenderer>(
			TextureTag::Title_BreakID // ★ ID用
		);

	// Update から触れるように保存
	m_breakButton = &breakUi;

	// Update から触れるように保存
	m_breakButton = &breakUi;


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
	// breakuirenderer が存在しない場合は何もしない
	if (!m_breakButton) return;

	//// エンターキーが「押された瞬間
	if (InputKeyboard::GetKeyDown(VK_RETURN))
	{
		m_breakButton->StartBreak();
	}
}

void SceneTitle::Draw()
{
}


