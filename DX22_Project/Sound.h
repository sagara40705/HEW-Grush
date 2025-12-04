#pragma once
#define CONIOEX
#include"conioex.h"
#include <cstdint> 
#include <memory>
#include <array>

enum class SoundId : std::uint8_t
{
	GameBgm,
	PlayerWalkSE,
	WeaponSwingSE,
	DashAttackSE,
	SpinAttackSE,
	GradeUpSE,
	GradeDownSE,
	GlassbreakSE,
	GaugeDownSE,
	Count
};
constexpr size_t ToIndex(SoundId id) noexcept{
	return static_cast<size_t>(id);
}

class Sound
{
public:
	static void Init();
	static void Clear();

	static void Load(SoundId id, const char* file);
	static void UnLoad(SoundId id);
	
	static void Play(SoundId id, int loop);
	static void Stop(SoundId id);
private:
	static std::array<INT_PTR, ToIndex(SoundId::Count)> s_sounds;
	
};

