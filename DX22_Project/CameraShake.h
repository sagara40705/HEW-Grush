#pragma once
#include "Component.h"
#include "Vector3.h"
#include <cstdint>
#include <cmath>
#include <cstdlib>

enum class ShakeType : std::uint8_t
{
    Level_1 = 0,
    Level_2 = 1,
    Level_3 = 2,
    Count = 3,
};
constexpr size_t ToIndex(ShakeType type) noexcept {
    return static_cast<size_t>(type);
}


class CameraShake : public MonoBehaviour, public ILateUpdate
{
private:
    bool play = false;
public:
    void Update() override;
    void LateUpdate() override;
};