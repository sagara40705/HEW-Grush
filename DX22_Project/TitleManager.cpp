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

//FadeOut生成
void TitleManager::Awake()
{
	//エンプティの子オブジェクト化
	GameObject& fadeIn = gameObject->CreateChild();
	fadeIn.AddComponent<UiSpriteRenderer>
		(TextureTag::FadeIn, Vector2(0,0), Vector2(1280, 720));
	fade_ = &fadeIn.AddComponent
		<SpriteAnimation>(6, 8, 3, 48, false);
}
 
void TitleManager::Start()
{
	fade_->Stop();
	//fade_->SetFrame(17);
}

void TitleManager::Update()
{
	//入力があれば次のsceneへ
	if (InputNext())
	{
		inputNext = true;
		fade_->Play();
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
