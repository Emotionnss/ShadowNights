#pragma once
#include "../Player/Player.h"
#include "../Enemy/BaseEnemy.h"
class TimeSlow
{
private:
	Player* player = nullptr;
	bool active{};
public:
	TimeSlow();
	~TimeSlow();
	void Update(std::vector<BaseEnemy*> enemyList,float dt);
	bool ToggleSlow();
};

