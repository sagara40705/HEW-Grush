#include "ResultArubauto.h"
#include "GameObject.h"
#include "UiSpriteRenderer.h"
#include "TextureManager.h"
//#include "ResultGaugeUI.h"
#include "Allpoint.h"
//画像のUV設定
constexpr Vector2 kNumberUVScale = { 1.f, 1.0f };

void ResultArubauto::Start()
{
	
	constexpr Vector2 kMinutePos[ToIndex(Arubaito::Count)] = { {680.f, 470.f}, {680.f, 470.f} };  //画像の位置
	constexpr Vector2 kSecondPos[ToIndex(Arubaito::Count)] = { {0.f, 0.f}, {0.f, 0.f} };          //画像の位置二つ目、一つ目と変えたいときに使う
	constexpr Vector2 kNumberSize = { 580.f,80.f };                                               //画像の大きさ
	//-----初期化処理------
	GameObject& tenMinute = gameObject->CreateChild();
	GameObject& oneMinute = gameObject->CreateChild();
	GameObject& sunMinute = gameObject->CreateChild();

	//-------アルバイトレベルによって画像を変える処理------
	int pointGauge = arubaitoLevel();
	switch (pointGauge)
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
