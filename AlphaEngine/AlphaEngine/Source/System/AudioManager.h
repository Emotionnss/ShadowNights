#pragma once

#include "AEEngine.h"
#include "SettingsManager.h"
#include "SingletonTemplate.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

struct AudioClip
{
	std::string name;
	std::string filepath;
	bool isMusic;
	AEAudio audio;
};

/*!
Reads audio clips from audio data
A - name of clip to play
B - audio file name with file type like .wav or .mp3
C - If it's a music BGM, false if it isn't
*/
class AudioManager : public SingletonTemplate<AudioManager>
{
	friend class SingletonTemplate<AudioManager>;

private:
	f32 localBGMVolume;
	f32 localSFXVolume;

	AEAudioGroup BGM_LOOP;
	AEAudioGroup SFX_LOOP;
	AEAudioGroup SFX_SINGLE;
	int ClipCount;

	std::vector<AudioClip> AudioClipList;

	SettingsManager* settings = nullptr;

	void Trim(std::string& str); // Used to parse CSV
	void ParseCSV(const std::string& csvPath);
	void LoadAll();
	const AudioClip* GetAudioClip(const std::string& name) const;

public:
	AudioManager();
	~AudioManager();

	/*!
	@brief	Plays audio clip one shot (No loop)
	*/
	void PlaySFXOneShot(const std::string& name, f32 volume = 1.0f, f32 pitch = 1.0f);

	/*!
	@brief	Plays audio clip with loop
	*/
	void PlayEffect(const std::string& name, f32 volume = 1.0f, f32 pitch = 1.0f);
	void PauseEffect();
	void ResumeEffect();
	void StopEffect();

	/*!
	@brief	Plays audio with loop
	*/
	void PlayMusic(const std::string& name, f32 volume = 1.0f, f32 pitch = 1.0f);
	void PauseMusic();
	void ResumeMusic();
	void StopMusic();

	void  SetBGMVolume(float volume);
	void  SetSFXVolume(float volume);
	float GetBGMVolume() const;
	float GetSFXVolume() const;
};
