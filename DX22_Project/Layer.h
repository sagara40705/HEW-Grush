#pragma once
#pragma once
#include <cstdint>
#include <initializer_list>

enum class Layer : uint8_t
{
    Default = 0,
    Player,
    Ground,
    Glass,
	Shard,
    UI,
};

using LayerMask = uint32_t;

constexpr LayerMask ToBit(Layer l) noexcept
{
    return 1u << static_cast<uint8_t>(l);
}

constexpr LayerMask MaskNone = 0u;
constexpr LayerMask MaskAll = 0xFFFFFFFFu;

inline constexpr LayerMask MaskOf(std::initializer_list<Layer> ls) noexcept
{
    LayerMask m = 0;
    for (auto l : ls) m |= ToBit(l);
    return m;
}