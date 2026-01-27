#include "SoundManager.h"

XAUDIO2_BUFFER* SoundManager::s_sounds[SoundTag::Count];
IXAudio2SourceVoice* SoundManager::speakers[SoundTag::Count];

void SoundManager::Load()
{
	InitSound();
	s_sounds[SoundTag::SceneGameBGM]  = LoadSound("Assets/Sound/maou_bgm_cyber41.mp3", true);
	s_sounds[SoundTag::NomalAttackSE] = LoadSound("Assets/Sound/attack_v2.wav", false);
	s_sounds[SoundTag::DashAttackSE]  = LoadSound("Assets/Sound/attack_dash.wav", false);
	s_sounds[SoundTag::SpinAttackSE]  = LoadSound("Assets/Sound/SpinAttack.mp3", false);
	s_sounds[SoundTag::Level1GlassBreakSE]  = LoadSound("Assets/Sound/glassbreak1.mp3", false);
	s_sounds[SoundTag::Level2GlassBreakSE] = LoadSound("Assets/Sound/glassbreak6.wav", false);
	s_sounds[SoundTag::Level3GlassBreakSE] = LoadSound("Assets/Sound/glassbreak5.mp3", false);
	s_sounds[SoundTag::GradeUpSE]     = LoadSound("Assets/Sound/gauge_up.wav", false);
	s_sounds[SoundTag::GradeDownSE]   = LoadSound("Assets/Sound/gauge_down_v2.wav", false);
	s_sounds[SoundTag::GaugeDownSE]   = LoadSound("Assets/Sound/GaugeDown.mp3", true);
	s_sounds[SoundTag::WalkSE]        = LoadSound("Assets/Sound/walk.mp3", true);
	s_sounds[SoundTag::TitleBGM]      = LoadSound("Assets/Sound/maou_bgm_cyber15.mp3",true);
	s_sounds[SoundTag::TitleStartSE]  = LoadSound("Assets/Sound/glassbreak4.mp3",false);
	s_sounds[SoundTag::StageSelectBGM] = LoadSound("Assets/Sound/maou_bgm_cyber44.mp3", true);
	s_sounds[SoundTag::SelectSE] = LoadSound("Assets/Sound/start.wav", false);
	s_sounds[SoundTag::ResultSE] = LoadSound("Assets/Sound/maou_se_jingle07.mp3", false);
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

void SoundManager::SoundPlay(SoundTag tag,float volume)
{
	speakers[tag] = PlaySound(GetSound(tag));
	speakers[tag]->SetVolume(volume);
}

void SoundManager::StopSound(SoundTag tag)
{
	speakers[tag]->Stop();
}
