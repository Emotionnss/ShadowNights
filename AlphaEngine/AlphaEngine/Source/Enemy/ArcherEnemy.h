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
    /*!
    @brief    Default constructor for ArcherEnemy.
    *//*______________________________________________________________*/
    ArcherEnemy();

    /*!
    @brief    Destructor for ArcherEnemy. Cleans up any specific resources.
    *//*______________________________________________________________*/
    ~ArcherEnemy() override;

    /*!
    @brief    Initializes the archer enemy with position, size, and sprite.
    @param    x Initial x-coordinate of the enemy.
    @param    y Initial y-coordinate of the enemy.
    @param    w Width of the enemy's bounding box/transform.
    @param    h Height of the enemy's bounding box/transform.
    @param    spriteName The string identifier for the enemy's texture.
    *//*______________________________________________________________*/
    void Init(float x, float y, float w, float h, std::string spriteName) override;

    /*!
    @brief    Updates the archer enemy's logic, including aggro and shooting timers.
    @param    deltaTime The time elapsed since the last frame.
    *//*______________________________________________________________*/
    void Update(float deltaTime) override;

    /*!
    @brief    Renders the archer enemy to the screen.
    *//*______________________________________________________________*/
    void Draw() override;

    /*!
    @brief    Executes the shooting behavior, spawning a projectile aimed at the player.
    *//*______________________________________________________________*/
    void Shoot() override;

private:
    float aggroRadius = 800.0f;
    float shootTimer = 0.0f;
    float fireRate = 3.0f;
};
