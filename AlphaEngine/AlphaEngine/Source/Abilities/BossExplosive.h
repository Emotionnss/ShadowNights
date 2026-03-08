#include "../Player/Player.h"
#include "../Player/Transform2D.h"
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
	void Exit();

	bool Triggered(AEVec2 pos, AEVec2 dir);
	void Explode(AEVec2 origin);
};

