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
public:
    TimeSlow();
    ~TimeSlow();
    void Update(float dt);
    bool ToggleSlow(std::vector<BaseEnemy*> enemyList);
    void Draw();
};