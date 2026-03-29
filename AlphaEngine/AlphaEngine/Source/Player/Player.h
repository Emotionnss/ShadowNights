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

#include "../System/AudioManager.h"

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

	void ResolveX(AEVec2& position);
	void ResolveY(AEVec2& position);
	//void ComputeCollision(AEVec2& position);

	void Update(float deltaTime) override;

	void Draw(AEGfxVertexList* mesh);
	void DrawHealthBar(AEGfxVertexList* mesh, float value, Transform2D matrix);

	// I hate whoever forced me to do this
	template <typename T>
	void TakeDamage(T damage);

	void SetHealth(float val);
	f32 GetHealth();

	void SetCollidedXTile(const Tile variable);
	Tile GetCollidedXTile();

	void SetCollidedYTile(const Tile variable);
	Tile GetCollidedYTile();

	std::vector<BaseProjectile*> GetProjectileList() { return projectileList; }
	void ClearProjectileList();

	std::vector<BaseProjectile*> projectileList;

	Sprite* GetSprite() const;

	bool CheckProjectileCollision(BaseProjectile* proj, Map* map);

	void SetMoveable(bool state);

private:
	AEVec2 playerToMouseVector, playerToMouseVectorNormalized;
	AEVec2 CalcPlayerToMouseDirNormalized(AEVec2& output);

	void AimRotation(AEVec2& input);
	void AimArrow(AEVec2& input);
	Transform2D arrowTfm{};
	AEGfxTexture* arrowTex{};
	Transform2D healthbar;
	AEGfxTexture* healthbarOverlay;
	Transform2D healthbarOverlayTfm;
	AEGfxTexture* healthBarBG;
	enum Direction { LEFT = -1, RIGHT = 1, IDLE = 0 };
	Direction playerDirection = RIGHT;

	AEVec2 input;
	bool isGrounded;

	f32 speed, maxSpeed, friction, size, gravity, jumpForce;

	f32 swordCooldown = 0.f;
	f32 swordMaxCooldown = 5.f;

	f32 maxHP, currentHP;

	f32 damagedTimer;

	//AEVec2 cameraPosition, cameraOffset;
	AEVec2 cameraPosition;

	Map* map;

	Tile collidedXTile, collidedYTile;

	bool canMove = true;
protected:
	std::string currentAnimState = "";
	void PlayAnim(std::string name, s32 count = -1);
};
