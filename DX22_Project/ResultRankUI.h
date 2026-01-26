#pragma once
#include <cstdint>
#include "Component.h"
#include "TextureManager.h"

enum class RankType : std::uint8_t
{
	None,
	Rank_S,
	Rank_A,
	Rank_B,
	Rank_C,
	Count
};
constexpr static size_t ToIndex(RankType d) noexcept { return static_cast<size_t>(d); }

class ResultRankUI : public Component
{
public:

	ResultRankUI() = default;
	~ResultRankUI() = default;

	void Awake();
	void Start();
	void OnDestroy() override;
private:
	RankType rank_ = RankType::None;
	UiSpriteRenderer* ui_ = nullptr;
private:
	RankType CalcRank();
	TextureTag LoadRankTexture(RankType rank);
public:
	RankType GetRank() const { return rank_; }
};


