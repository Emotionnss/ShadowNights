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
    /*!
    @brief    Default constructor for BatEnemy.
    *//*______________________________________________________________*/
    BatEnemy();

    /*!
    @brief    Destructor for BatEnemy.
    *//*______________________________________________________________*/
    ~BatEnemy() override;

    /*!
    @brief    Initializes the bat enemy with position, size, and sprite.
    @param    x Initial x-coordinate.
    @param    y Initial y-coordinate.
    @param    w Width of the bounding box.
    @param    h Height of the bounding box.
    @param    spriteName The string identifier for the texture.
    *//*______________________________________________________________*/
    void Init(float x, float y, float w, float h, std::string spriteName) override;

    /*!
    @brief    Updates the bat enemy's logic, including chasing and shooting.
    @param    deltaTime The time elapsed since the last frame.
    *//*______________________________________________________________*/
    void Update(float deltaTime) override;

    /*!
    @brief    Renders the bat enemy to the screen.
    *//*______________________________________________________________*/
    void Draw() override;

    /*!
    @brief    Executes the shooting behavior, spawning a projectile.
    *//*______________________________________________________________*/
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