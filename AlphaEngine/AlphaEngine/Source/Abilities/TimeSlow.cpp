#include "TimeSlow.h"
#include <iostream>
TimeSlow::TimeSlow()//default constructor
	:player{ Player::GetInstance() },
	active{ false },//bool on if the ability is active
	startDespawn{ false },//despawning boolean
	timeSlow{ "timeslow" },//name of the sprite in the spritesheet
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

	}
	if (startDespawn)
	{
		if (despawningTimer > 0)
		{
			despawningTimer += -1 * dt;
		}
		else
		{
			startDespawn = false;
		}
	}
	timeSlowSprite->Update();
}

bool TimeSlow::ToggleSlow(std::vector<BaseEnemy*> enemyList)
{
	if (active == true)
	{
		startDespawn = true;
		timeSlowSprite->PlayAnimation("despawn", 1, despawningTime);
		active = false;
		for (BaseEnemy* enemy : enemyList)
		{
			enemy->Slowed(false);
			for (BaseProjectile* proj : enemy->GetProjectiles())
			{
				proj->slowed = false;
			}
		}
		for (BaseProjectile* proj : player->GetProjectileList())
		{
			proj->slowed = false;
		}
	}
	else
	{
		timeSlowSprite->PlayAnimation("spawn", 1, 0.6f);
		startDespawn = false;
		active = true;
		for (BaseProjectile* proj : player->GetProjectileList())//e
		{
			proj->slowed = true;
		}
		for (BaseEnemy* enemy : enemyList)
		{
			enemy->Slowed(true);
			for (BaseProjectile* proj : enemy->GetProjectiles())
			{
				proj->slowed = false;
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