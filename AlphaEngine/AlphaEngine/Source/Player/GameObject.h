#pragma once
#include "AEEngine.h"
#include "Transform2D.h"
#include "../System/Sprite.h"

class GameObject {
public:
    GameObject();
    virtual ~GameObject() {}

    virtual void Init() {}

    virtual void Update(float deltaTime);

    virtual void Draw();

    Transform2D& GetTransform() { return transform; }

    void SetVelocity(float x, float y) { velocity.x = x; velocity.y = y; }

    Sprite sprite;

protected:
    Transform2D transform;

    bool isStatic;
    AEVec2 velocity;
    AEVec2 acceleration;
    float friction;
};