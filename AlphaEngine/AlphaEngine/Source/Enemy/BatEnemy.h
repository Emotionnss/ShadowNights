#pragma once
#include "BaseEnemy.h"
#include "../System/MeshManager.h"

/*!
@file       BatEnemy.h
@author     Lim Chee Kang (l.cheekang)
@brief      This file contains definitions of functions and variables
             used for the BatEnemy class, which is a flying enemy that chases
			 the player when they get close, and shoots projectiles at them.
*//*_______________________________________________________________________*/

class BatEnemy : public BaseEnemy
{
public:
    BatEnemy();
    ~BatEnemy() override;

    void Init(float x, float y, float w, float h, std::string spriteName) override;
    void Update(float deltaTime) override;
    void Draw() override;
    void Shoot() override;

private:
    bool isAwake = false;
    float aggroRadius = 400.0f;
    float shootTimer = 2.0f;

    float startX = 0.0f;
    float startY = 0.0f;

    int pathIndex = 0;
    float pathTimer = 0.0f;
    bool showDebugPath = false;
};