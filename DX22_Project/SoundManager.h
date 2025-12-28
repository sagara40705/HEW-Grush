#pragma once
#include "Sound.h"

enum SoundTag {
	SceneGameBGM,
	NomalAttackSE,
	DashAttackSE,
	SpinAttackSE,
	GlassBreakSE,
	GradeUpSE,
	GradeDownSE,
	GaugeDownSE,
	WalkSE,
	Count		//êî
};

class SoundManager {
private:
	static XAUDIO2_BUFFER* s_sounds[SoundTag::Count];

public:
	static void Load();
	static void Unload();
	static XAUDIO2_BUFFER* GetSound(SoundTag tag);
};