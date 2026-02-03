#include "TimeStop.h"
#include <iostream>
#include "../utils.h"
#include "../System/CSVManager.h"
void TimeStop::Init()
{
	meshManager = MeshManager::GetInstance();
    player = Player::GetInstance();
    active = false;
    sprite = new Sprite(CSVManager::ReadInfo(SPRITESHEET_DATA, "timestop"));
    sprite->PlayAnimation("spawn", 1, 1.f);
}

void TimeStop::Update(float dt)
{
    sprite->Update();
    if (active)
    {
        timeStopped += 1 * dt;
        resource -= 30 * dt;
        zoneScale += 1500 * dt;
        for (BaseProjectile* proj : player->GetProjectileList())
        {
            if (AreCirclesIntersecting(proj->transform.GetPosition().x, proj->transform.GetPosition().y, proj->size,
                zoneTransform.GetPosition().x, zoneTransform.GetPosition().y, GetScale()))
            {
                proj->stopped = true;
            }
        }
    }
    else
    {
        for (BaseProjectile* proj : player->GetProjectileList())
        {
            if (AreCirclesIntersecting(proj->transform.GetPosition().x, proj->transform.GetPosition().y, proj->size,
                zoneTransform.GetPosition().x, zoneTransform.GetPosition().y, GetScale()))
            {
                proj->stopped = false;
            }
        }
        resource += 100 * dt;
        zoneScale = originalScale;

    }
    zoneScale = AEClamp(zoneScale, originalScale, maxScale);
    SetZoneScale();
    f32 camX{}, camY{};
    AEGfxGetCamPosition(&camX, &camY);
    resource = AEClamp(resource, 0, maxResource);
    resourceBarTfm.SetScale(20, (resource / maxResource * 100.f));
    resourceBarTfm.SetPosition(camX + 450.f, camY - 200.f);
    if (resource <= 0)
    {
        active = false;
    }

}

void TimeStop::ToggleTime(AEVec2 position)
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
    std::cout << "ZA WORLDO" << std::endl;
}

bool TimeStop::TimeStopped()
{
    return active;
}

f32 TimeStop::GetScale()
{
    return zoneScale;
}

void TimeStop::SetZonePos(AEVec2 position)
{
    zoneTransform.SetPosition(position);
}

void TimeStop::SetZoneScale()
{
    zoneTransform.SetScale(zoneScale, zoneScale);
}

void TimeStop::Draw()
{
    //zone for timestop
    AEGfxSetRenderMode(AE_GFX_RM_COLOR); 
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetTransparency(0.25f);
    AEGfxSetColorToMultiply(0.0f, 1.0f, 1.0f, 1.0f);
    AEGfxSetColorToAdd(0.f, 0.f, 255.f, 0.f);
    AEGfxSetTransform(zoneTransform.GetMatrix().m);
    if (active)
    {
        sprite->Render(zoneTransform);
    }
    //if (active)
    //{
    //    AEGfxMeshDraw(meshManager->GetMesh(TEXTURE_SQUARE), AE_GFX_MDM_TRIANGLES);
    //}

    //resource bar for timestop
    AEGfxSetTransparency(1.f);
    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
    AEGfxSetColorToAdd(1.f, 1.f, 255.f, 0.f);
    AEGfxSetTransform(resourceBarTfm.GetMatrix().m);
    AEGfxMeshDraw(meshManager->GetMesh(CORNER_SQUARE), AE_GFX_MDM_TRIANGLES);
}

void TimeStop::Free()
{
    meshManager = nullptr;
    player = nullptr;
}