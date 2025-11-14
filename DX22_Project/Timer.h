#pragma once
#include <chrono>
#include <string>

class Timer
{
public:
	static void Init();
	static void Uninit();
	static void Update();
	static float GetDeltaTime();
};