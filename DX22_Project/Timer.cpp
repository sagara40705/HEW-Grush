#include "Timer.h"

//static float deltaTime = 0.f;

float Timer::deltaTime = 0.0f;
std::chrono::high_resolution_clock::time_point Timer::lastTime;	

void Timer::Init()
{
	lastTime = std::chrono::high_resolution_clock::now();
	deltaTime = 0.0f;
}

void Timer::Uninit()
{

}

void Timer::Update()
{
	// Œ»İŠÔ‚ğæ“¾
	auto currentTime = std::chrono::high_resolution_clock::now();

	// Œo‰ßŠÔ‚ğŒvZ
	std::chrono::duration<float> elapsed = currentTime - lastTime;

	// DeltaTime‚ğŠi”[
	deltaTime = elapsed.count();

	// Œ»İŠÔ‚ğ[‘O‰ñ‚ÌŠÔ]‚Æ‚µ‚Ä•Û‘¶
	lastTime = currentTime;
}

float Timer::GetDeltaTime()
{
	return deltaTime;
}
