#pragma once

#include "AEEngine.h"
#include "../System/SingletonTemplate.h"
#include "GameObject.h"
#include "../System/Map.h"
#include "../System/CSVManager.h"
#include "../utils.h"

#include "../Projectiles/BaseProjectile.h"
#include "../Projectiles/SwordProjectile.h"

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
	void ComputeCollision(AEVec2& position);

	void Update(float deltaTime) override;

	void Draw(AEGfxVertexList* mesh);
	void DrawHealthBar(AEGfxVertexList* mesh, float value, Transform2D matrix);

	void TakeDamage(float damage);

	void TakeDamage(int value) { currentHP -= value; }

	std::vector<BaseProjectile*> GetProjectileList() { return projectileList; }

	std::vector<BaseProjectile*> projectileList;

private:

	Transform2D healthbar;

	enum Direction { LEFT = -1, RIGHT = 1, IDLE = 0 };
	Direction playerDirection = IDLE;

	AEVec2 input;
	bool isGrounded;

	float speed, maxSpeed, friction, size, gravity;

	float maxHP, currentHP;

	AEVec2 cameraPosition, cameraOffset;

	Map* map;

	
};
