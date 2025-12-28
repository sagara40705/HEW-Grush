#include "SoundManager.h"

XAUDIO2_BUFFER* SoundManager::s_sounds[SoundTag::Count];

void SoundManager::Load()
{
	InitSound();
	s_sounds[SoundTag::SceneGameBGM]  = LoadSound("Assets/Sound/maou_bgm_cyber41.mp3", true);
	s_sounds[SoundTag::NomalAttackSE] = LoadSound("Assets/Sound/attack_v2.wav", false);
	s_sounds[SoundTag::DashAttackSE]  = LoadSound("Assets/Sound/attack_dash.wav", false);
	s_sounds[SoundTag::SpinAttackSE]  = LoadSound("Assets/Sound/SpinAttack.mp3", false);
	s_sounds[SoundTag::GlassBreakSE]  = LoadSound("Assets/Sound/glassbreak.mp3", false);
	s_sounds[SoundTag::GradeUpSE]     = LoadSound("Assets/Sound/gauge_up.wav", false);
	s_sounds[SoundTag::GradeDownSE]   = LoadSound("Assets/Sound/gauge_down_v2.wav", false);
	s_sounds[SoundTag::GaugeDownSE]   = LoadSound("Assets/Sound/GaugeDown.mp3", false);
	s_sounds[SoundTag::WalkSE]        = LoadSound("Assets/Sound/walk.mp3", true);
}

void SoundManager::Unload()
{
	for (int i = 0; i <= SoundTag::Count; i++)
	{
		if (s_sounds[i])
		{
			s_sounds[i] = nullptr;
		}
	}
	UninitSound();
}

XAUDIO2_BUFFER* SoundManager::GetSound(SoundTag tag)
{
	return s_sounds[tag];
}
