#pragma once
#pragma once

#include "AEEngine.h"
#include "SingletonTemplate.h"

class SoundManager : public SingletonTemplate<SoundManager>
{
	friend class SingletonTemplate<SoundManager>;

private:
	bool pathDebug = false;
	bool tilesDebug = false;
	bool playerCollisionDebug = false;

	// Volume that reads from settings excel
	s32 bgmVolume;
	s32 sfxVolume;

public:

	/*
	SettingsManager();
	SettingsManager(std::vector<std::string> settingsInfo);

	void CheckSettingsInputs();

	bool GetPlayerColDebug() const;
	void SetPlayerColDebug(bool d);
	bool GetPathDebug() const;
	void SetPathDebug(bool d);
	bool GetTilesDebug() const;
	void SetTilesDebug(bool d);
	*/
};