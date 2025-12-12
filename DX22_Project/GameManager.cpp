#include "GameState.h"
#include "TitleManager.h"
#include "SceneManager.h"
#include "SceneResult.h"
#include "InputKeyboard.h"
#include "InputGamepad.h"
#include "GameObject.h"
#include "SpriteAnimation.h"
#include "GameManager.h"


GameManager::GameManager(GameObject* player)
{
}

void GameManager::Start()
{
	playerAnimation_ = player_->GetComponent<SpriteAnimation>();

	//Spriteアニメーションを開始
	if (playerAnimation_)
	{
		playerAnimation_->Play();
	}
}

void GameManager::Update()
{
	//入力があれば次のsceneへ
	if (InputNext())
	{
		playerAnimation_->SetStartFrame(5);
		playerAnimation_->SetFramCount(20);

		inputNext = true;
	}



	//if (inputNext) AnimationTimer();

	if (canLoadNext) GoNext();
}

void GameManager::OnDestroy()
{
}

bool GameManager::InputNext()
{
	if (inputNext) return false;

	return (InputKeyboard::GetKeyDown(VK_UP) || InputGamepad::GetButtonDown(XINPUT_GAMEPAD_X));
}

void GameManager::AnimationTimer()
{
	constexpr float AnimationTime = 2.0f;
	constexpr float DT = 1.f / 60.f;

	timer += DT;
	if (timer >= AnimationTime)
	{
		canLoadNext = true;
	}
}

void GameManager::GoNext()
{
	
}
