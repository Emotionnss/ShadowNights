/*!
@file       TimeSlow.cpp
@author     Chew Zavier (c.zavier)
@brief      This file contains definitions of functions used by the Time Slow ability which is a
			player ability that allows the user to slow down time across the whole map
*//*_______________________________________________________________________*/
#include "TimeSlow.h"
#include <iostream>

TimeSlow::TimeSlow()//default constructor
	:player{ Player::GetInstance() },
	active{ false },//bool on if the ability is active
	startDespawn{ false },//despawning boolean
	timeSlow{ "timeslow2test" },//name of the sprite in the spritesheet
	despawningTime{ 1.f },//the timer for how long the despawning animation plays
	despawningTimer{ despawningTime }//the original amount of time it takes for the animation to play
{
	timeSlowSprite = new Sprite(CSVManager::ReadInfo(timeSlow));//initialize the sprite

	timeSlowTransform.SetScale(player->GetTransform().GetScale().x * 10, player->GetTransform().GetScale().y * 10);//set the scale for the sprite
	timeSlowSprite->PlayAnimation("spawn", 1, 0.6f);//play the animation only once
}

TimeSlow::~TimeSlow()//default destructor
{
	player = nullptr;//set this class's player to nullptr
	if (timeSlowSprite)//if the sprite exists
	{
		delete timeSlowSprite;//delete it
		timeSlowSprite = nullptr;//set to nullptr
	}
}

void TimeSlow::Update(float dt)//update
{
	if (active)//if active
	{
		if (!startDespawn)//not despawning yet
			despawningTimer = despawningTime;//dont let it tick down

		
		timeSlowTransform.SetPosition(player->GetTransform().GetPosition());//set the slow sprite onto our player
		for (BaseProjectile* proj : player->GetProjectileList())//slow all projectiles thrown
		{
			proj->slowed = true;
		}
		for (BaseEnemy* enemy : enemies)//slow all enemies and their projectiles
		{
			enemy->Slowed(true);
			for (BaseProjectile* proj : enemy->GetProjectiles())
			{
				proj->slowed = true;
			}
		}
	}
	if (startDespawn)//despawning sequence
	{
		if (despawningTimer > 0)
		{
			despawningTimer += -1 * dt;
		}
		else
		{
			startDespawn = false;
		}
		for (BaseEnemy* enemy : enemies)//unslow every enemy and their projectile
		{
			enemy->Slowed(false);
			for (BaseProjectile* proj : enemy->GetProjectiles())
			{
				proj->slowed = false;
			}
		}
		for (BaseProjectile* proj : player->GetProjectileList())//unslow all projectile thrown by player
		{
			proj->slowed = false;
		}
	}
	timeSlowSprite->Update();
}

bool TimeSlow::ToggleSlow(std::vector<BaseEnemy*> enemyList)
{
	this->enemies = enemyList;
	if (active == true)//if the ability is already active
	{
		AudioManager::GetInstance()->PlaySFXOneShot("time_slow_end",1);
		startDespawn = true;//start despawning
		timeSlowSprite->PlayAnimation("despawn", 0, despawningTime);//play despawn animation
		active = false;
		for (BaseEnemy* enemy : enemyList)//unslow every enemy and their projectile
		{
			enemy->Slowed(false);
			for (BaseProjectile* proj : enemy->GetProjectiles())
			{
				proj->slowed = false;
			}
		}
		for (BaseProjectile* proj : player->GetProjectileList())//unslow all projectile thrown by player
		{
			proj->slowed = false;
		}
	}
	else
	{
		timeSlowSprite->PlayAnimation("spawn", 0, 0.6f);//play spawning animation
		startDespawn = false;//not despawning
		active = true;//is active
		AudioManager::GetInstance()->PlaySFXOneShot("time_slow_start");
		for (BaseProjectile* proj : player->GetProjectileList())//slow all projectiles thrown
		{
			proj->slowed = true;
		}
		for (BaseEnemy* enemy : enemyList)//slow all enemies and their projectiles
		{
			enemy->Slowed(true);
			for (BaseProjectile* proj : enemy->GetProjectiles())
			{
				proj->slowed = true;
			}
		}
	}
	return active;
}

void TimeSlow::Draw()
{
	//sprite for timeslow
	if (active || startDespawn)
	{
		timeSlowSprite->Render(timeSlowTransform, 0.75f);
	}
}