#pragma once
#include "AEEngine.h"
#include "Transform2D.h"
#include "../Sprite/Sprite.h"

class GameObject {
public:
    GameObject();

    virtual ~GameObject();

    virtual void Init() {}

    virtual void Update(float deltaTime);

    virtual void Draw();

    Transform2D& GetTransform() { return transform; }

    void SetVelocity(float x, float y) { velocity.x = x; velocity.y = y; }

    const AEVec2 GetVelocity() { return velocity; }

    Sprite* sprite;

protected:
    Transform2D transform;

    AEVec2 velocity;
    AEVec2 acceleration;
    float friction;
    bool isStatic;

};