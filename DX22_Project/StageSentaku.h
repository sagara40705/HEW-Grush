#pragma once
#include "Component.h"
#include "TextureManager.h"
//#include "StagePoint.h"
#include <array>
enum class StageSlect : std::uint8_t
{
	iti,
	ni,
	san,
	yon,
	go,
	Count
};
constexpr size_t ToIndex(StageSlect d) noexcept { return static_cast<size_t>(d); }

class StageSentaku : public MonoBehaviour
{
	void Awake();
	void Update() override;
	UiSpriteRenderer* Stage1_[ToIndex(StageSlect::Count)]{};;
	UiSpriteRenderer* Stage2_[ToIndex(StageSlect::Count)]{};;
	UiSpriteRenderer* Stage3_[ToIndex(StageSlect::Count)]{};;
	UiSpriteRenderer* Stage4_[ToIndex(StageSlect::Count)]{};;
	UiSpriteRenderer* Stage5_[ToIndex(StageSlect::Count)]{};;
	UiSpriteRenderer* bou = nullptr;
	UiSpriteRenderer* bou2 = nullptr;
};

