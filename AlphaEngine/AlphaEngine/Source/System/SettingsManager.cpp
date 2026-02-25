#include "SettingsManager.h"

bool SettingsManager::GetDebug() const
{
	return debug;
}

void SettingsManager::SetDebug(bool d)
{
	debug = d;
}
