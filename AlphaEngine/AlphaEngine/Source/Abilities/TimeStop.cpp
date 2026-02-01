#include "TimeStop.h"
#include <iostream>

void TimeStop::Init()
{
	meshManager = MeshManager::GetInstance();
    player = Player::GetInstance();
    //meshManager->CreateCircle();
    //meshManager->CreateSquareMesh();
    //std::cout << "TImestop mesh manager address:" << &meshManager << "\n";
    //std::cout << "TImestop active address:" << &active << "\n";
    //std::cout << "TImestop trm address:" << &zoneTransform << "\n";
    active = false;
}

void TimeStop::Update(float dt)
{
    std::cout << resource << "\n";
    if (active)
    {
        resource -= 30 * dt;
    }
    else
    {
        resource += 30 * dt;
    }
    resource = AEClamp(resource, 0, maxResource);
    resourceBarTfm.SetScale(20, (resource / maxResource * 100.f));
    resourceBarTfm.SetPosition(player->transform.GetPosition().x + 450, player->transform.GetPosition().y - 200.f);
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
        SpawnZone(position);
    }
    std::cout << "ZA WORLDO" << std::endl;
}

bool TimeStop::TimeStopped()
{
    return active;
}

float TimeStop::GetScale()
{
    return zoneScale;
}

void TimeStop::SpawnZone(AEVec2 position)
{
    std::cout << "MADE\n";

    zoneTransform.SetScale(zoneScale, zoneScale);
    zoneTransform.SetPosition(position);
    
    
    active = true;
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
        AEGfxMeshDraw(meshManager->GetMesh(CIRCLE), AE_GFX_MDM_TRIANGLES);
    }

    //resource bar for timestop
    AEGfxSetTransparency(1.f);
    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
    AEGfxSetColorToAdd(1.f, 1.f, 255.f, 0.f);
    AEGfxSetTransform(resourceBarTfm.GetMatrix().m);
    AEGfxMeshDraw(meshManager->GetMesh(SQUARE), AE_GFX_MDM_TRIANGLES);
}

void TimeStop::Free()
{
    meshManager = nullptr;
    player = nullptr;
}