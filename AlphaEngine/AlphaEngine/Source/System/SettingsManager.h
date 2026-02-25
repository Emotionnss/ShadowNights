#pragma once

#include "AEEngine.h"
#include "SingletonTemplate.h"

class SettingsManager : public SingletonTemplate<SettingsManager>
{
	friend class SingletonTemplate<SettingsManager>;

private:
	bool pathDebug = false;
	bool tilesDebug = false;
	bool playerCollisionDebug = false;

public:

	void CheckSettingsInputs();

	bool GetPlayerColDebug() const;
	void SetPlayerColDebug(bool d);
	bool GetPathDebug() const;
	void SetPathDebug(bool d);
	bool GetTilesDebug() const;
	void SetTilesDebug(bool d);
};