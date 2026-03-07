#include "../Player/Player.h"
#include "../Player/Transform2D.h"
#include "../Sprite/Sprite.h"
#include "../System/MeshManager.h"

#pragma once
class BossExplosive
{
private:

public:
	BossExplosive();
	~BossExplosive();

	bool Triggered(AEVec2 pos, AEVec2 dir);

	void Explode(AEVec2 origin);
};

