#include "AudioManager.h"

AudioManager::AudioManager()
	: localBGMVolume{ 1.0f }
	, localSFXVolume{ 1.0f }
	, BGM_{}
	, SFX_{}
	, ClipCount{ 0 }
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

	BGM_ = AEAudioCreateGroup();
	SFX_ = AEAudioCreateGroup();

	AEAudioSetGroupVolume(BGM_, localBGMVolume);
	AEAudioSetGroupVolume(SFX_, localSFXVolume);

	ParseCSV("Assets/Data/AudioData.csv");
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

void AudioManager::PlaySFX(const std::string& name, float pitch)
{
	const AudioClip* index = GetAudioClip(name);
	if (index != nullptr)
	{
		AEAudioPlay(index->audio, SFX_, localSFXVolume, pitch, 0);
	}
}

void AudioManager::PlayMusic(const std::string& name, float pitch)
{
	const AudioClip* index = GetAudioClip(name);
	if (index != nullptr)
	{
		AEAudioStopGroup(BGM_);
		AEAudioPlay(index->audio, BGM_, localBGMVolume, pitch, -1);
	}
}

void AudioManager::PauseMusic()
{
	AEAudioPauseGroup(BGM_);
}
void AudioManager::ResumeMusic()
{
	AEAudioResumeGroup(BGM_);
}
void AudioManager::StopMusic()
{
	AEAudioStopGroup(BGM_);
}

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
