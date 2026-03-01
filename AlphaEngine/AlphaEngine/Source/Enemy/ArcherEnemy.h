#pragma once
#include "BaseEnemy.h"

class ArcherEnemy : public BaseEnemy
{
public:
    ArcherEnemy();
    ~ArcherEnemy() override;

    void Init(float x, float y, float w, float h, std::string spriteName) override;
    void Update(float deltaTime) override;
    void Draw() override;
    void Shoot() override;

private:
    float aggroRadius = 800.0f;
    float shootTimer = 0.0f;
    float fireRate = 3.0f;
};
