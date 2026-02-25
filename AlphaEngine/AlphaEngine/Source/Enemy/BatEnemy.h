#pragma once
#include "BaseEnemy.h"

class BatEnemy : public BaseEnemy
{
public:
    BatEnemy();
    ~BatEnemy() override;

    void Init(float x, float y, float w, float h, std::string spriteName) override;
    void Update(float deltaTime) override;
    void Draw() override;

private:
    bool isAwake = false;
    float aggroRadius = 400.0f;

    float startX = 0.0f;
    float startY = 0.0f;
};