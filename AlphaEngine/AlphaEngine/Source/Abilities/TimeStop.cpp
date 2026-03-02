#include "TimeStop.h"
#include <iostream>
#include "../utils.h"
#include "../System/CSVManager.h"

TimeStop::TimeStop()//constructor
    :meshManager{ MeshManager::GetInstance() }, //mesh manager
    player{ Player::GetInstance() }, //player instance
    active{ false }, //active boolean
    spriteName{ "timestop" }//spritename in sprite excelsheet
{
    zoneSprite = new Sprite(CSVManager::ReadInfo(spriteName));//create new sprite for the timestop zone
    zoneSprite->PlayAnimation("spawn", 1, 1.f);
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
}

void TimeStop::Update(std::vector<BaseEnemy*> enemyList,float dt)
{
    if (active)//if time stopped
    {
        zoneScale += 1500 * dt;//rapidly increase zone
        for (BaseProjectile* proj : player->GetProjectileList())//for every projectile the player throws
        {
            if (AreCirclesIntersecting(proj->transform.GetPosition().x, proj->transform.GetPosition().y, 0.1f ,
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
        }
    }
    
    else//else if not timestopped
    {
        for (BaseProjectile* proj : player->GetProjectileList())//every player thrown projectile
        {

            proj->stopped = false;//not stopped
            
        }
        for (BaseEnemy* enemy : enemyList)//every enemy thrown projectile
        {

            enemy->isStopped(false);//not stopped
            
        }
        zoneScale = originalScale;//revert the zonescale back to the original

    }
    zoneScale = AEClamp(zoneScale, originalScale, maxScale);//clamp the zone size
    zoneTransform.SetScale(zoneScale, zoneScale);//update the scale of the zone
    spriteTransform.SetScale(zoneScale * 2.f, zoneScale * 2.f);//update of the scale of the sprite
    zoneSprite->Update();//let sprite update as well
}

bool TimeStop::ToggleTime(AEVec2 position)//toggle the ability
{
    if (active == true)//if its active
    {
        active = false;//deactivate it
        zoneSprite->PlayAnimation("spawn", 1, 1.f);
    }
    else if (active == false)//if not active
    {
        active = true;//activate
        SetZonePos(position);//set the position of the zone
    }
    return active;//return its activity
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
    spriteTransform.SetPosition(position);
}

void TimeStop::Draw()//Rendering the sprites
{
    //zone for timestop
    AEGfxSetRenderMode(AE_GFX_RM_COLOR); 
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    if (active)
    {
        AEGfxSetTransparency(0.25f);
        AEGfxSetColorToMultiply(0.0f, 1.0f, 1.0f, 1.0f);
        AEGfxSetColorToAdd(0.f, 0.f, 255.f, 0.f);
        AEGfxSetTransform(zoneTransform.GetMatrix().m);
        AEGfxMeshDraw(meshManager->GetMesh(CIRCLE), AE_GFX_MDM_TRIANGLES);

        zoneSprite->Render(spriteTransform,0.75f);
    }
}