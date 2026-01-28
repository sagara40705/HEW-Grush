#pragma once
#include <cstdint>
#include "Component.h"
#include "TextureManager.h"
#include "Time/Timer.h"


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

class ResultRankUI : public MonoBehaviour
{
public:

	ResultRankUI() = default;
	~ResultRankUI() = default;

	void Awake();
	void Start();
	void Update() override;
	void OnDestroy() override;
private:
	RankType rank_ = RankType::None;
	UiSpriteRenderer* ui_ = nullptr;
	UiSpriteRenderer* uiFlash_ = nullptr;
	
	float glowTimer = 0.0f;
	const float kGlowDuration = 0.5f; // 光る長さ
	bool  isRankFlashOn_ = false;     // ランクフラッシュ中かどうか
	Timer waitTimer_;
	const float WAIT_TIME = 2.0f;

private:
	RankType CalcRank();
	TextureTag LoadRankTexture(RankType rank);
public:
	RankType GetRank() const { return rank_; }
};


