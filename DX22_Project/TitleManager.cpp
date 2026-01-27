#include "GameState.h"
#include "TitleManager.h"
#include "SceneManager.h"
#include "SceneGame.h"
#include "InputKeyboard.h"
#include "InputGamepad.h"
#include "GameObject.h"
#include "SpriteAnimation.h"

#include "UiSpriteRenderer.h"
#include "TextureManager.h"
#include "SceneStory.h"

//FadeOut¶¬
void TitleManager::Awake()
{
}

void TitleManager::Start()
{

}

void TitleManager::Update()
{
	//“ü—Í‚ª‚ ‚ê‚ÎŽŸ‚Ìscene‚Ö
	if (InputNext())
	{
		inputNext = true;
	}

	if (inputNext) AnimationTimer();

	if (canLoadNext) GoNext();
}

void TitleManager::OnDestroy()
{

}

//ƒ{ƒ^ƒ““ü—Í‚ðŒŸo
bool TitleManager::InputNext()
{
	if (inputNext) return false;

	return (InputKeyboard::GetKeyDown(VK_SPACE) || InputGamepad::GetButtonDown(XINPUT_GAMEPAD_X));
}

void TitleManager::AnimationTimer()
{
	constexpr float AnimationTime = 1.0f;
	constexpr float DT = 1.f / 60.f;

	timer += DT;
	if (timer >= AnimationTime)
	{
		canLoadNext = true;
	}
}

void TitleManager::GoNext()
{
	if (isLoadNext) return;

	isLoadNext = true;
	SceneManager::ChangeSceneAsync<SceneStory>();
}
