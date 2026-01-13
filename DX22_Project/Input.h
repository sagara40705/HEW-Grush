#pragma once
#include <cstdint>


class Input
{
public:
	enum class Type : uint8_t
	{
		Keybord,
		Mouse,
		Gamepad,
		Count
	};
};