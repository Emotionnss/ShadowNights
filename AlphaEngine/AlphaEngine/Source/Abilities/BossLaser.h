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
#include <fstream>

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
	/*!
	@brief      Constructor that inits all necessary info

	Default constructor that is run when ability is created by boss.
	*/
	BossLaser();
	/*!
	@brief      Overload constructor with custom parameters

	Custom constructor that can specify other parameters like baseLifespan and startupTime.

	@param		float baseLifespan - custom lifespan of the laser 
	@param		float startupTime  - custom charging time of laser
	*/
	BossLaser(float baseLifespan, float startupTime);
	/*!
	@brief      Destructor

	Destructor cleans up all resources and is called when laser has expired
	*/
	~BossLaser();
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
	@brief      Triggered function for when laser is activated

	Triggered function is called by boss to enable laser and start charging

	@param		AEVec2 pos - where to place center of laser at
	*/
	bool Triggered(AEVec2 pos);

	/*!
	@brief    Serializes the current internal state of the ability to an output file stream.

	This function writes the active status, phase timers, and transformation data
	of the ability (and any of its internal projectiles) into the save file.
	It is called by BossScene during its specialized snapshot save process.

	@param    f The output file stream (std::ofstream) associated with the current save slot.
	@return   void
	*//*______________________________________________________________*/
	void SaveState(std::ofstream& f);

	/*!
	@brief    Deserializes and reconstructs the ability's state from an input file stream.

	This function reads the saved parameters (such as lifespan, charging status,
	and positions) from the save file. It ensures that animations and physics
	are immediately resynced so the ability resumes seamlessly without
	graphical stretching or "jumping."

	@param    f The input file stream (std::ifstream) associated with the current save slot.
	@return   void
	*//*______________________________________________________________*/
	void LoadState(std::ifstream& f);
};

