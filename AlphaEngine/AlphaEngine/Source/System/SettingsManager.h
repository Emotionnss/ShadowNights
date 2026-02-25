#pragma once

#include "AEEngine.h"
#include "SingletonTemplate.h"

class SettingsManager : public SingletonTemplate<SettingsManager>
{
	friend class SingletonTemplate<SettingsManager>;

private:
	bool pathDebug = false;
	bool tilesDebug = false;

public:

	void CheckSettingsInputs();

	bool GetPathDebug() const;
	void SetPathDebug(bool d);
	bool GetTilesDebug() const;
	void SetTilesDebug(bool d);
};