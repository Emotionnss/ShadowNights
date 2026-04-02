/*!
@file       SwordProjectile.h
@author     Bay Yeok Hao (yeokhao.bay)
@brief      This file contains declarations of functions used by SwordProjectile,
			and inherited methods from BaseProjectile
*//*_______________________________________________________________________*/


#pragma once
#include "BaseProjectile.h"

void ShutdownSwordResources();

class SwordProjectile : public BaseProjectile
{
public:
	/*!
	@brief      Constructor with custom parameters

	Constructor that can specify other parameters like position and direction.

	@param		AEVec2 pos - position of the sword projectile
	@param		AEVec2 dir - direction of the sword projectile
	*/
	SwordProjectile(AEVec2 pos, AEVec2 dir);
	/*!
	@brief      Destructor

	Destructor that also calls Exit()
	*/
	~SwordProjectile();
	/*!
	@brief      Update function

	Overriden update function is called every frame by scene

	@param		float deltaTime - dt passed in from scene
	*/
	void Update(float deltaTime) override;
};

