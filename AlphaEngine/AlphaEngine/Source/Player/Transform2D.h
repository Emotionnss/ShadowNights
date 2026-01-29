#pragma once
#include "AEEngine.h"

class Transform2D
{
public:
	Transform2D();
	Transform2D(AEVec2 pos, AEVec2 scale, float rot);

	// Get Functions
	AEVec2 GetPosition() const;
	AEVec2 GetScale() const;
	float GetRotation() const;

	// Get Matrix
	AEMtx33& GetMatrix();

	// Set Functions
	void SetPosition(float x, float y);
	void SetPosition(AEVec2 pos);
	void SetScale(float x, float y);
	void SetRotation(float r);

private:
	void UpdateMatrix();

	float rotation;
	bool needUpdate;

	AEVec2 position, scale;
	AEMtx33 transform;
};
