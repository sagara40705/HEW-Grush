#pragma once
#include <cstdint>
#include "Component.h"
#include "ResultGaugeUI.h"

enum class RankType : std::uint8_t
{
	Rank_S,
	Rank_A,
	Rank_B,
	Rank_C,
	Rank_D,
	Count
};

constexpr static size_t ToIndex(RankType d) noexcept { return static_cast<size_t>(d); }

class ResultRankUI : public MonoBehaviour
{
public:

	ResultRankUI() = default;
	~ResultRankUI() = default;

	void Start();
	void Update(float);
	void Draw();
private:
	UiSpriteRenderer* uiMinute_[ToIndex(RankType::Count)]{};
	UiSpriteRenderer* uiColon_ = nullptr;
	UiSpriteRenderer* uiSecond_[ToIndex(RankType::Count)]{};
};
