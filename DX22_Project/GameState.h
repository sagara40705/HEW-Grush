#pragma once
#include <array>
#include <cstdint>

enum class StageId : std::uint8_t
{
    Stage1,
    Stage2,
    Stage3,
    // ...
    Count
};
constexpr size_t ToIndex(StageId stage) noexcept {
    return static_cast<size_t>(stage);
}


struct GameState
{
    bool hasPlayedIntroMovie = false;
    StageId stage_ = StageId::Stage1;

    void Reset()
    {
        hasPlayedIntroMovie = false;
    }
};

bool HasPlayedIntroMovie();
void PlayedIntroMovie();