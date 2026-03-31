#pragma once

#include "AEEngine.h"
#include "../System/SingletonTemplate.h"
#include "../GameObject/GameObject.h"
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
	void TakeDamage(float damage);

	void SetHealth(float val);
	f32 GetHealth() const;

	void SetCollidedXTile(const Tile variable);
	Tile GetCollidedXTile() const;

	void SetCollidedYTile(const Tile variable);
	Tile GetCollidedYTile() const;

	std::vector<BaseProjectile*> GetProjectileList() const { return projectileList; }
	void ClearProjectileList();

	std::vector<BaseProjectile*> projectileList;

	Sprite* GetSprite() const;

	bool CheckProjectileCollision(BaseProjectile* proj, Map* mapData);

	void SetMoveable(bool state);

	void TriggerCameraShake(f32 intensity, f32 duration);

private:
	AEVec2 playerToMouseVector, playerToMouseVectorNormalized;
	AEVec2 CalcPlayerToMouseDirNormalized(AEVec2& output, AEVec2& cameraOffsetValue);

	void UpdateCameraPosition(f32 _dt);
	f32 shakeIntensity, shakeDuration, shakeTimer;

	void AimRotation(AEVec2& inputValue);
	void AimArrow(AEVec2& inputValue);
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
	AEVec2 cameraPosition, cameraOffset;

	Map* map;

	Tile collidedXTile, collidedYTile;

	bool canMove = true;
protected:
	std::string currentAnimState = "";
	void PlayAnim(std::string name, s32 count = -1);
};
