#include "FadeOut.h"
#include "GameObject.h"
#include "UiSpriteRenderer.h"
#include "SpriteAnimation.h"

FadeOut::FadeOut(FadeType type)
{
	FadeJudgment = type;
}

void FadeOut::Awake()
{

	switch (FadeJudgment)
	{
	case FadeOut::FadeType::In:
	{

		// FadeOut用のUIを生成
		GameObject* fadeIn = &gameObject->CreateChild();
		fadeIn->AddComponent<UiSpriteRenderer>(TextureTag::FadeIn,
			Vector2::zero(), Vector2(1280, 720));
		anime_ = &fadeIn->AddComponent
			<SpriteAnimation>(6, 8, 3, 48, false);
		break;
	}
	case FadeOut::FadeType::Out:
	{
		// FadeOut用のUIを生成
		GameObject* fadeOut = &gameObject->CreateChild();
		fadeOut->AddComponent<UiSpriteRenderer>(TextureTag::FadeOut,
			Vector2::zero(), Vector2(1280, 720));
		anime_ = &fadeOut->AddComponent
			<SpriteAnimation>(6, 8, 3, 48, false);
		break;
	}
	}
}

void FadeOut::Start()
{
	// アニメーションを再生開始
	anime_->Play();
}

void FadeOut::OnDestroy()
{
	anime_ = nullptr;
}
