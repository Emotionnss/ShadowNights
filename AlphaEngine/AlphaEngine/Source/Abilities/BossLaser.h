#include "../Player/Transform2D.h"
#include "../Sprite/Sprite.h"
#include "../System/MeshManager.h"

#pragma once
class BossLaser
{
private:
	bool active{};

	MeshManager* meshManager = nullptr;
	Sprite* laserSprite = nullptr;

	std::string spriteName;
	Transform2D transform{};

public:
	BossLaser();
	~BossLaser();

	void Update(float deltaTime);
	void Draw();

	bool Triggered(AEVec2 pos);
};

