#include "BossLaser.h"
#include "../System/MeshManager.h"
#include "../Enemy/BossEnemy.h"
#include "../System/CSVManager.h"
#include <iostream>

BossLaser::BossLaser()
	: meshManager{ MeshManager::GetInstance() },
    player{ Player::GetInstance() },
	active{ false },
    spriteName{ "bosslaser" }
{
    laserSprite = new Sprite(CSVManager::ReadInfo(spriteName));
    laserSprite->PlayAnimation("spawn", 1, 1.f);

    transform.SetScale(250, 1000);
}

BossLaser::BossLaser(float baseLifespan, float startupTime)
    : meshManager{ MeshManager::GetInstance() },
    player{ Player::GetInstance() },
    active{ false },
    spriteName{ "bosslaser" }
{
    this->baseLifespan = baseLifespan;
    this->startupDelayBase = startupTime;

    laserSprite = new Sprite(CSVManager::ReadInfo(spriteName));
    laserSprite->PlayAnimation("spawn", 1, 1.f);

    transform.SetScale(250, 1000);
}

BossLaser::~BossLaser()
{
    meshManager = nullptr;
    player = nullptr;
    if (laserSprite)
    {
        delete laserSprite;
        laserSprite = nullptr;
    }
}

void BossLaser::Update(float deltaTime)
{
    if (!active)
        return;

    laserSprite->Update();
    
    // charging phase
    if (charging)
    {
        std::cout << "CHARGING" << std::endl;
        startupDelay -= 1.f * deltaTime;
        if (startupDelay <= 0.f)
        {
            charging = false;
            laserSprite->PlayAnimation("attack", -1, 1.f);
        }
        return;
    }

    // attacking phase
    AEVec2 laserPos = transform.GetPosition();
    AEVec2 laserHalf = { transform.GetScale().x, transform.GetScale().y };

    AEVec2 playerPos = player->GetTransform().GetPosition();
    AEVec2 playerHalf = { 35.0f, 35.0f };

    if (AABBOverlap(laserPos, laserHalf, playerPos, playerHalf) && damageCooldown <= 0.f)
    {
        std::cout << "LASER TICK" << std::endl;

        player->TakeDamage(20);
        damageCooldown = baseCooldown;
    }

    if (damageCooldown > 0.f)
    {
        damageCooldown -= 1.f * deltaTime;
    }

    lifespan -= 1.f * deltaTime;
    if (lifespan <= 0.f)
    {
        active = false;
        charging = false;
        lifespan = baseLifespan;
    }
}
void BossLaser::Draw()
{
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    //AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    if (active)
    {
        //AEGfxSetTransparency(1.f);
        //AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
        //AEGfxSetColorToAdd(0.f, 0.f, 0.f, 0.f);
        //AEGfxSetTransform(transform.GetMatrix().m);
        //AEGfxMeshDraw(meshManager->GetMesh(SQUARE), AE_GFX_MDM_TRIANGLES);

        laserSprite->Render(transform, 1.f);
    }
}

bool BossLaser::Triggered(AEVec2 pos)
{
    if (active)//if its active
    {
        active = false;//deactivate it
        charging = false;
        lifespan = baseLifespan;
    }
    else //if not active
    {
        active = true;
        charging = true;
        startupDelay = startupDelayBase;
        transform.SetPosition(pos);

        laserSprite->PlayAnimation("spawn", 1, 1.f);
    }
    return active;//return its activity
}