#include "Transform2D.h"

Transform2D::Transform2D()
	: position{ 0.f,0.f }
	, scale{ 1.f, 1.f }
	, rotation{ 0.f }
	, needUpdate{ true }
{
	AEMtx33Identity(&transform);
}

Transform2D::Transform2D(AEVec2 pos, AEVec2 scale, float rot)
	: position{ pos }
	, scale{ scale }
	, rotation{ rot }
	, needUpdate{ true }
{
	AEMtx33Identity(&transform);
}

AEVec2 Transform2D::GetPosition() const
{
	return position;
}

AEVec2 Transform2D::GetScale() const
{
	return AEVec2();
}

float Transform2D::GetRotation() const
{
	return rotation;
}

void Transform2D::SetPosition(float x, float y)
{
	position = { x, y };
	needUpdate = true;
}

void Transform2D::SetPosition(AEVec2 pos)
{
	position = pos;
	needUpdate = true;
}

void Transform2D::SetScale(float x, float y)
{
	scale = { x, y };
	needUpdate = true;
}

void Transform2D::SetRotation(float r)
{
	rotation = { r };
	needUpdate = true;
}

AEMtx33& Transform2D::GetMatrix()
{
	if (needUpdate)
	{
		UpdateMatrix();
	}
	return transform;
}

void Transform2D::UpdateMatrix()
{
	AEMtx33 scaleMatrix, rotationMatrix, translateMatrix;

	AEMtx33Scale(&scaleMatrix, scale.x, scale.y);
	AEMtx33Rot(&rotationMatrix, rotation);
	AEMtx33Trans(&translateMatrix, position.x, position.y);

	AEMtx33Concat(&transform, &rotationMatrix, &scaleMatrix);
	AEMtx33Concat(&transform, &translateMatrix, &transform);

	needUpdate = false;
}
