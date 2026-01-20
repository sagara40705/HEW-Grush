#include "ResultManeger.h"
//#include "GameState.h"
//#include "TitleManager.h"
#include "SceneManager.h"
//#include "SceneGame.h"
#include "InputKeyboard.h"
#include "InputGamepad.h"
//#include "GameObject.h"
#include "SceneTitle.h"

ResultManeger::ResultManeger()
{
}

void ResultManeger::Update()
{
	if (InputNext())
	{
		inputNext = true;
	}

	
   if (inputNext) TitleTimer();

	if (canLoadNext) GoNext();
	

}

bool ResultManeger::InputNext()
{
	if (inputNext) return false;

	return (InputKeyboard::GetKeyDown(VK_SPACE) || InputGamepad::GetButtonDown(XINPUT_GAMEPAD_X));
}

void ResultManeger::TitleTimer()
{
	int Timer = 3;
	if (Timer >= 3)
	{
		canLoadNext = true;
	}
}

void ResultManeger::GoNext()
{
	
	SceneManager::ChangeSceneAsync<SceneTitle>();
}
