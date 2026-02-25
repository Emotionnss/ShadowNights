#pragma once
#include "BaseProjectile.h"
#include "../Sprite/Sprite.h"

class EnemyProjectile : public BaseProjectile
{
public:

    EnemyProjectile(AEVec2 pos, float directionX);
    ~EnemyProjectile();

    void Update(float deltaTime) override;

    void Draw();

    bool IsDead();
    void Speed(float newSpeed);
    float Speed() const;
    float OriginalSpeed() const;

private:
    Sprite* sprite = nullptr;
};