#include "GameObject.h"
#include <iostream>
/*!
@file       GameObject.cpp
@author     Chng Wei Sheng Bennett (chng.w)
@brief      This file contains the virtual class definitions of gameobject
			that has a Transform2D and Sprite to represent a gameobject in
			the game with init, update and draw virtual functions.
*//*_______________________________________________________________________*/

GameObject::GameObject()
	: isStatic(true), friction(0.0f), sprite(nullptr)
{
	velocity = { 0.0f, 0.0f };
	acceleration = { 0.0f, 0.0f };
}

GameObject::~GameObject()
{
	if (sprite != nullptr)
	{
		delete sprite;
	}
}

void GameObject::Update(float deltaTime)
{
	if (isStatic) return;

	velocity.x += acceleration.x * deltaTime;
	velocity.y += acceleration.y * deltaTime;

	AEVec2 pos = transform.GetPosition();

	velocity.x *= 0.99f;
	velocity.y *= 0.99f;

	pos.x += velocity.x * deltaTime;
	pos.y += velocity.y * deltaTime;

	transform.SetPosition(pos);

	if (sprite != nullptr)
	{
		sprite->Update();
	}
}

void GameObject::Draw()
{
	if (sprite != nullptr)
	{
		sprite->Render(transform);
	}
}