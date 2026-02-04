#pragma once
#include "../Player/Player.h"
class TimeSlow
{
private:
	Player* player = nullptr;
public:
	void Init();
	void Update(float dt);
	void Free();
};

