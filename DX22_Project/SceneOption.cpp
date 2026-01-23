#include "SceneOption.h"
#include "GameObject.h"
#include "AllComponent.h"
#include "SceneManager.h"
#include "DebugLog.h"
#include "InputKeyboard.h"

void SceneOption::Load()
{
	TexturesLoad();
	CheckCanvas();
	CreateBG();
}

void SceneOption::Unload()
{

}

void SceneOption::TexturesLoad()
{
	TextureManager::Load(TextureTag::UIFrostNoise);
}

void SceneOption::CheckCanvas()
{
	if (Canvas::GetMain())
	{
		DebugLog::Output("Canvas already exists.");
		return;
	}
	else
	{
		World& world = GetWorld();

		GameObject& canvasObj = world.CreateGameObject();
		canvasObj.AddComponent<Canvas>();
		DebugLog::Output("Canvas created.");
		return;
	}
}

void SceneOption::CreateBG()
{
	World& world = GetWorld();

	GameObject& bgObj = world.CreateGameObject();
	bgObj.AddComponent<UiSpriteRenderer>(TextureTag::UIFrostNoise, Vector2::zero(), Vector2(128.f, 72.f));
}

void SceneOption::Update()
{
	if (InputKeyboard::GetKeyDown('O'))
	{
		SceneManager::PopScene();
	}
}

void SceneOption::Draw()
{
}