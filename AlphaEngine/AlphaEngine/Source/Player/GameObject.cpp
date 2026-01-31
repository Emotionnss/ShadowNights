#include "GameObject.h"

GameObject::GameObject()
    : isStatic(true), friction(0.0f)
{
    velocity = { 0.0f, 0.0f };
    acceleration = { 0.0f, 0.0f };
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
}

void GameObject::Draw() {
    
}