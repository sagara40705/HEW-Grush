#include "GameState.h"
#include "TitleManager.h"
#include "SceneManager.h"
#include "SceneGame.h"
#include "InputKeyboard.h"
#include "InputGamepad.h"
#include "GameObject.h"
#include "SpriteAnimation.h"

TitleManager::TitleManager(GameObject* player) : player_(player)
{
}

void TitleManager::Start()
{
	playerAnimation_ = player_->GetComponent<SpriteAnimation>();

	//Spriteアニメーションを開始
	if (playerAnimation_)
	{
		playerAnimation_->Play();
	}
	
}

void TitleManager::Update()
{
	//入力があれば次のsceneへ
	if (InputNext())
	{
		playerAnimation_->SetStartFrame(5);
		playerAnimation_->SetFramCount(20);

		inputNext = true;
	}



	if (inputNext) AnimationTimer();

	if (canLoadNext) GoNext();
}

void TitleManager::OnDestroy()
{

}

//ボタン入力を検出
bool TitleManager::InputNext()
{
	if (inputNext) return false;

	return (InputKeyboard::GetKeyDown(VK_SPACE) || InputGamepad::GetButtonDown(XINPUT_GAMEPAD_X));
}

void TitleManager::AnimationTimer()
{
	constexpr float AnimationTime = 2.0f;
	constexpr float DT = 1.f / 60.f;

	timer += DT;
	if (timer >= AnimationTime)
	{
		canLoadNext = true;
	}
}

void TitleManager::GoNext()
{
	SceneManager::ChangeSceneAsync<SceneGame>();
}
