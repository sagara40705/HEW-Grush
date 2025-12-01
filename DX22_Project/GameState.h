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

struct GameState
{
    bool hasPlayedIntroMovie = false;

    void Reset()
    {
        hasPlayedIntroMovie = false;
    }
};

bool HasPlayedIntroMovie();
void PlayedIntroMovie();