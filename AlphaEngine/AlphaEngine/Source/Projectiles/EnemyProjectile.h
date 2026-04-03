#pragma once
#include "BaseProjectile.h"
#include "../Sprite/Sprite.h"

/*!
@file       EnemyProjectile.h
@author     Lim Chee Kang (l.cheekang)
@brief      This file contains definitions of functions and variables
             used for the EnemyProjectile class, which handles the logic
             and rendering for projectiles fired by hostile entities.
*//*_______________________________________________________________________*/

class EnemyProjectile : public BaseProjectile
{
public:
    /*!
    @brief    Constructor that sets initial spawn position and movement direction.
    @param    pos The starting world coordinates.
    @param    dir The normalized vector indicating travel direction.
    *//*______________________________________________________________*/
    EnemyProjectile(AEVec2 pos, AEVec2 dir);

    /*!
    @brief    Destructor to clean up projectile specific resources.
    *//*______________________________________________________________*/
    ~EnemyProjectile();

    /*!
    @brief    Updates projectile movement, lifespan, and homing logic if applicable.
    @param    deltaTime The time elapsed since the last frame.
    *//*______________________________________________________________*/
    void Update(float deltaTime) override;

    /*!
    @brief    Renders the projectile using its assigned sprite.
    @param    mesh Pointer to the vertex list for drawing geometry.
    *//*______________________________________________________________*/
    void Draw(AEGfxVertexList* mesh) override;

    /*!
    @brief    Checks if the projectile has exceeded its maximum lifespan.
    @return   True if dead/expired, false otherwise.
    *//*______________________________________________________________*/
    bool IsDead();

    /*!
    @brief    Overrides the projectile's current movement speed.
    @param    newSpeed The new scalar speed value.
    *//*______________________________________________________________*/
    void Speed(float newSpeed);

    /*!
    @brief    Retrieves the current movement speed.
    @return   The current scalar speed value.
    *//*______________________________________________________________*/
    float Speed() const;

    /*!
    @brief    Retrieves the base starting speed before any modifiers were applied.
    @return   The original scalar speed value.
    *//*______________________________________________________________*/
    float OriginalSpeed() const;

    bool isHoming = false;

private:
    Sprite* sprite = nullptr;
};