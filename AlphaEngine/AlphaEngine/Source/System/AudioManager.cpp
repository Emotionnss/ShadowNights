#include "AudioManager.h"

AudioManager::AudioManager()
	: localBGMVolume{ 1.0f }
	, localSFXVolume{ 1.0f }
	, BGM_LOOP{}
	, SFX_LOOP{}
	, SFX_SINGLE{}
	, ClipCount{ 0 }
	, BGMplaying{ false }
	, SFXplaying{ false }
{
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

	BGM_LOOP = AEAudioCreateGroup();
	SFX_LOOP = AEAudioCreateGroup();
	SFX_SINGLE = AEAudioCreateGroup();

	AEAudioSetGroupVolume(BGM_LOOP, localBGMVolume);
	AEAudioSetGroupVolume(SFX_LOOP, localSFXVolume);
	AEAudioSetGroupVolume(SFX_SINGLE, localSFXVolume);

	ParseCSV("Assets/Data/AudioData.csv");
	LoadAll();
}

AudioManager::~AudioManager()
{
	for (int i = 0; i < ClipCount; ++i)
	{
		AEAudioUnloadAudio(AudioClipList[i].audio);
	}

	AEAudioUnloadAudioGroup(BGM_LOOP);
	AEAudioUnloadAudioGroup(SFX_LOOP);
	AEAudioUnloadAudioGroup(SFX_SINGLE);
}

void AudioManager::Trim(std::string& str)
{
	const std::string whitespace = " \t\r\n";
	size_t start = str.find_first_not_of(whitespace);
	size_t end = str.find_last_not_of(whitespace);
	if (start == std::string::npos)
	{
		str = "";
	}
	else
	{
		str = str.substr(start, end - start + 1);
	}
}

void AudioManager::ParseCSV(const std::string& csvPath)
{
	std::ifstream file(csvPath);
	if (!file.is_open())
	{
		printf("AudioManager: Failed to open CSV '%s'.\n", csvPath.c_str());
		return;
	}

	std::string line;
	while (std::getline(file, line))
	{
		if (line.empty())
		{
			continue;
		}

		std::stringstream ss(line);
		std::string name, filename, isMusicStr;

		// Column A — clip name
		if (!std::getline(ss, name, ',')) continue;
		// Column B — audio filename
		if (!std::getline(ss, filename, ',')) continue;
		// Column C — is music (true/false)
		if (!std::getline(ss, isMusicStr, ',')) continue;

		Trim(name);
		Trim(filename);
		Trim(isMusicStr);

		if (name.empty() || filename.empty() || isMusicStr.empty())
		{
			printf("AudioManager: Skipping malformed row '%s'.\n", line.c_str());
			continue;
		}

		AudioClip clip{};
		clip.name = name;
		clip.filepath = "Assets/Audio/" + filename;
		clip.isMusic = (isMusicStr == "TRUE");
		clip.audio = {};

		AudioClipList.push_back(clip);
		++ClipCount;
	}

	file.close();
}

void AudioManager::LoadAll()
{
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
				printf("AudioClip '%s' is found but has invalid audio.\n", name.c_str());
				return nullptr;
			}
		}
	}

	printf("Unable to find AudioClip '%s' from list.\n", name.c_str());
	return nullptr;
}

void AudioManager::PlaySFXOneShot(const std::string& name, f32 volume, f32 pitch)
{
	const AudioClip* index = GetAudioClip(name);
	if (index != nullptr)
	{
		AEAudioPlay(index->audio, SFX_SINGLE, volume, pitch, 0);
	}
}

void AudioManager::PlayEffect(const std::string& name, f32 volume, f32 pitch)
{
	const AudioClip* index = GetAudioClip(name);
	if (index != nullptr)
	{
		AEAudioStopGroup(SFX_LOOP);
		AEAudioPlay(index->audio, SFX_LOOP, volume, pitch, -1);
		SFXplaying = true;
	}
}

void AudioManager::PauseEffect()
{
	AEAudioPauseGroup(SFX_LOOP);
}

void AudioManager::ResumeEffect()
{
	AEAudioResumeGroup(SFX_LOOP);
}

void AudioManager::StopEffect()
{
	AEAudioStopGroup(SFX_LOOP);
	SFXplaying = false;
}

bool AudioManager::GetEffectGroup() const
{
	return SFXplaying;
}

void AudioManager::PlayMusic(const std::string& name, f32 volume, f32 pitch)
{
	const AudioClip* index = GetAudioClip(name);
	if (index != nullptr)
	{
		AEAudioStopGroup(BGM_LOOP);
		AEAudioPlay(index->audio, BGM_LOOP, volume, pitch, -1);
		BGMplaying = true;
	}
}

void AudioManager::PauseMusic()
{
	AEAudioPauseGroup(BGM_LOOP);
}
void AudioManager::ResumeMusic()
{
	AEAudioResumeGroup(BGM_LOOP);
}
void AudioManager::StopMusic()
{
	AEAudioStopGroup(BGM_LOOP);
	BGMplaying = false;
}

bool AudioManager::GetMusicGroup() const
{
	return BGMplaying;
}

void AudioManager::SetBGMVolume(float volume)
{
	localBGMVolume = volume;
	AEAudioSetGroupVolume(BGM_LOOP, localBGMVolume);
}

void AudioManager::SetSFXVolume(float volume)
{
	localSFXVolume = volume;
	AEAudioSetGroupVolume(SFX_LOOP, localSFXVolume);
	AEAudioSetGroupVolume(SFX_SINGLE, localSFXVolume);
}

float AudioManager::GetBGMVolume() const { return localBGMVolume; }
float AudioManager::GetSFXVolume() const { return localSFXVolume; }
