#include "AbilityHandler.h"
#include <iostream>
#include "../Projectiles/BaseProjectile.h"
#include "../utils.h"
AbilityHandler::AbilityHandler()
{
	timeStop.Init();
	player = Player::GetInstance();
	//std::cout << "abil timestop instance:" << &timeStop << "\n";
	//std::cout << "abil handler player: " << &player << "\n";
}

AbilityHandler::~AbilityHandler()
{
	timeStop.Free();
	//player->DestroyInstance();
}

void AbilityHandler::AbilityUpdate(float deltaTime)
{
	timeStop.Update(deltaTime);
	if (AEInputCheckTriggered(AEVK_F))
	{
		timeStop.ToggleTime(player->transform.GetPosition());
	}
	if (timeStop.TimeStopped())
	{
		for (BaseProjectile* proj : player->projectileList)
		{
			if (AreCirclesIntersecting(proj->transform.GetPosition().x, proj->transform.GetPosition().y, proj->size,
				timeStop.zoneTransform.GetPosition().x, timeStop.zoneTransform.GetPosition().y, timeStop.GetScale()))
			{
				proj->speed = 0;
			}
		}
	} 
	else if(!timeStop.TimeStopped())
	{
		std::cout << "movin\n";
		for (BaseProjectile* proj : player->projectileList)
		{
			proj->speed = originalProjSpeed;
		}
	}
	
}

void AbilityHandler::Draw()
{

	timeStop.Draw();
}
