#include "TimeSlow.h"
#include <iostream>
TimeSlow::TimeSlow()
	:player{ Player::GetInstance() }, 
	active{ false },
	startDespawn{false},
	timeSlow{ "timeslow2" },
	despawningTime{3.f},
	despawningTimer{despawningTime}
{
	timeSlowSprite = new Sprite(CSVManager::ReadInfo(timeSlow));

	timeSlowTransform.SetScale(player->GetTransform().GetScale().x,player->GetTransform().GetScale().y);
	
}

TimeSlow::~TimeSlow()
{
	player = nullptr;
	if (timeSlowSprite)
	{
		delete timeSlowSprite;
		timeSlowSprite = nullptr;
	}
}

void TimeSlow::Update(std::vector<BaseEnemy*> enemyList,float dt)
{
	if (active)
	{
		if(!startDespawn)
			despawningTimer = despawningTime;

		
		timeSlowTransform.SetPosition(player->GetTransform().GetPosition());
		for (BaseProjectile* proj : player->GetProjectileList())
		{
			proj->slowed = true;
		}
		for (BaseEnemy* enemy : enemyList)
		{
			enemy->Slowed(true);
		}
	}
	else if (!active)
	{
		for (BaseEnemy* enemy : enemyList)
		{
			enemy->Slowed(false);
		}
		for (BaseProjectile* proj : player->GetProjectileList())
		{
			proj->slowed = false;
		}
	}
	if(startDespawn)
	{
		if (despawningTimer > 0)
		{
			despawningTimer += -1 * dt;
			timeSlowSprite->PlayAnimation("despawn");
		}
		else
		{
			startDespawn = false;
		}
	}
	timeSlowSprite->Update();
}

bool TimeSlow::ToggleSlow()
{
	if (active == true)
	{
		startDespawn = true;
		active = false;
	}
	else
	{
		timeSlowSprite->PlayAnimation("spawn",1,10.f);
		startDespawn = false;
		active = true;
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
