#include "GameObject.h"

GameObject::GameObject()
	: isStatic{ true }
	, velocity{ 0.f, 0.f }
	, acceleration{ 0.f, 0.f }
	, friction{0.f}
	, mass{10.f}
	, collision{CollisionShape::NONE}
{

}

void GameObject::Init()
{

}

void GameObject::Update(f32 deltaTime)
{
	if (deltaTime) //Sample reference
	{

	}

	if (isStatic)
	{
		// If Object does not engage physics
	}
	else
	{

	}
}

void GameObject::Draw(AEGfxVertexList *pMesh)
{
	if (pMesh == nullptr)
	{
		return;
	}
}

void GameObject::Exit()
{

}
