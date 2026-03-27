#pragma once
#include "BaseEnemy.h"

/*!
@file       ArcherEnemy.h
@author     Lim Chee Kang (l.cheekang)
@brief      This file contains definitions of functions and variables
             used for the ArcherEnemy class, which is a type of enemy that
             shoots projectiles at the player from a distance, and contains
			 the specific implementation of the enemy's behavior and attacks.
*//*_______________________________________________________________________*/

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
