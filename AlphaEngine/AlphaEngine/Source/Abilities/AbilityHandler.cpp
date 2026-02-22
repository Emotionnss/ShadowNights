#include "AbilityHandler.h"
#include <iostream>
#include "../Projectiles/BaseProjectile.h"
#include "../utils.h"
AbilityHandler::AbilityHandler()
{
	player = Player::GetInstance();
}

AbilityHandler::~AbilityHandler()
{
	timeStop->Free();
	delete timeStop;
}

void AbilityHandler::AbilityUpdate(float deltaTime)
{
	timeStop->Update(deltaTime);
	if (AEInputCheckTriggered(AEVK_F))
	{
		timeStop->ToggleTime(player->GetTransform().GetPosition());
	}
	if (AEInputCheckTriggered(AEVK_Q))
	{
		
	}
	
}

void AbilityHandler::Draw()
{

	timeStop->Draw();
}
