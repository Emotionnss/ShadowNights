#pragma once

#include "AEEngine.h"
#include "../System/SingletonTemplate.h"
#include "GameObject.h"
#include "../System/Map.h"
#include "../System/CSVManager.h"
#include "../utils.h"

#include "../Projectiles/BaseProjectile.h"

#include <memory>
#include <iostream>
#include <vector>

class Player : public GameObject, public SingletonTemplate<Player>
{
	friend class SingletonTemplate<Player>;
public:
	Player();
	~Player() override;

	void HandleInput();
	void ComputeCollision(AEVec2 &position);

	void Update(float deltaTime) override;

	void Draw(AEGfxVertexList* mesh);

	std::vector<BaseProjectile*> GetProjectileList() { return projectileList; }

private:
	enum Direction { LEFT = -1, RIGHT = 1, IDLE = 0 };
	Direction playerDirection = IDLE;

	AEVec2 input;
	bool isGrounded;

	float speed, maxSpeed, friction, size, gravity;

	AEVec2 cameraPosition, cameraOffset;

	Map* map;

	std::vector<BaseProjectile*> projectileList;
};
