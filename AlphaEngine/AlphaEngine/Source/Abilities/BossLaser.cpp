/*!
@file       BossLaser.cpp
@author     Bay Yeok Hao (yeokhao.bay)
@brief      This file contains definitions of functions used by BossLaser
            which is an object created by BossEnemy when attacking
*//*_______________________________________________________________________*/

#include "BossLaser.h"
#include "../System/MeshManager.h"
#include "../Enemy/BossEnemy.h"
#include "../System/CSVManager.h"
#include <iostream>

// Constructor
BossLaser::BossLaser()
	: meshManager{ MeshManager::GetInstance() },
    player{ Player::GetInstance() },
	active{ false },
    spriteName{ "bosslaser" }
{
    laserSprite = new Sprite(CSVManager::ReadInfo(spriteName));
    laserSprite->PlayAnimation("spawn", 1, 1.f);
    AudioManager::GetInstance()->PlaySFXOneShot("boss_laser_spawning");
    transform.SetScale(250, 1000);
}

// Overload of constructor for custom parameters
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
    AudioManager::GetInstance()->PlaySFXOneShot("boss_laser_spawning");
    transform.SetScale(250, 1000);
}

// Destructor
BossLaser::~BossLaser()
{
    // Cleanup
    meshManager = nullptr;
    player = nullptr;
    if (laserSprite)
    {
        delete laserSprite;
        laserSprite = nullptr;
    }
}

// Update
void BossLaser::Update(float deltaTime)
{
    if (!active)
        return;

    // Run sprite update
    laserSprite->Update();
    
    // Charging phase
    if (charging)
    {
        startupDelay -= 1.f * deltaTime;
        if (startupDelay <= 0.f)
        {
            charging = false;
            laserSprite->PlayAnimation("attack", -1, 1.f);
            player->TriggerCameraShake(10.f, 4.f);
        }
        return;
    }

    // Attacking phase
    AEVec2 laserPos = transform.GetPosition();
    AEVec2 laserHalf = { transform.GetScale().x * 0.5f, transform.GetScale().y * 0.5f };

    AEVec2 playerPos = player->GetTransform().GetPosition();
    AEVec2 playerHalf = { player->GetTransform().GetScale().x * 0.5f, player->GetTransform().GetScale().y * 0.5f };

    // Damage player if they collided with bounding box of the laser
    if (AABBOverlap(laserPos, laserHalf, playerPos, playerHalf) && damageCooldown <= 0.f)
    {
        player->TakeDamage(20);
        damageCooldown = baseCooldown;
    }

    // Damage safeguard to ensure laser does not tick damage every frame
    if (damageCooldown > 0.f)
    {
        damageCooldown -= 1.f * deltaTime;
    }

    // Tick down object's lifespan
    lifespan -= 1.f * deltaTime;
    if (lifespan <= 0.f)
    {
        active = false;
        charging = false;
        lifespan = baseLifespan;
    }
}

// Render
void BossLaser::Draw()
{
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    if (active)
    {
        laserSprite->Render(transform, 1.f);
    }
}

// Upon activation (spawning) of ability by BossEnemy
bool BossLaser::Triggered(AEVec2 pos)
{
    // Toggle "active-ness"
    if (active)
    {
        active = false;
        charging = false;
        lifespan = baseLifespan;
    }
    else
    {
        active = true;
        charging = true;
        startupDelay = startupDelayBase;
        transform.SetPosition(pos);

        laserSprite->PlayAnimation("spawn", 1, 1.f);
    }
    return active;
}