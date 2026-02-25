#include "TimeStop.h"
#include <iostream>
#include "../utils.h"
#include "../System/CSVManager.h"

TimeStop::TimeStop()
    :meshManager{ MeshManager::GetInstance() }, player{ Player::GetInstance() }, active{ false }, spriteName{ "timestop" }
{
    zoneSprite = new Sprite(CSVManager::ReadInfo(SPRITESHEET_DATA, spriteName));
}

TimeStop::~TimeStop()
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
    if (active)
    {
        zoneScale += 1500 * dt;
        for (BaseProjectile* proj : player->GetProjectileList())
        {
            if (AreCirclesIntersecting(proj->transform.GetPosition().x, proj->transform.GetPosition().y, 0.1f ,
                zoneTransform.GetPosition().x, zoneTransform.GetPosition().y, GetScale()))
            {
                proj->stopped = true;
            }
        }
        for (BaseEnemy* enemy : enemyList)
        {
            if (AreCirclesIntersecting(enemy->transform.GetPosition().x, enemy->transform.GetPosition().y, 0.1f,
                zoneTransform.GetPosition().x, zoneTransform.GetPosition().y, GetScale()))
            {
                enemy->isStopped(true);
            }
        }
    }
    
    else
    {
        for (BaseProjectile* proj : player->GetProjectileList())
        {

            proj->stopped = false;
            
        }
        for (BaseEnemy* enemy : enemyList)
        {

            enemy->isStopped(false);
            
        }
        zoneScale = originalScale;

    }
    zoneScale = AEClamp(zoneScale, originalScale, maxScale);
    SetZoneScale();
    zoneSprite->Update();
}

bool TimeStop::ToggleTime(AEVec2 position)
{
    if (active == true)
    {
        active = false;

    }
    else if (active == false)
    {
        active = true;
        SetZonePos(position);
    }
    return active;
}

bool TimeStop::TimeStopped() const
{
    return active;
}

f32 TimeStop::GetScale() const
{
    return zoneScale;
}

void TimeStop::SetZonePos(AEVec2 position)
{
    zoneTransform.SetPosition(position);
    spriteTransform.SetPosition(position);
}

void TimeStop::SetZoneScale()
{
    zoneTransform.SetScale(zoneScale, zoneScale);
    spriteTransform.SetScale(zoneScale*2.f, zoneScale*2.f);

}

void TimeStop::Draw()
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