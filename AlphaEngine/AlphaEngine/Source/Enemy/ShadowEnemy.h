#pragma once
#include "BaseEnemy.h"

/*!
@file       ShadowEnemy.h
@author     Lim Chee Kang (l.cheekang)
@brief      This file contains definitions of functions and variables
             used for the ShadowEnemy class, which is a melee enemy that
             teleports around the player and attacks them with a short-range
			 attack when they get close.
*//*_______________________________________________________________________*/

class ShadowEnemy : public BaseEnemy
{
public:
    ShadowEnemy();
    ~ShadowEnemy() override;

    void Init(float x, float y, float w, float h, std::string spriteName) override;
    void Update(float deltaTime) override;
    void Draw() override;

private:
    // Attack Logic
    bool isAttacking = false;
    float attackTimer = 0.0f;
    float originalTimer = 1.0f;
    bool hasFired = false;

    // Aggro Logic
    bool isAggroed = false;
    bool isSurprised = false;
    float surpriseTimer = 0.0f;
};

