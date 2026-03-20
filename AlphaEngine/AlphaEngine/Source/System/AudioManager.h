#pragma once

#include "AEEngine.h"
#include "SettingsManager.h"
#include "SingletonTemplate.h"
#include <string>

struct AudioClip
{
	std::string name;
	std::string filepath;
	bool isMusic;
	AEAudio audio;
};

/*!
Add new audio here
Refer to struct if unsure
*/
static AudioClip AudioClipList[] =
{
	{ "bgm_level1", "Assets/Audio/Night Ambient 5.mp3", true},
	{ "player_attack", "Assets/Audio/ANIMAL-LION_GEN-HDF-01134.wav", false},
};

class AudioManager : public SingletonTemplate<AudioManager>
{
	friend class SingletonTemplate<AudioManager>;

private:
	f32 localBGMVolume;
	f32 localSFXVolume;

	AEAudioGroup BGM_;
	AEAudioGroup SFX_;
	int ClipCount;

	SettingsManager* settings = nullptr;

	void LoadAll();
	const AudioClip* GetAudioClip(const std::string& name) const;

public:
	AudioManager();
	~AudioManager();

	/*!
	@brief	Plays audio clip one shot
	*/
	void PlaySFX(const std::string& name, float volume = 1.0f, float pitch = 1.0f);
	void PlayMusic(const std::string& name, float volume = 1.0f, float pitch = 1.0f);
	void PauseMusic();
	void ResumeMusic();
	void StopMusic();

	void  SetBGMVolume(float volume);
	void  SetSFXVolume(float volume);
	float GetBGMVolume() const;
	float GetSFXVolume() const;
};
