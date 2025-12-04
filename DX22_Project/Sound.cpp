#include "Sound.h"

std::array<INT_PTR, ToIndex(SoundId::Count)> Sound::s_sounds;

void Sound::Init()
{
	//“Á‚É‚È‚µ
}

void Sound::Clear()
{
	//	‘S‰ð•ú
	for (size_t id= 0; id < ToIndex(SoundId::Count); ++id)
	{
		closesound(s_sounds[id]);
	}
}


void Sound::Load(SoundId id, const char* file)
{
	s_sounds[ToIndex(id)] = opensound(file);
}

void Sound::UnLoad(SoundId id)
{
	closesound(s_sounds[ToIndex(id)]);
}

void Sound::Play(SoundId id, int loop)
{
	playsound(s_sounds[ToIndex(id)], loop);
}

void Sound::Stop(SoundId id)
{
}
