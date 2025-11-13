#pragma once
#include <chrono>
#include <string>

class Timer
{
public:
	static void Update();
	static float DeltaTime();
};