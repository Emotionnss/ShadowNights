#include "BossLaser.h"
#include "../System/MeshManager.h"
#include "../Enemy/BossEnemy.h"
#include "../System/CSVManager.h"
#include <iostream>

BossLaser::BossLaser()
	: meshManager{ MeshManager::GetInstance() },
	active{ false },
    spriteName{ "bosslaser" }
{
    laserSprite = new Sprite(CSVManager::ReadInfo(SPRITESHEET_DATA, spriteName));
    laserSprite->PlayAnimation("spawn", 1, 1.f);

    transform.SetScale(500, 500);
}

BossLaser::~BossLaser()
{
    meshManager = nullptr;
    if (laserSprite)
    {
        delete laserSprite;
        laserSprite = nullptr;
    }
}

void BossLaser::Update(float deltaTime)
{
    laserSprite->Update();
}
void BossLaser::Draw()
{
    //zone for timestop
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    if (active)
    {
        AEGfxSetTransparency(1.f);
        AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
        AEGfxSetColorToAdd(0.f, 0.f, 0.f, 0.f);
        AEGfxSetTransform(transform.GetMatrix().m);
        AEGfxMeshDraw(meshManager->GetMesh(SQUARE), AE_GFX_MDM_TRIANGLES);

        laserSprite->Render(transform, 1.f);
    }
}

bool BossLaser::Triggered(AEVec2 pos)
{
    if (active == true)//if its active
    {
        active = false;//deactivate it
    }
    else if (active == false)//if not active
    {
        active = true;//activate
        transform.SetPosition(pos);
        laserSprite->PlayAnimation("spawn", 1, 1.f);
    }
    return active;//return its activity
}