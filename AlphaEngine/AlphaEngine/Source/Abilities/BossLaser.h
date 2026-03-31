/*!
@file       BossLaser.h
@author     Bay Yeok Hao (yeokhao.bay)
@brief      This file contains declarations of functions used by BossLaser
			which is an object created by BossEnemy when attacking
*//*_______________________________________________________________________*/

#include "../Player/Player.h"
#include "../GameObject/Transform2D.h"
#include "../Sprite/Sprite.h"
#include "../System/MeshManager.h"

#pragma once
class BossLaser
{
private:
	bool active{};
	bool charging{};

	MeshManager* meshManager = nullptr;
	Sprite* laserSprite = nullptr;

	Player* player = nullptr;

	std::string spriteName;
	Transform2D transform{};

	// attack cooldown so it doesnt instantly vaporize the player
	float baseCooldown = 2.f;
	float damageCooldown = 0.f;

	// lifespan of the entire ability
	float baseLifespan = 3.f;
	float lifespan = baseLifespan;

	// wind up for a bit of leeway
	float startupDelayBase = 3.f;
	float startupDelay = 0.f;

public:
	BossLaser();
	BossLaser(float baseLifespan, float startupTime);
	~BossLaser();

	void Update(float deltaTime);
	void Draw();

	bool Triggered(AEVec2 pos);
};

