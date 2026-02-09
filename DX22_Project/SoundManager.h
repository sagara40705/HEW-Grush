#pragma once
#include "Sound.h"

enum SoundTag {
	SceneGameBGM1,
	SceneGameBGM2,
	NomalAttackSE,
	DashAttackSE,
	SpinAttackSE,
	Level1GlassBreakSE,
	Level2GlassBreakSE,
	Level3GlassBreakSE,
	GradeUpSE,
	GradeDownSE,
	GaugeDownSE,
	WalkSE,
	TitleBGM,
	TitleStartSE,
	StageSelectBGM,
	SelectSE,
	ResultSE,
	OptionOpenSE,
	OptionMoveSE,
	OptionSelectSE,
	OptionCloseSE,
	GlassCrackSE,
	GameFinishSE,
	ResultGaugeUpSE,
	FeverGaugeSE,
	GameStartSE,
	Count		//êî
};

class SoundManager {
private:
	static XAUDIO2_BUFFER* s_sounds[SoundTag::Count];
	static IXAudio2SourceVoice* speakers[SoundTag::Count];

public:
	static void Load();
	static void Unload();
	static XAUDIO2_BUFFER* GetSound(SoundTag tag);
	static void SoundPlay(SoundTag tag, float volume);
	static void StopSound(SoundTag tag);
};