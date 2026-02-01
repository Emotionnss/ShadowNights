#include "GameObject.h"

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