#pragma once
#include <array>
#include <cstdint>

enum class StageId : std::uint8_t
{
    Stage1 = 0,
    Stage2 = 1,
    Stage3a = 2,
    Stage3b = 3,
    Stage3c = 4,
    Count = 5,
	None = 255
};
constexpr size_t ToIndex(StageId stage) noexcept {
    return static_cast<size_t>(stage);
}

struct GameState
{
    bool hasPlayedIntroMovie = false;
    StageId stage_ = StageId::Stage1;

	// 直近クリアしたステージのタイム（秒）
	float recentClearTimes_ = 0.0f;

    void Reset()
    {
        hasPlayedIntroMovie = false;
    }
};

bool HasPlayedIntroMovie();
void PlayedIntroMovie();

// 現在のステージを取得
StageId GetCurrentStage();

// 直近クリアしたステージのタイムを取得・設定
float GetRecentClearTimes();
void SetRecentClearTimes(float time);