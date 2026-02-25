#include "SettingsManager.h"

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
