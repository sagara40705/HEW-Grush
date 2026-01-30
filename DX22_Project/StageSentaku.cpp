#include "StageSentaku.h"
#include "GameState.h"
#include "GameObject.h"
#include "UiSpriteRenderer.h"

void StageSentaku::Awake()
{
	constexpr Vector2 kMinutePos[ToIndex(StageSlect::yon)] = { {250.f, 350.f}, {550.f, 370.f},{1175,5 } };
	constexpr Vector2 kColonPos = { 1289.f,700.f };
	constexpr Vector2 kSecondPos[ToIndex(StageSlect::san)] = { {1175.f,250.f}, {1175.f, 550.f} };
	constexpr Vector2 kNumberUVScale[ToIndex(StageSlect::go)] = { { 0.25f, 0.5f },{ 1.f, 1.f },{ 0.75f, 0.5f },{ 1.f, 0.5f } };

	constexpr Vector2 kNumberSize = { 100.f,100.f };
	constexpr Vector2 kNumber = { 100.f,60.f };

	GameObject& tenMinute = gameObject->CreateChild();
	Stage1_[ToIndex(StageSlect::iti)] = &tenMinute.AddComponent<UiSpriteRenderer>(
		TextureTag::Number, kMinutePos[ToIndex(StageSlect::iti)], kNumberSize, Vector2::zero(), kNumberUVScale[ToIndex(StageSlect::iti)]);

	GameObject& Stageleve2 = gameObject->CreateChild();
	Stage2_[ToIndex(StageSlect::ni)] = &Stageleve2.AddComponent<UiSpriteRenderer>(
		TextureTag::Number2, kMinutePos[ToIndex(StageSlect::ni)], kNumber, Vector2::zero(), kNumberUVScale[ToIndex(StageSlect::ni)]);

	GameObject& Stageleve3 = gameObject->CreateChild();
	Stage3_[ToIndex(StageSlect::san)] = &Stageleve3.AddComponent<UiSpriteRenderer>(
		TextureTag::Number3, kMinutePos[ToIndex(StageSlect::san)], kNumber, Vector2::zero(), kNumberUVScale[ToIndex(StageSlect::ni)]);

	GameObject& Stageleve4 = gameObject->CreateChild();
	Stage4_[ToIndex(StageSlect::san)] = &Stageleve4.AddComponent<UiSpriteRenderer>(
		TextureTag::Number3, kSecondPos[ToIndex(StageSlect::iti)], kNumber, Vector2::zero(), kNumberUVScale[ToIndex(StageSlect::ni)]);

	GameObject& Stageleve5 = gameObject->CreateChild();
	Stage5_[ToIndex(StageSlect::san)] = &Stageleve5.AddComponent<UiSpriteRenderer>(
		TextureTag::Number3, kSecondPos[ToIndex(StageSlect::ni)], kNumber, Vector2::zero(), kNumberUVScale[ToIndex(StageSlect::ni)]);

	

}

void StageSentaku::Update()
{
}
