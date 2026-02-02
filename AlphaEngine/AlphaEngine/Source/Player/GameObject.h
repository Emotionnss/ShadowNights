#pragma once
#include "AEEngine.h"
#include "Transform2D.h"
#include "../Sprite/Sprite.h"
#include "../System/MeshManager.h"

class GameObject {
public:
	GameObject();

	virtual ~GameObject();

	virtual void Init() {}

	virtual void Update(float deltaTime);

	virtual void Draw();

	Transform2D& GetTransform() { return transform; }

	void SetVelocity(float x, float y) { velocity.x = x; velocity.y = y; }
	const AEVec2 GetVelocity() const { return velocity; }

	void SetAcceleration(float x, float y) { acceleration.x = x; acceleration.y = y; }
	const AEVec2 GetAcceleration() { return acceleration; };

	void SetStatic(bool state) { isStatic = state; }
	bool const GetStatic() { return isStatic; }

	Sprite* sprite;
protected:
	Transform2D transform;

	AEVec2 velocity;
	AEVec2 acceleration;
	float friction;
	bool isStatic;
};
