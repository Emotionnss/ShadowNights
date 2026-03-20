#include "SettingsManager.h"


SettingsManager::SettingsManager() :
	TILE_WIDTH{ 0.f },
	TILE_HEIGHT{ 0.f }
{
}

SettingsManager::SettingsManager(std::vector<std::string> settingsInfo) :
	TILE_WIDTH{std::stof(settingsInfo[0])},
	TILE_HEIGHT{std::stof(settingsInfo[1])},
	savedBGMVolume{std::stoi(settingsInfo[2])},
	savedSFXVolume{std::stoi(settingsInfo[3])}
{

}

s32 SettingsManager::GetSettingsBGMVolume()
{
	return savedBGMVolume;
}

s32 SettingsManager::GetSettingsSFXVolume()
{
	return savedSFXVolume;
}

void SettingsManager::CheckSettingsInputs()
{
	if (AEInputCheckTriggered(AEVK_F3))
	{
		pathDebug = !pathDebug;
	}
	if (AEInputCheckTriggered(AEVK_F4))
	{
		tilesDebug = !tilesDebug;
	}
	if (AEInputCheckTriggered(AEVK_F5))
	{
		playerCollisionDebug = !playerCollisionDebug;
	}
}

bool SettingsManager::GetPlayerColDebug() const
{
	return playerCollisionDebug;
}

void SettingsManager::SetPlayerColDebug(bool d)
{
	playerCollisionDebug = d;
}

bool SettingsManager::GetPathDebug() const
{
	return pathDebug;
}

void SettingsManager::SetPathDebug(bool d)
{
	pathDebug = d;
}

bool SettingsManager::GetTilesDebug() const
{
	return tilesDebug;
}

void SettingsManager::SetTilesDebug(bool d)
{
	tilesDebug = d;
}
