/*!
@file       BossExplosive.h
@author     Bay Yeok Hao (yeokhao.bay)
@brief      This file contains declarations of functions used by BossExplosive
			which is an object created by BossEnemy when attacking
*//*_______________________________________________________________________*/

#include "../Player/Player.h"
#include "../GameObject/Transform2D.h"
#include "../System/MeshManager.h"
#include "../Projectiles/BaseProjectile.h"

#include <vector>

#pragma once
class BossExplosive
{
private:
	BaseProjectile* bigProjectile = nullptr;
	std::vector <BaseProjectile*> smallProjectiles;

	bool active = false;
	Player* player = nullptr;
	MeshManager* meshManager = nullptr;

	int numBullets;

public:
	BossExplosive();
	BossExplosive(int numBullets);
	~BossExplosive();

	void Update(float deltaTime);
	void Draw();

	bool Triggered(AEVec2 pos, AEVec2 dir);
	void Explode(AEVec2 origin);
};
