#pragma once
#include "BaseProjectile.h"
#include "../Sprite/Sprite.h"

class EnemyProjectile : public BaseProjectile
{
public:
    EnemyProjectile(AEVec2 pos, AEVec2 dir);
    ~EnemyProjectile();

    void Update(float deltaTime) override;

    void Draw(AEGfxVertexList* mesh) override;

    bool IsDead();
    void Speed(float newSpeed);
    float Speed() const;
    float OriginalSpeed() const;
    bool isHoming = false;

private:
    Sprite* sprite = nullptr;
};