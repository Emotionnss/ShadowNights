#pragma once
/*!
@file       GameObject.h
@author     Chng Wei Sheng Bennett (chng.w)
@brief      This file contains declarations of functions that help
			concat the matrix for 2D transformations.
*//*_______________________________________________________________________*/

#include "AEEngine.h"
#include "Transform2D.h"
#include "../Sprite/Sprite.h"
#include "../System/MeshManager.h"

class GameObject {
public:
	/*!
	@brief	Constructor
	*/
	GameObject();

	/*!
	@brief	Destructor
	*/
	virtual ~GameObject();

	/*!
	@brief	Virtual function for children to override
	*/
	virtual void Init() {}

	/*!
	@brief	Virtual function for children to override
			Default updates transforms with velocity and updates sprite
	@param	deltaTime - The deltaTime for function Update
	*/
	virtual void Update(float deltaTime);

	/*!
	@brief	Virtual function for children to override
			Default renders sprite based on transform
	*/
	virtual void Draw();

	/*!
	@brief	Returns reference of the local Transform
	@return Transform2D reference
	*/
	Transform2D& GetTransform() { return transform; }

	/*!
	@brief	Returns the local AEVec2 velocity
	@return Velocity of AEVec2
	*/
	const AEVec2 GetVelocity() const { return velocity; }

	/*!
	@brief	Returns the local AEVec2 acceleration
	@return Acceleration of AEVec2
	*/
	const AEVec2 GetAcceleration() { return acceleration; };

	/*!
	@brief	Set the local velocity based on arguments
	@param	x - The x velocity to set
	@param	y - The y velocity to set
	*/
	void SetVelocity(f32 x, f32 y) { velocity.x = x; velocity.y = y; }

	/*!
	@brief	Set the local acceleration based on arguments
	@param	x - The x acceleration to set
	@param	y - The y acceleration to set
	*/
	void SetAcceleration(f32 x, f32 y) { acceleration.x = x; acceleration.y = y; }

	/*!
	@brief	Set the state of the object to the argument boolean
			If state isStatic is true, object will not have any velocity
	@param	state - The boolean to set isStatic to
	*/
	void SetStatic(bool state) { isStatic = state; }

	/*!
	@brief	Retrieves isStatic boolean information
	@return	Returns the boolean of isStatic
	*/
	bool const GetStatic() { return isStatic; }

	Sprite* sprite;
	Transform2D transform;

protected:
	AEVec2 velocity, acceleration;
	f32 friction;
	bool isStatic;
};
