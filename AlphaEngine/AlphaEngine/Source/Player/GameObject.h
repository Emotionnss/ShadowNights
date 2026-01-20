#pragma once
#include "AEEngine.h"
#include "Transform2D.h"

class GameObject
{
public:
	GameObject();
	virtual ~GameObject() = default; // No idea if we'll use this

	virtual void Init();
	virtual void Update(float deltaTime);
	virtual void Draw(AEGfxVertexList *pMesh);
	virtual void Exit();


	// Contains position and matrix
	Transform2D transform;

	bool isStatic;
	AEVec2 velocity, acceleration;
	float mass, friction;

	enum class CollisionShape
	{
		NONE,
		AABB,
		CIRCLE
	};

	CollisionShape collision;
};
