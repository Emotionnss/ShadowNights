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

	// Volume that reads from settings excel
	s32 savedBGMVolume;
	s32 savedSFXVolume;

public:
	const f32 TILE_WIDTH;
	const f32 TILE_HEIGHT;

	SettingsManager();
	SettingsManager(std::vector<std::string> settingsInfo);

	void CheckSettingsInputs();

	s32 GetSettingsBGMVolume();
	s32 GetSettingsSFXVolume();

	bool GetPlayerColDebug() const;
	void SetPlayerColDebug(bool d);
	bool GetPathDebug() const;
	void SetPathDebug(bool d);
	bool GetTilesDebug() const;
	void SetTilesDebug(bool d);
};