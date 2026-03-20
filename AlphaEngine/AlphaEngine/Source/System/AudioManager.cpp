#include "AudioManager.h"

AudioManager::AudioManager()
	: localBGMVolume{ 1.0f }
	, localSFXVolume{ 1.0f }
	, BGM_{}
	, SFX_{}
	, ClipCount{ 0 }
{
	// Count the amount of clips that has been defined
	for (const AudioClip& clip : AudioClipList)
	{
		++ClipCount;
	}

	settings = SettingsManager::GetInstance();
	if (settings != nullptr)
	{
		localBGMVolume = settings->GetSettingsBGMVolume() / 100.0f;
		localSFXVolume = settings->GetSettingsSFXVolume() / 100.0f;
	}
	else
	{
		printf("SoundManager: Unable to retrieve volume settings.\n");
	}

	BGM_ = AEAudioCreateGroup();
	SFX_ = AEAudioCreateGroup();

	AEAudioSetGroupVolume(BGM_, localBGMVolume);
	AEAudioSetGroupVolume(SFX_, localSFXVolume);

	LoadAll();
}

AudioManager::~AudioManager()
{
	for (int i = 0; i < ClipCount; ++i)
	{
		AEAudioUnloadAudio(AudioClipList[i].audio);
	}

	AEAudioUnloadAudioGroup(BGM_);
	AEAudioUnloadAudioGroup(SFX_);
}

void AudioManager::LoadAll()
{
	// I didn't really do any checks for the list :P
	for (int i = 0; i < ClipCount; ++i)
	{
		if (AudioClipList[i].isMusic)
		{
			AudioClipList[i].audio = AEAudioLoadMusic(AudioClipList[i].filepath.c_str());
		}
		else
		{
			AudioClipList[i].audio = AEAudioLoadSound(AudioClipList[i].filepath.c_str());
		}
	}
}

const AudioClip* AudioManager::GetAudioClip(const std::string& name) const
{
	// Strict audio check if audio is in list and has valid audio
	for (int i = 0; i < ClipCount; ++i)
	{
		if (AudioClipList[i].name == name)
		{
			if (AEAudioIsValidAudio(AudioClipList[i].audio))
			{
				return &AudioClipList[i];
			}
			else
			{
				printf("AudioClip is found but has invalid audio.\n");
				return nullptr;
			}
		}
	}
	printf("Unable to find AudioClip from list.\n");
	return nullptr;
}

void AudioManager::PlaySFX(const std::string& name, float volume, float pitch)
{
	const AudioClip* index = GetAudioClip(name);
	if (index != nullptr)
	{
		AEAudioPlay(index->audio, SFX_, volume, pitch, 0);
	}
	else
	{
		printf("Unable to play SFX.\n");
	}
}

void AudioManager::PlayMusic(const std::string& name, float volume, float pitch)
{
	const AudioClip* index = GetAudioClip(name);
	if (index != nullptr)
	{
		AEAudioStopGroup(BGM_);
		AEAudioPlay(index->audio, SFX_, volume, pitch, 0);
	}
	else
	{
		printf("Unable to play music.\n");
	}
}

void AudioManager::PauseMusic() { AEAudioPauseGroup(BGM_); }
void AudioManager::ResumeMusic() { AEAudioResumeGroup(BGM_); }
void AudioManager::StopMusic() { AEAudioStopGroup(BGM_); }

void AudioManager::SetBGMVolume(float volume)
{
	localBGMVolume = volume;
	AEAudioSetGroupVolume(BGM_, localBGMVolume);
}

void AudioManager::SetSFXVolume(float volume)
{
	localSFXVolume = volume;
	AEAudioSetGroupVolume(SFX_, localSFXVolume);
}

float AudioManager::GetBGMVolume() const { return localBGMVolume; }
float AudioManager::GetSFXVolume() const { return localSFXVolume; }
