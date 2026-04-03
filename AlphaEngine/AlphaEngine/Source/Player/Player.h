#pragma once
/*!
@file       Player.h
@author     Chng Wei Sheng Bennett (chng.w)
@brief      This file contains the declarations of functions that help
			the player GameObject function with Camera System implemented
			as well.
*//*_______________________________________________________________________*/

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
	std::vector<BaseProjectile*> projectileList;

	/*!
	@brief	Constructor, Loads all necessary texture
	*/
	Player();

	/*!
	@brief	Override destructor, frees sprite and projectileList
	*/
	~Player() override;

	/*!
	@brief	Handles all Input of player, runs at start of Update
	*/
	void HandleInput();

	/*!
	@brief	AABB collision handling with tile map, uses minBounds and maxBounds
			Split into X and Y functions to resolve each collision axis individually
			This function is the X resolution of the player collision to tiles
	@param	position - The current player position
	*/
	void ResolveX(AEVec2& position);

	/*!
	@brief	AABB collision handling with tile map, uses minBounds and maxBounds
			Split into X and Y functions to resolve each collision axis individually
			This function is the Y resolution of the player collision to tiles
			Addtionally, player can pass through platform using Player's y velocity positivity
	@param	position - The current player position
	*/
	void ResolveY(AEVec2& position);

	/*!
	@brief	override function of GameObject, updates logic every frame
	@param	deltaTime - The deltaTime to update in the function
	*/
	void Update(float deltaTime) override;

	/*!
	@brief	Renders the player sprite and healthbar
	@param	mesh - The vertex list to used and draw with AlphaEngine Graphics
	*/
	void Draw(AEGfxVertexList* mesh);

	/*!
	@brief	Renders the healthbar that is called in Draw()
			Split into local function to debug healthbar with it's own UI
	@param	mesh - The vertex list to used and draw with AlphaEngine Graphics
	@param	value - The current player health points
	@param	matrix - The healthbar position to draw on screen
	*/
	void DrawHealthBar(AEGfxVertexList* mesh, f32 value, Transform2D matrix);

	/*!
	@brief	Damages the player's current health
	@param	damage - The amount of damage to reduce the current health
	*/
	void TakeDamage(f32 damage);

	/*!
	@brief	Set the player current health to argument value
	@param	val - The amount to set current health to
	*/
	void SetHealth(f32 val);

	/*!
	@brief	Get the player current health
	@return	Returns the player's current health amount
	*/
	f32 GetHealth() const;

	/*!
	@brief	Set the currently colliding tile into the local variable to store
			Returns based on X axis collided tile only
	@param	variable - The tile to set the local collided variable to store into
	*/
	void SetCollidedXTile(const Tile variable);

	/*!
	@brief	Get the currently colliding tile of X axis stored in local variable
	@return	The colliding X axis tile
	*/
	Tile GetCollidedXTile() const;

	/*!
	@brief	Set the currently colliding tile into the local variable to store
			Returns based on Y axis collided tile only
	@param	variable - The tile to set the local collided variable to store into
	*/
	void SetCollidedYTile(const Tile variable);

	/*!
	@brief	Get the currently colliding tile of Y axis stored in local variable
	@return	The colliding Y axis tile
	*/
	Tile GetCollidedYTile() const;

	/*!
	@brief	Retrieve the projectileList
	@return	Returns the vector projectileList
	*/
	std::vector<BaseProjectile*> GetProjectileList() const { return projectileList; }

	/*!
	@brief	Loops through the vector projectile list and clears them by deletion
	*/
	void ClearProjectileList();

	/*!
	@brief	Returns the sprite pointer that the player is currently running
	@return Returns Sprite Pointer of player
	*/
	Sprite* GetSprite() const;

	/*!
	@brief	Loops through map data and checks if any of the projectiles in the list is colliding
			with any of the tiles in the map.
	@param	proj - The current projectile object
	@param	mapData - The current level map data
	@return Returns true if colliding, false otherwise
	*/
	bool CheckProjectileCollision(BaseProjectile* proj, Map* mapData);

	/*!
	@brief	Set if the player is currently able to move or not in the local variable check
	@param	state - The boolean to set the canMove boolean to
	*/
	void SetMoveable(bool state);

	/*!
	@brief	Allows usage of triggering camera shake using intensity as max Limit of range motion
			Duration to set how long should the camera shake last
	@param	intensity - Acts as the max limit cap to how random the positions should jump
	@param	duration - How long should the camera shake last
	*/
	void TriggerCameraShake(f32 intensity, f32 duration);

private:
	enum Direction { LEFT = -1, RIGHT = 1, IDLE = 0 };

	Direction playerDirection = RIGHT;

	Transform2D healthbar, healthbarOverlayTfm, arrowTfm{};
	AEGfxTexture* healthBarBG, healthbarOverlay, arrowTex{};

	AEVec2 input, playerToMouseVector, playerToMouseVectorNormalized;
	AEVec2 cameraPosition, cameraOffset;

	bool isGrounded, canDoubleJump;
	bool canMove = true;

	f32 maxHP, currentHP, damagedTimer;
	f32 speed, maxSpeed, friction, size, gravity, jumpForce, coyoteTime;
	f32 swordCooldown = 0.f, swordMaxCooldown = 5.f;
	f32 shakeIntensity, shakeDuration, shakeTimer;

	Map* map;

	Tile collidedXTile, collidedYTile;

	/*!
	@brief	Calculates the normalized direction from player to cursor
			First, converts cursor position from screen space to world space using utils screen to world function
			Second, Create a local variable to fix camera offset amount (This is to ensure camera doesn't update on new position)
			^ This is due to new position updating which updates the direction where it'll repeat until 0
			Third, Add the camera position to mouse position since camera is moving with player
			Lastly, creates a vector from player to cursor in world space and normalizes it.
	@param	output - The reference vector to output the calculated normalized vector into
	@param	cameraOffsetValue - The reference vector to output the camera offset amount into
	@return Returns the normalized vector in argument output after changing the reference vector
	*/
	AEVec2 CalcPlayerToMouseDirNormalized(AEVec2& output, AEVec2& cameraOffsetValue);

	/*!
	@brief	Local function to update camera position
			Function collects the tile min and max bounds (Bottom Left to Top right)
			Clamp the camera based on the min and max bounds, accounting for the tile and camera half sizes
			Also updates camera shake inside to finalize camera position properly
	@param	_dt - The deltaTime used in update
	*/
	void UpdateCameraPosition(f32 _dt);

	/*!
	@brief	Change the direction of where the object is facing based on the AEVec2 inputValue direction (based on cursor position)
	@param	inputValue - The AEVec2 reference to help check which direction the cursor is from player
	*/
	void AimRotation(AEVec2& inputValue);

	/*!
	@brief	Change the direction of where the aim arrow is facing based on the AEVec2 inputValue direction
	@param	inputValue - The AEVec2 reference to help check what direction the cursor is from player
	*/
	void AimArrow(AEVec2& inputValue);
protected:
	std::string currentAnimState = "";

	/*!
	@brief	Plays the animation of the sprite with the string name
	@param	name - The animation name
	@param	count - The amount of times to play
			Defaults to -1 to only play once
	*/
	void PlayAnim(std::string name, s32 count = -1);
};
