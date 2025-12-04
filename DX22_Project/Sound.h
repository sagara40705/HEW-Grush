#pragma once
#include"conioex.h"
#define CONIOEX
#include <memory>
#include <list>

enum class SoundId : int
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


class Sound
{
public:
	static void Init();
	static void Uninit();

	static void Play(SoundId id, int loop);
	static void Closes(SoundId id);
private:
	static std::list<std::unique_ptr<INT_PTR>> s_sounds;
	
};

