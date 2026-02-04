#pragma once
#include "../Player/Player.h"
#include "TimeStop.h"
#include "TimeSlow.h"
class AbilityHandler
{
private:
	//TimeSlow timeSlow{};
	TimeStop *timeStop = new TimeStop();
	Player* player = nullptr;
public:
	AbilityHandler();
	~AbilityHandler();
	void AbilityUpdate(float deltaTime);
	void Draw();

};

