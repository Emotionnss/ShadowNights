/*!
@file       SettingsManager.h
@author     Yap Yu Xuan, Daevon (yuxuandaevon.yap), 
@brief      This file contains the declarations to manage the settings
			used in the game
*//*_______________________________________________________________________*/

#pragma once

#include "AEEngine.h"
#include "SingletonTemplate.h"
#include <vector>
#include <string>

class SettingsManager : public SingletonTemplate<SettingsManager>
{
	friend class SingletonTemplate<SettingsManager>;

private:
	bool pathDebug = false;
	bool tilesDebug = false;
	bool playerCollisionDebug = false;
	bool cameraShake = true;

	// Volume that reads from settings excel
	s32 savedBGMVolume;
	s32 savedSFXVolume;

public:
	const f32 TILE_WIDTH;
	const f32 TILE_HEIGHT;

	/*!
	@brief	Get camera shake active
	@param	void
	@return bool
	*//*______________________________________________________________*/
	bool GetCameraShake();

	/*!
	@brief	Toggles/sets camera shake state (implementation decides toggle or explicit set)
	@param	void
	@return void
	*//*______________________________________________________________*/
	void SetCameraShake();

	/*!
	@brief	Default constructor; initializes settings manager with default values
	@param	void
	@return SettingsManager
	*//*______________________________________________________________*/
	SettingsManager();

	/*!
	@brief	Constructs SettingsManager from a vector of string settings (e.g., read from file)
	@param	std::vector<std::string> settingsInfo - list of settings values/keys
	@return SettingsManager
	*//*______________________________________________________________*/
	SettingsManager(std::vector<std::string> settingsInfo);

	/*!
	@brief	Checks for input changes to settings and applies them
	@param	void
	@return void
	*//*______________________________________________________________*/
	void CheckSettingsInputs();

	/*!
	@brief	Gets the saved background music volume
	@param	void
	@return s32 - saved BGM volume
	*//*______________________________________________________________*/
	s32 GetSettingsBGMVolume();

	/*!
	@brief	Gets the saved sound effects volume
	@param	void
	@return s32 - saved SFX volume
	*//*______________________________________________________________*/
	s32 GetSettingsSFXVolume();

	/*!
	@brief	Saves the provided BGM and SFX volumes to settings
	@param	s32 bgm - background music volume to save
	@param	s32 sfx - sound effects volume to save
	@return void
	*//*______________________________________________________________*/
	void SaveSettings(s32 bgm, s32 sfx);

	/*!
	@brief	Returns whether player collision debug is enabled
	@param	void
	@return bool - true if player collision debug is active
	*//*______________________________________________________________*/
	bool GetPlayerColDebug() const;

	/*!
	@brief	Sets the player collision debug flag
	@param	bool d - desired debug state
	@return void
	*//*______________________________________________________________*/
	void SetPlayerColDebug(bool d);

	/*!
	@brief	Returns whether path debug is enabled
	@param	void
	@return bool - true if path debug is active
	*//*______________________________________________________________*/
	bool GetPathDebug() const;

	/*!
	@brief	Sets the path debug flag
	@param	bool d - desired debug state
	@return void
	*//*______________________________________________________________*/
	void SetPathDebug(bool d);

	/*!
	@brief	Returns whether tile debug is enabled
	@param	void
	@return bool - true if tiles debug is active
	*//*______________________________________________________________*/
	bool GetTilesDebug() const;

	/*!
	@brief	Sets the tiles debug flag
	@param	bool d - desired debug state
	@return void
	*//*______________________________________________________________*/
	void SetTilesDebug(bool d);
};