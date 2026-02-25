#include "TimeSlow.h"
#include <iostream>
TimeSlow::TimeSlow()
	:player{Player::GetInstance()}
{

}

TimeSlow::~TimeSlow()
{
	player = nullptr;
}

void TimeSlow::Update(std::vector<BaseEnemy*> enemyList,float dt)
{
	if (active)
	{
		for (BaseProjectile* proj : player->GetProjectileList())
		{

		}
		for (BaseEnemy* enemy : enemyList)
		{
			enemy->Slowed(true);
		}
	}
	else
	{
		for (BaseEnemy* enemy : enemyList)
		{
			enemy->Slowed(false);
		}
	}
}

bool TimeSlow::ToggleSlow()
{
	if (active == true)
	{
		active = false;
		std::cout << "normal\n";
	}
	else
	{
		active = true;
		std::cout << "slowing\n";
	}
	return active;
}
