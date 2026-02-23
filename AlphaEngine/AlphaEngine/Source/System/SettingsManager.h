#include "SingletonTemplate.h"

class SettingsManager : public SingletonTemplate<SettingsManager>
{
	friend class SingletonTemplate<SettingsManager>;

private:
	bool debug = false;

public:

	bool GetDebug() const;
	void SetDebug(bool d);
};