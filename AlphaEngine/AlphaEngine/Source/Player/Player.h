#pragma once
#include "../System/SingletonTemplate.h"
#include "AEEngine.h"
#include "Transform2D.h"
#include "../Projectiles/BaseProjectile.h"

class Player : public SingletonTemplate<Player>
{	
	friend class SingletonTemplate<Player>;
public:
	Player();
	~Player();

	// Handle Inputs
	void HandleInput();

	// Update Logic
	void Update(float deltaTime);

	// Render Logic
	void Draw(AEGfxVertexList* mesh);

	// Player's Transform Properties
	Transform2D transform;


private:
	BaseProjectile* proj;
	// Future Proofing
	enum Direction
	{
		LEFT = -1,
		RIGHT = 1,
		IDLE = 0
	};

	// Player input handler
	AEVec2 input;

	// Player jump
	AEVec2 velocity;
	bool isGrounded;

	float speed, size, gravity;

	// For Camera System
	// Honestly, I don't even know where else to put it
	AEVec2 cameraPosition, cameraOffset;

};
