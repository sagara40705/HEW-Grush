#pragma once
#include <cstdint>
#include "Component.h"
#include "TextureManager.h"
#include "GameState.h"
#include "Time/Timer.h"
#include "ResultRankUI.h"

//enum class RankType : std::uint8_t
//{
//	None,
//	Rank_S,
//	Rank_A,
//	Rank_B,
//	Rank_C,
//	Count
//};
//constexpr static size_t ToIndex(RankType d) noexcept { return static_cast<size_t>(d); }

class StageClearRank : public ResultRankUI
{
public:
	StageClearRank() = default;
	~StageClearRank() = default;

	void Awake();
	void Start();
	void Update() override;
	void OnDestroy() override;
	StageId GetStageSelect();
private:
	RankType  rank_ = RankType::None;
	UiSpriteRenderer* Stage1Rank_ = nullptr;
	UiSpriteRenderer* Stage2Rank_ = nullptr;
	UiSpriteRenderer* Stage3aRank_ = nullptr;
	UiSpriteRenderer* Stage3bRank_ = nullptr;
	UiSpriteRenderer* Stage3cRank_ = nullptr;
	TextureTag LoadTexture(RankType rank_);
	StageId StageSelect_ = StageId::None;
	RankType ClearRank();

	bool isStageRank_ = false;
};

