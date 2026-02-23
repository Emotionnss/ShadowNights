#pragma once
#include "BaseProjectile.h"

class SwordProjectile : public BaseProjectile
{
public:
	SwordProjectile(AEVec2 pos, AEVec2 dir);
	~SwordProjectile();

	void Update(float deltaTime) override;
};

