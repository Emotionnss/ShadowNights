#pragma once
#include "../Player/Player.h"
#include "TimeStop.h"
class AbilityHandler
{
private:
	TimeStop timeStop;
	Player* player = nullptr;
	float originalProjSpeed{500.f};
public:
	AbilityHandler();
	~AbilityHandler();
	void AbilityUpdate(float deltaTime);
	void Draw();

};

