#pragma once
/*!
@file       Transform2D.h
@author     Chng Wei Sheng Bennett (chng.w)
@brief      This file helps to implement the matrix usage and simplify into
			Transform class to help gameobject use simple matrix functions
			such as position, scale and rotation.
*//*_______________________________________________________________________*/

#include "AEEngine.h"

class Transform2D
{
public:
	/*!
	@brief	Constructor, creates the transform with an identity matrix
	*/
	Transform2D();

	/*!
	@brief	Constructor overload, Sets parameters based on arguments
	@param	pos - The position to set
	@param	scale - The scale to set
	@param	rot - The rotation to set
	*/
	Transform2D(AEVec2 pos, AEVec2 scale, float rot);

	/*!
	@brief	The function to retrieve the position vector
	@return Returns the vector of the transform position
	*/
	AEVec2 GetPosition() const;

	/*!
	@brief	The function to retrieve the scale vector
	@return Returns the vector of the transform scale
	*/
	AEVec2 GetScale() const;

	/*!
	@brief	The function to retrieve the scale vector
	@return Returns the vector of the transform scale
	*/
	f32 GetRotation() const;

	// Get Matrix
	/*!
	@brief	Retrieves the matrix of the current transform
	@return Returns the matrix stored in the current object
	*/
	AEMtx33& GetMatrix();

	/*!
	@brief	Set the position of vector x and y with argument floats
	@param	x - The x position to set
	@param	y - The y position to set
	*/
	void SetPosition(f32 x, f32 y);

	/*!
	@brief	SetPosition overloader,
			uses AEVec2 instead to set position to
	@param	pos - The AEVec2 position to set current position to
	*/
	void SetPosition(AEVec2 pos);

	/*!
	@brief	SetPosition overloader,
			uses AEVec2 instead to set position to
	@param	pos - The AEVec2 position to set current position to
	*/
	void SetScale(float x, float y);

	/*!
	@brief	Uses the float in argument to rotate object
			using Alpha Engine Rotate function,
			Rotates objects in radians
	@param	r - Float to rotate in radians
	*/
	void SetRotation(float r);

private:
	bool needUpdate;
	f32 rotation;
	AEVec2 position, scale;
	AEMtx33 transform;

	/*!
	@brief	Private function that uses AlphaEngine matrix functions
			to concat the matrix of the object
			Only updates when needUpdate bool is true
	*/
	void UpdateMatrix();
};
