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
	/*!
	@brief      Constructor that inits all necessary info

	Default constructor that is run when ability is created by boss.
	*/
	BossExplosive();
	/*!
	@brief      Overload constructor with custom parameters

	Custom constructor that can specify other parameters like numBullets.

	@param		int numBullets - specify the number of smaller projectiles the big one will split into
	*/
	BossExplosive(int numBullets);
	/*!
	@brief      Destructor

	Destructor cleans up all resources and is called when all projectiles have expired
	*/
	~BossExplosive();
	/*!
	@brief      Update function

	Update function is called every frame by scene

	@param		float deltaTime - dt passed in from scene
	*/
	void Update(float deltaTime);
	/*!
	@brief      Draw function

	Draw function is called to render laser every frame
	*/
	void Draw();
	/*!
	@brief      Triggered function for when explosive is activated

	Triggered function is called by boss to shoot big explosive

	@param		AEVec2 pos - where to place center of laser at
	@param		AEVec2 dir - direction to shoot big explosive in
	*/
	bool Triggered(AEVec2 pos, AEVec2 dir);
	/*!
	@brief      Explode function for when big explosive has expired

	Explode function is called once big explosive expires, and spawns smaller projectiles where it expired

	@param		AEVec2 origin - location where all smaller projectiles will move outwards from
	*/
	void Explode(AEVec2 origin);
};
