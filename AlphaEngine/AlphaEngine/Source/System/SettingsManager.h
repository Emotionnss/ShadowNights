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

	bool GetCameraShake();
	void SetCameraShake();

	SettingsManager();
	SettingsManager(std::vector<std::string> settingsInfo);

	void CheckSettingsInputs();

	s32 GetSettingsBGMVolume();
	s32 GetSettingsSFXVolume();

	void SaveSettings(s32 bgm, s32 sfx);

	bool GetPlayerColDebug() const;
	void SetPlayerColDebug(bool d);
	bool GetPathDebug() const;
	void SetPathDebug(bool d);
	bool GetTilesDebug() const;
	void SetTilesDebug(bool d);
};