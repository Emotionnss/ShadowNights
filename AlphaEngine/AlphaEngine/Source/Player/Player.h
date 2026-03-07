#pragma once

#include "AEEngine.h"
#include "../System/SingletonTemplate.h"
#include "GameObject.h"
#include "../System/Map.h"
#include "../System/CSVManager.h"
#include "../Utils/utils.h"
#include "../System/SettingsManager.h"

#include "../Projectiles/BaseProjectile.h"
#include "../Projectiles/SwordProjectile.h"

#include <memory>
#include <iostream>
#include <vector>

struct LineSegment
{
	AEVec2 P0;      // Start point of directed line segment
	AEVec2 P1;      // End   point of directed line segment
	AEVec2 n_hat;   // Normalised outward normal:  n = (ey, -ex)  (rotate e by -90 deg)
};

class Player : public GameObject, public SingletonTemplate<Player>
{
	friend class SingletonTemplate<Player>;
public:
	Player();
	~Player() override;

	void HandleInput();

	void ResolveX(AEVec2& position);
	void ResolveY(AEVec2& position);
	//void ComputeCollision(AEVec2& position);

	void Update(float deltaTime) override;

	void Draw(AEGfxVertexList* mesh);
	void DrawHealthBar(AEGfxVertexList* mesh, float value, Transform2D matrix);

	void TakeDamage(float damage);
	void TakeDamage(int value) { currentHP -= value; }

	void SetHealth(float val);
	f32 GetHealth();

	void SetCollidedTile(const Tile variable);
	Tile GetCollidedTile();

	std::vector<BaseProjectile*> GetProjectileList() { return projectileList; }

	std::vector<BaseProjectile*> projectileList;

private:

	Transform2D healthbar;

	enum Direction { LEFT = -1, RIGHT = 1, IDLE = 0 };
	Direction playerDirection = IDLE;

	AEVec2 input;
	bool isGrounded;

	f32 speed, maxSpeed, friction, size, gravity, jumpForce;

	f32 maxHP, currentHP;

	AEVec2 cameraPosition, cameraOffset;

	Map* map;

	Tile collidedTile;
};
