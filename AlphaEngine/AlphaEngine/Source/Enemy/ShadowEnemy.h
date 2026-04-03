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
    /*!
    @brief    Default constructor for ShadowEnemy.
    *//*______________________________________________________________*/
    ShadowEnemy();

    /*!
    @brief    Destructor for ShadowEnemy.
    *//*______________________________________________________________*/
    ~ShadowEnemy() override;

    /*!
    @brief    Initializes the shadow enemy with position, size, and sprite.
    @param    x Initial x-coordinate.
    @param    y Initial y-coordinate.
    @param    w Width of the bounding box.
    @param    h Height of the bounding box.
    @param    spriteName The string identifier for the texture.
    *//*______________________________________________________________*/
    void Init(float x, float y, float w, float h, std::string spriteName) override;

    /*!
    @brief    Updates the shadow enemy's logic, including teleportation and attacks.
    @param    deltaTime The time elapsed since the last frame.
    *//*______________________________________________________________*/
    void Update(float deltaTime) override;

    /*!
    @brief    Renders the shadow enemy to the screen.
    *//*______________________________________________________________*/
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