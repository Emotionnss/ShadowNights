/*!
@file       TimeSlow.h
@author     Chew Zavier (c.zavier)
@brief      This file contains declarations of functions used by the Time Slow ability which is a
            player ability that allows the user to slow down time across the whole map
*//*_______________________________________________________________________*/
#pragma once
#include "../Player/Player.h"
#include "../Enemy/BaseEnemy.h"
#include "../System/MeshManager.h"
#include "../Sprite/Sprite.h"
class TimeSlow
{
private:
    Player* player = nullptr;//player ptr
    bool active{};//activity bool
    bool startDespawn{};//despawning bool
    Sprite* timeSlowSprite = nullptr;//sprite
    std::string timeSlow{};//name of sprite

    f32 despawningTime{};//despawning timers
    f32 despawningTimer{};
    Transform2D timeSlowTransform{};//transform the sprite
    std::vector<BaseEnemy*> enemyList;
public:

    /*!
    @brief    Default constructor
    *//*______________________________________________________________*/
    TimeSlow();
    /*!
    @brief    Default destructor
    *//*______________________________________________________________*/
    ~TimeSlow();
    /*!
    @brief    Updates the object every frame
    @param    The time inbetween each frame for time based updates
    @return   void
    *//*______________________________________________________________*/
    void Update(float dt);
    /*!
    @brief    Toggles the abilitys activity
    @param    the vector of enemys to affect
    @return   bool - true if active, false if not
    *//*______________________________________________________________*/
    bool ToggleSlow(std::vector<BaseEnemy*> enemyList);
    /*!
    @brief    Renders the ability's visuals
    @param void
    @return void
    *//*______________________________________________________________*/
    void Draw();
};