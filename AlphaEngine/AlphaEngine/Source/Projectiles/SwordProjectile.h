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
	SwordProjectile(AEVec2 pos, AEVec2 dir);
	~SwordProjectile();

	void Update(float deltaTime) override;
};

