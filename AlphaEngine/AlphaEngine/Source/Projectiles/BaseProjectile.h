/*!
@file       BaseProjectile.h
@author     Bay Yeok Hao (yeokhao.bay), Lim Chee Kang (l.cheekang)
@brief      This file contains declarations of functions used by BaseProjectile,
            a base class that all other projectiles will inherit from
*//*_______________________________________________________________________*/

#pragma once
#include "../GameObject/Transform2D.h"

void ShutdownKnifeResources();

class BaseProjectile
{
protected:
    float timeLeft = 2.f;
    AEVec2 direction;
    float currLifespan = timeLeft;

public:
    float oriSpeed, speed, size;
    Transform2D transform;
    AEGfxTexture* projTex;

    bool stopped{};
    bool slowed{};
    bool collidesWithTiles = true;

    /*!
    @brief      Constructor with custom parameters

    Constructor that can specify other parameters like baseLifespan and startupTime.

    @param		float baseLifespan - custom lifespan of the laser
    @param		float startupTime  - custom charging time of laser
    */
    BaseProjectile(AEVec2 pos, AEVec2 dir, bool skipTextureLoad = false);
    /*!
    @brief      Destructor

    Virtual destructor that also calls Exit()
    */
    virtual ~BaseProjectile();
    /*!
    @brief      Update function

    Virtual update function is called every frame by scene

    @param		float deltaTime - dt passed in from scene
    */
    virtual void Update(float deltaTime);
    /*!
    @brief      Draw function

    Draw function is called to render laser every frame

    @param      AEGfxVertexList* mesh - pointer to mesh to draw
    */
    virtual void Draw(AEGfxVertexList* mesh);
    /*!
    @brief      Exit function

    Spawns particles when the projectile is destroyed
    */
    void Exit();
    /*!
    @brief      Getter function to get lifespan

    Gets current lifespan of projectile, used in scene to handle deletion

    @return     float - current lifespan of projectile
    */
    float GetLifespan();
    /*!
    @brief      Setter function to set lifespan

    Sets current lifespan of projectile to a value

    @param      float time - new lifespan to overwrite
    */
    void SetLifespan(float time);
    /*!
    @brief      Getter function to get direction of projectile

    Gets current direction of projectile

    @return     AEVec2 - current direction of projectile
    */
    AEVec2 GetDirection();
    /*!
    @brief      Setter function to set direction

    Sets current direction of projectile to a new direction vector

    @param      AEVec2 newDir - new direction to overwrite
    */
    void SetDirection(AEVec2 newDir);
    /*!
    @brief      Setter function to set velocity

    Sets current velocity of projectile to a new velocity

    @param      float speed - new speed to overwrite
    */
    void SetSpeed(float speed);
};
