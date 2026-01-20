#include "ResultArubauto.h"
#include "GameObject.h"
#include "UiSpriteRenderer.h"
#include "TextureManager.h"
#include "ResultGaugeUI.h"
constexpr Vector2 kNumberUVScale = { 1.f, 1.0f };

void ResultArubauto::Start()
{
	constexpr Vector2 kMinutePos[ToIndex(Arubaito::Count)] = { {680.f, 470.f}, {680.f, 470.f} };
	constexpr Vector2 kColonPos = { 700.f,430.f };
	constexpr Vector2 kSecondPos[ToIndex(Arubaito::Count)] = { {0.f, 0.f}, {0.f, 0.f} };

	constexpr Vector2 kNumberSize = {580.f,80.f };
	constexpr Vector2 kColonSize = { 40.f, 40.f };
	GameObject& tenMinute = gameObject->CreateChild();
	GameObject& oneMinute = gameObject->CreateChild();
	GameObject& sunMinute = gameObject->CreateChild();
//	int pointGauge = ResultGaugeUI::GetArubaitoLevel();
	switch (2)
	{
	case 1:

		uiMinute_[ToIndex(Arubaito::Ten)] = &tenMinute.AddComponent<UiSpriteRenderer>(TextureTag::pika_Arubaita, kMinutePos[ToIndex(Arubaito::Ten)], kNumberSize, Vector2::zero(), kNumberUVScale);
		break;
	case 2:

		uiMinute_[ToIndex(Arubaito::Ten)] = &oneMinute.AddComponent<UiSpriteRenderer>(TextureTag::sugo_Arubaita, kMinutePos[ToIndex(Arubaito::Ten)], kNumberSize, Vector2::zero(), kNumberUVScale);
		break;
	case 3:

		uiMinute_[ToIndex(Arubaito::Ten)] = &sunMinute.AddComponent<UiSpriteRenderer>(TextureTag::tatu_Arubaita, kMinutePos[ToIndex(Arubaito::Ten)], kNumberSize, Vector2::zero(), kNumberUVScale);
		break;
	};
	
}

void ResultArubauto::Update()
{
	
}

void ResultArubauto::OnDestroy()
{
}
