#pragma once
/*!
@file       AudioManager.h
@author     Chng Wei Sheng Bennett (chng.w)
@brief      This file contains declarations of functions and variables
			used for the Audio in the game such as playing Sound Effects
			or Music.
*//*_______________________________________________________________________*/

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
Reads audio clips from audio data in the CSV sheet
Row 1 - name of clip to play
Row 2 - audio file name with file type like .wav or .mp3
Row 3 - If it's a music BGM, false if it isn't
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
	bool SFXplaying, BGMplaying, isMuted;

	std::vector<AudioClip> AudioClipList;

	AudioClip currentMusic;
	AudioClip currentSFX;

	SettingsManager* settings = nullptr;

	/*!
	@brief	Splits the string
	@param	str - The string input to split
	*/
	void Trim(std::string& str);

	/*!
	@brief	Used to trim CSV into AudioClip struct and
			push into the AudioClipList
	@param	str - The file path of the CSV
	*/
	void ParseCSV(const std::string& csvPath);

	/*!
	@brief	Takes the AudioClipList and loads them using AELoad
	*/
	void LoadAll();

	/*!
	@brief	Searches the list with the argument name and
			returns a pointer to it
	@param	name - The clip name to find using std::string
	@return AudioClip pointer of the clip
	*/
	const AudioClip* GetAudioClip(const std::string& name) const;

public:

	/*!
	@brief	Constructor, Creates the AEGroups and collects
			the volume from the settings
	*/
	AudioManager();

	/*!
	@brief	Destructor, Unloads the AEGroups and all the audioclips
	*/
	~AudioManager();

	/*!
	@brief	Returns if the audio should be muted
	@return boolean of isMuted
	*/
	bool GetMuteStatus() const { return isMuted; }

	/*!
	@brief	Reverse the boolean of isMuted
	*/
	void SetMute();

	/*!
	@brief	Plays audio clip one shot (No loop)
	@param	name - The clip name to play
	@param	volume - volume of the clip, defaults to 1.0f
	@param	pitch - pitch of the clip, defaults to 1.0f
	*/
	void PlaySFXOneShot(const std::string& name, f32 volume = 1.0f, f32 pitch = 1.0f);

	/*!
	@brief	Plays audio clip with loop
	@param	name - The clip name to play
	@param	volume - volume of the clip, defaults to 1.0f
	@param	pitch - pitch of the clip, defaults to 1.0f
	*/
	void PlayEffect(const std::string& name, f32 volume = 1.0f, f32 pitch = 1.0f);

	/*!
	@brief	Pause the sound effect group
	*/
	void PauseEffect() const;

	/*!
	@brief	Resumes the sound effect group
	*/
	void ResumeEffect() const;

	/*!
	@brief	Completely stops the sound effect group
	*/
	void StopEffect();

	/*!
	@brief	Checks if the sound effect group is playing
	@return Returns the sound effect boolean
	*/
	bool GetEffectGroup() const;

	/*!
	@brief	Retrieves the currently playing audio clip in sound effect group
	@return Returns the AudioClip struct currently being played
	*/
	AudioClip GetCurrentEffectClip() const;

	/*!
	@brief	Plays audio with loop in music group
	@param	name - The clip name to play
	@param	volume - volume of the clip, defaults to 1.0f
	@param	pitch - pitch of the clip, defaults to 1.0f
	*/
	void PlayMusic(const std::string& name, f32 volume = 1.0f, f32 pitch = 1.0f);

	/*!
	@brief	Pause the music group
	*/
	void PauseMusic() const;

	/*!
	@brief	Resumes the music group
	*/
	void ResumeMusic() const;

	/*!
	@brief	Completely stops the music group
	*/
	void StopMusic();

	/*!
	@brief	Checks if the music group is playing
	@return Returns the music group boolean
	*/
	bool GetMusicGroup() const;

	/*!
	@brief	Retrieves the currently playing audio clip in music group
	@return Returns the AudioClip struct currently being played
	*/
	AudioClip GetCurrentMusicClip() const;

	/*!
	@brief	Set the volume of the music group with argument volume
	@param	volume - The float to set the music group
	*/
	void  SetBGMVolume(f32 volume);

	/*!
	@brief	Set the volume of the sound effect group with argument volume
	@param	volume - The float to set the sound effect group
	*/
	void  SetSFXVolume(f32 volume);

	/*!
	@brief	Get the volume of the music group with argument volume
	@return	Returns the float value of music group
	*/
	float GetBGMVolume() const;

	/*!
	@brief	Get the volume of the music group with argument volume
	@return	Returns the float value of sound effect group
	*/
	float GetSFXVolume() const;
};
