/*!
@file       TimeStop.cpp
@author     Chew Zavier (c.zavier)
@brief      This file contains definitions of functions used by the Time Stop ability which is a
            player ability that allows the user to stop time in an area around the activated position
*//*_______________________________________________________________________*/
#include "TimeStop.h"
#include <iostream>
#include "../Utils/utils.h"
#include "../System/CSVManager.h"
#include "../System/AudioManager.h"
TimeStop::TimeStop()//constructor
    :meshManager{ MeshManager::GetInstance() }, //mesh manager
    player{ Player::GetInstance() }, //player instance
    active{ false } //active boolean
{
    centerDespawn = new Sprite(CSVManager::ReadInfo("timestopCenterDespawn"));
    zoneCenterSprite = new Sprite(CSVManager::ReadInfo("timestopCenter"));
    zoneSprite = new Sprite(CSVManager::ReadInfo("timestopZone"));//create new sprite for the timestop zone
    centerTransform.SetScale(100.f, 100.f);
    //zoneSprite->PlayAnimation("spawn", 0, 1.f);
}

TimeStop::~TimeStop()//destructor
{
    meshManager = nullptr;
    player = nullptr;
    if (zoneSprite)
    {
        delete zoneSprite;
        zoneSprite = nullptr;
    }
    if (zoneCenterSprite)
    {
        delete zoneCenterSprite;
        zoneCenterSprite = nullptr;
    }
    if (centerDespawn)
    {
        delete centerDespawn;
        centerDespawn = nullptr;
    }
}

void TimeStop::Update(std::vector<BaseEnemy*> enemyList, float dt)//update
{
    if (active)//if time stopped
    {
        zoneScale += 1500 * dt;//rapidly increase zone
        for (BaseProjectile* proj : player->GetProjectileList())//for every projectile the player throws
        {
            if (AreCirclesIntersecting(proj->transform.GetPosition().x, proj->transform.GetPosition().y, 0.1f,
                zoneTransform.GetPosition().x, zoneTransform.GetPosition().y, GetScale()))//if its in the timestop zone
            {
                proj->stopped = true;//stop moving the projectile

            }
        }
        for (BaseEnemy* enemy : enemyList)//same for the enemies
        {
            if (AreCirclesIntersecting(enemy->transform.GetPosition().x, enemy->transform.GetPosition().y, 0.1f,
                zoneTransform.GetPosition().x, zoneTransform.GetPosition().y, GetScale()))
            {
                enemy->isStopped(true);//stop the enemies
            }
            for (EnemyProjectile* enemyProj : enemy->GetProjectiles())
            {
                if (AreCirclesIntersecting(enemyProj->transform.GetPosition().x, enemyProj->transform.GetPosition().y, 0.1f,
                    zoneTransform.GetPosition().x, zoneTransform.GetPosition().y, GetScale()))
                {
                    enemyProj->stopped = true;;//stop the enemies projectiles
                }
            }
        }
    }

    if (tempStop)
    {
        for (BaseProjectile* proj : player->GetProjectileList())//for every projectile the player throws
        {
            proj->stopped = true;//stop moving the projectile
        }
        for (BaseEnemy* enemy : enemyList)//same for the enemies
        {
            enemy->isStopped(true);//stop the enemies
            for (EnemyProjectile* enemyProj : enemy->GetProjectiles())
            {
                if (AreCirclesIntersecting(enemyProj->transform.GetPosition().x, enemyProj->transform.GetPosition().y, 0.1f,
                    zoneTransform.GetPosition().x, zoneTransform.GetPosition().y, GetScale()))
                {
                    enemyProj->stopped = true;;//stop the enemies projectiles as well
                }
            }
        }
    }

    else if (!tempStop && !active)//else if not timestopped
    {
        for (BaseProjectile* proj : player->GetProjectileList())//every player thrown projectile
        {
            proj->stopped = false;//not stopped
        }
        for (BaseEnemy* enemy : enemyList)//every enemy
        {
            enemy->isStopped(false);//not stopped
            for (EnemyProjectile* enemyProj : enemy->GetProjectiles())//every enemy thrown projectile
            {
                enemyProj->stopped = false;//dont stop the projectiles
            }
        }
        zoneScale = originalScale;//revert the zonescale back to the original

    }
    if (despawning)
    {
        despawnTimer -= dt;
        if (despawnTimer < 0)
        {
            despawning = false;
            despawnTimer = despawnTime;
        }
    }
    zoneScale = AEClamp(zoneScale, originalScale, maxScale);//clamp the zone size
    zoneTransform.SetScale(zoneScale, zoneScale);//update the scale of the zone

    zoneSpriteTfm.SetPosition(zoneTransform.GetPosition());
    zoneSpriteTfm.SetScale(zoneScale * 2.f, zoneScale * 2.f);//*2 bc sprite is smaller than zone
    //centerTransform.SetScale(zoneScale * 2.f, zoneScale * 2.f);//update of the scale of the sprite
    zoneSprite->Update();//let sprite update as well
    centerDespawn->Update();
    zoneCenterSprite->Update();
}

bool TimeStop::ToggleTime(AEVec2 position)//toggle the ability
{
    if (active == true)//if its active
    {
        active = false;//deactivate it
        despawning = true;
        centerDespawn->PlayAnimation("despawn",0,despawnTime);
        AudioManager::GetInstance()->PlaySFXOneShot("time_stop_start");
    }
    else if (active == false)//if not active
    {
        despawning = false;
        active = true;//activate
        SetZonePos(position);//set the position of the zone
        AudioManager::GetInstance()->PlaySFXOneShot("time_stop_end");
        zoneSprite->PlayAnimation("spawn", -1);
        zoneCenterSprite->PlayAnimation("spawn", -1);
    }
    return active;//return its activity
}

void TimeStop::TempStop(bool activity)
{
    tempStop = activity;
}

bool TimeStop::TimeStopped() const//getter
{
    return active;
}

f32 TimeStop::GetScale() const//getter
{
    return zoneScale;
}

void TimeStop::SetZonePos(AEVec2 position)//setter for the zones position
{
    zoneTransform.SetPosition(position);
    centerTransform.SetPosition(position);

}

void TimeStop::Draw()//Rendering the sprites
{
    //zone for timestop
    if (active && !despawning)
    {
        zoneSprite->Render(zoneSpriteTfm, 0.75f);
        zoneCenterSprite->Render(centerTransform);
    }
    if (despawning && !active)
    {
        centerDespawn->Render(centerTransform);
    }
}