/*!
@file       BossEnemy.cpp
@author     Yap Yu Xuan Daevon (yuxuandaevon.yap), Bay Yeok Hao (yeokhao.bay)
@brief      This file contains definitions of functions used by BossEnemy for
            attacking the player. Randomly uses 3 abilities (BossLaser, 
            BossExplosive, SpawnRandomEnemy)
*//*_______________________________________________________________________*/


#include "BossEnemy.h"
#include "../Player/Player.h"
#include "../System/MeshManager.h"

#include "ArcherEnemy.h"
#include "BatEnemy.h"
#include "ShadowEnemy.h"

#include <cmath>
#include <algorithm>

BossEnemy::BossEnemy() : BaseEnemy()
{
}

BossEnemy::~BossEnemy()
{
    AEGfxTextureUnload(weakpoint.tex);

    // Cleanup Laser, Explosive, BaseEnemy resources
    for (BossLaser* l : bossLasers)
        delete l;
    bossLasers.clear();

    for (BossExplosive* e : bossExplosives)
        delete e;
    bossExplosives.clear();

    for (BaseEnemy* e : pendingSpawns)
        delete e;
    pendingSpawns.clear();
}

void BossEnemy::Init(f32 x, f32 y, f32 w, f32 h, std::string spriteName)
{
    BaseEnemy::Init(x, y, w, h, spriteName);

	//override base enemy stats
    health = 200;
    maxHealth = 200;

    deathAnimName = "death";

    gravity = 0.0f;
    moveSpeed = 100.0f;

    // Make the AI targets match the physical Hard Clamp
    float bossHalfWidth = 200.0f;
    float bossHalfHeight = 200.0f;
    float tileSize = map->GetTileSize();

    // The CSV has 7 empty tiles on the left before the brick wall starts!
    float leftWallBuffer = tileSize * 7.0f;
    float ceilingBuffer = tileSize * 2.0f;

    minCoordLimit = { leftWallBuffer + bossHalfWidth, tileSize + bossHalfHeight };
    maxCoordLimit = { (map->GetNumCol() * tileSize) - tileSize - bossHalfWidth,
                      (map->GetNumRow() * tileSize) - ceilingBuffer - bossHalfHeight };

    //variables
    isAttacking = false;
    attackTimer = 0.0f;
    hasFired = false;

    isAggroed = false;
    isSurprised = false;
    surpriseTimer = 0.0f;

    transform.SetScale(400.f, 400.f);
    
    weakpoint.tex = AEGfxTextureLoad("Assets/Sprites/pearl_01c.png");
    weakpoint.transform.SetScale(100.f, 100.f);

    isCasting = false;
    castProgress = 0.f;
    castDuration = 3.f;
    pendingAttack = PendingAttack::NONE;

    attackCooldown = 3.f;
}

void BossEnemy::Update(f32 deltaTime)
{
    // Save position to ignore inner wall collisions
    AEVec2 ghostPos = transform.GetPosition();

    // BaseEnemy handles Projectiles and Death animations
    BaseEnemy::Update(deltaTime);
    
    // If we are stopped or dying dont run AI or movement
    if (stopped || isDying) return;

    // Restore position so we float through inner blocks
    transform.SetPosition(ghostPos.x, ghostPos.y);

    // (This calculates the movement)
    EnemyFSM(deltaTime);

    // FORCE the boss to stay inside the outer map boundaries

    float bossHalfWidth = fabs(transform.GetScale().x) * 0.5f;
    float bossHalfHeight = fabs(transform.GetScale().y) * 0.5f;
    float tileSize = map->GetTileSize();

	// add the buffer to prevent the boss from clipping into the ceiling and left wall since those are the only two sides we care about for now
    float ceilingBuffer = tileSize * 2.0f;
    float leftWallBuffer = tileSize * 7.0f;

    float minX = leftWallBuffer + bossHalfWidth;
    float maxX = (map->GetNumCol() * tileSize) - tileSize - bossHalfWidth;

    float minY = tileSize + bossHalfHeight;
    float maxY = (map->GetNumRow() * tileSize) - ceilingBuffer - bossHalfHeight;

    AEVec2 pos = transform.GetPosition();

    if (pos.x < minX) pos.x = minX;
    if (pos.x > maxX) pos.x = maxX;
    if (pos.y < minY) pos.y = minY;
    if (pos.y > maxY) pos.y = maxY;

    transform.SetPosition(pos.x, pos.y);

    Player* player = Player::GetInstance();
    AEVec2 myPos = transform.GetPosition();
    AEVec2 playerPos = player->GetTransform().GetPosition();

    //Set direction of sprite to face player
    float dx = playerPos.x - myPos.x;

    float visualWidth = fabs(transform.GetScale().x);
    float visualHeight = transform.GetScale().y;

    if (dx > 0) transform.SetScale(visualWidth, visualHeight);
    else        transform.SetScale(-visualWidth, visualHeight);

    if (sprite) sprite->Update();

    // Handle randomized ability queueing
    if (isCasting)
    {
        castProgress += deltaTime;

        if (castProgress >= castDuration)
        {
            castProgress = 0.f;
            isCasting = false;

            if (pendingAttack == PendingAttack::LASER)
            {
                FireLaser();
            }
            else if (pendingAttack == PendingAttack::EXPLOSIVE)
            {
                FireExplosive();
            }
            else if (pendingAttack == PendingAttack::SPAWN_RANDOM_ENEMY)
            {
                SpawnRandomEnemy();
            }
            pendingAttack = PendingAttack::NONE;
        }
    }

    // Handle casting bar for pending attack
    attackCooldown -= deltaTime;
    if (attackCooldown <= 0.f)
    {
        int choice = rand() % 3;
        if (choice == 0)
        {
            StartCast(PendingAttack::LASER);
        }
        else if (choice == 1)
        {
            StartCast(PendingAttack::EXPLOSIVE);
        }
        else
        {
            StartCast(PendingAttack::SPAWN_RANDOM_ENEMY);
        }
        attackCooldown = 3.f;
    }

    // Update vector containing Lasers and Explosives
    for (BossLaser* l : bossLasers)
    {
        l->Update(deltaTime);
    }
    for (BossExplosive* e : bossExplosives)
    {
        e->Update(deltaTime);
    }
}

void BossEnemy::Draw()
{
    //BaseEnemy::Draw();

    float alpha = 1.0f;

    if (isDying)
    {
        alpha = deathTimer / oriDeathTimer;
        if (alpha < 0.0f) alpha = 0.0f;
        if (alpha > 1.0f) alpha = 1.0f;
    }

    if (weakpoint.active)
    {
        alpha = 0.5f;
    }
    else
    {
        alpha = 1.f;
    }

    if (sprite) sprite->Render(transform, alpha);

    if (!isDying)
    {
        // Safely wipe the paintbrush
        AEGfxTextureSet(nullptr, 0, 0);

        AEGfxSetRenderMode(AE_GFX_RM_COLOR);
        AEGfxSetColorToMultiply(1.0f, 0.0f, 0.0f, 1.0f);
        AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
        AEGfxSetBlendMode(AE_GFX_BM_BLEND);
        AEGfxSetTransparency(1.0f);

        if (!isBoss)
        {
            healthbarTransform = transform;

            healthbarTransform.SetPosition(
                transform.GetPosition().x,
                transform.GetPosition().y + (0.5f * transform.GetScale().y) + 15.f
            );
            healthbarTransform.SetScale((static_cast<float>(health) / static_cast<float>(maxHealth)) * 100.f, 10.f);
        }
        else
        {
            AEVec2 camPos = { 0.f, 0.f };
            AEGfxGetCamPosition(&camPos.x, &camPos.y);

            healthbarTransform.SetPosition(
                camPos.x,
                camPos.y - 300.f
            );
            healthbarTransform.SetScale((static_cast<float>(health) / static_cast<float>(maxHealth)) * 1000.f, 50.f);
        }

        AEGfxSetTransform(healthbarTransform.GetMatrix().m);
        AEGfxMeshDraw(MeshManager::GetInstance()->GetMesh(SQUARE), AE_GFX_MDM_TRIANGLES);
        AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
    }

    for (EnemyProjectile* p : enemyProjectiles)
    {
        p->Draw(nullptr);
    }

    if (path.size() > 1)
    {
        if (settingsManager->GetTilesDebug())
        {
            map->RenderCheckedTilesDebug();
        }

        if (settingsManager->GetPathDebug())
        {
            map->RenderPathDebug(path);
        }
    }



    AEGfxTextureSet(nullptr, 0, 0);
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    AEGfxSetTransparency(1.0f);
    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
    AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

    // This specifically stops the Giant Red Bars!
    AEMtx33 identity;
    AEMtx33Identity(&identity);
    AEGfxSetTransform(identity.m);

    if (weakpoint.active)
    {
        AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

        AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
        AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

        AEGfxSetBlendMode(AE_GFX_BM_BLEND);
        AEGfxSetTransparency(1.0f);

        AEGfxTextureSet(weakpoint.tex, 0, 0);
        AEGfxSetTransform(weakpoint.transform.GetMatrix().m);
        AEGfxMeshDraw(MeshManager::GetInstance()->GetMesh(TEXTURE_SQUARE), AE_GFX_MDM_TRIANGLES);
    }

    // Render cast bar if ability cast in progress
    if (isCasting)
    {
        castbarTransform = transform;
        castbarTransform.SetPosition(
            transform.GetPosition().x - 100.f,
            transform.GetPosition().y + (0.5f * transform.GetScale().y) + 15.f
        );
        castbarTransform.SetScale((castProgress / castDuration) * 200.f, 10.f);

        AEGfxSetColorToMultiply(1.0f, 1.0f, 0.0f, 1.0f);
        AEGfxSetTransform(castbarTransform.GetMatrix().m);
        AEGfxMeshDraw(MeshManager::GetInstance()->GetMesh(CORNER_SQUARE), AE_GFX_MDM_TRIANGLES);
    }

    // Render all active Lasers and Explosives
    for (BossLaser* l : bossLasers)
        l->Draw();
    for (BossExplosive* e : bossExplosives)
        e->Draw();

}

void BossEnemy::EnemyFSM(f32 deltaTime)
{
    switch (currentBossState)
    {
    case BOSS_STATE::SPAWNING:
        if (UpdateTimer(deltaTime))
        {
            EnterIdling();

            //One time call to play idle animation
            sprite->PlayAnimation("idle", -1, 1.5f);
        }
        break;
    case BOSS_STATE::IDLING:
        if (UpdateTimer(deltaTime))
        {
            EnterMoving();
        }
        break;
    case BOSS_STATE::MOVING:
        if (UpdateMoving(deltaTime))
        {
            EnterIdling();
        }
        break;
    case BOSS_STATE::EXHAUSTED:
        UpdateWeakpoint(deltaTime);
        if (UpdateTimer(deltaTime))
        {
            EnterMoving();
            weakpoint.active = false;
        }
        break;
    }

    if (stateCounter == maxStateCounter)
    {
        EnterExhausted();
    }

    //EnterExhausted();

    //HP Check
    /*if (health <= 0)
    {
        EnterDeath();
    }*/
}

void BossEnemy::EnterIdling()
{
    currentBossState = BOSS_STATE::IDLING;
    timer = idleTimer;
    ++stateCounter;
}

void BossEnemy::EnterMoving()
{
    //Set new state
    currentBossState = BOSS_STATE::MOVING;

    //Set targetPos for moving towards
    targetPos = { AERandFloat() * (maxCoordLimit.x - minCoordLimit.x) + minCoordLimit.x, AERandFloat() * (maxCoordLimit.y - minCoordLimit.y) + minCoordLimit.y };

    //Get vector from target to current pos;
    AEVec2 currentPos = transform.GetPosition();
    f32 dx = targetPos.x - currentPos.x;
    f32 dy = targetPos.y - currentPos.y;
    f32 dist = sqrt(dx * dx + dy * dy);

    f32 dirX = dx / dist;
    f32 dirY = dy / dist;

    velocity = { dirX * moveSpeed, dirY * moveSpeed };
    ++stateCounter;
}

bool BossEnemy::UpdateMoving(f32 deltaTime)
{
    //Update boss to move towards waypoint
    AEVec2 position = transform.GetPosition();
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;

	transform.SetPosition(position.x, position.y);

    //Check for whether boss reached near position
    if (position.x > targetPos.x - 5.0f && position.x < targetPos.x + 5.0f && position.y > targetPos.y - 5.0f && position.y < targetPos.y + 5.0f)
    {
        return true;
    }

    return false;
}

void BossEnemy::EnterExhausted()
{
    currentBossState = BOSS_STATE::EXHAUSTED;

    // so the Boss doesn't drift away
    velocity = { 0.0f, 0.0f };

    // Turn the Weakpoint ON
    weakpoint.active = true;

    timer = exhaustedTimer;
    stateCounter = 0;
}

void BossEnemy::EnterDeath()
{
    currentBossState = BOSS_STATE::DEATH;
    timer = deathTimer;
    sprite->PlayAnimation("death", 1, deathTimer);
}

bool BossEnemy::UpdateTimer(f32 deltaTime)
{
    timer -= deltaTime;

    //Signal to trigger changes
    if (timer <= 0)
    {
        return true;
    }

    return false;
}

void BossEnemy::ActivateWeakpoint()
{
    weakpoint.active = true;
    weakpoint.dir = 1;
    AEVec2 currentPos = transform.GetPosition();
    weakpoint.transform.SetPosition(currentPos);
    weakpoint.maxY = currentPos.y + 5.f;
    weakpoint.minY = currentPos.y - 5.f;
    weakpoint.lerpVector = { currentPos.x, weakpoint.maxY + 2.f };
}

void BossEnemy::UpdateWeakpoint(f32)
{
    // Get the Boss's exact current position
    AEVec2 bossPos = transform.GetPosition();

    // Use a sine wave to create a smooth, floating bobbing effect
    float bobbingOffset = sinf(timer * 5.0f) * 15.0f;

    // Pin the weakpoint to the Boss's chest + the bobbing offset
    weakpoint.transform.SetPosition(bossPos.x, bossPos.y + bobbingOffset);
}

Weakpoint BossEnemy::GetWeakpoint() const
{
    return weakpoint;
}

// Helper function to queue the next pending boss ability
void BossEnemy::StartCast(PendingAttack attackType)
{
    isCasting = true;
    castProgress = 0.0f;
    pendingAttack = attackType;

    if (attackType == PendingAttack::LASER) castDuration = 1.5f;
    else if (attackType == PendingAttack::EXPLOSIVE) castDuration = 3.f;
    else if (attackType == PendingAttack::SPAWN_RANDOM_ENEMY) castDuration = 2.f;
}

// Laser ability activation
void BossEnemy::FireLaser()
{
    Player* player = Player::GetInstance();
    BossLaser* newLaser = new BossLaser();
    newLaser->Triggered({ player->GetTransform().GetPosition().x, 500.f });
    bossLasers.push_back(newLaser);
}

// Explosive ability activation
void BossEnemy::FireExplosive()
{
    BossExplosive* newExplosive = new BossExplosive();
    float dirX = transform.GetScale().x > 0.f ? 1.f : -1.f;
    AEVec2 dir = { dirX, AERandFloat() * 2.f - 1.f };
    newExplosive->Triggered(transform.GetPosition(), dir);
    bossExplosives.push_back(newExplosive);
}

// Getter to return the random enemy to be spawned in the scene
std::vector<BaseEnemy*>& BossEnemy::GetPendingSpawns()
{
    return pendingSpawns;
}

// Random enemy spawn(Sniper, Bat, Shadow) ability activation
void BossEnemy::SpawnRandomEnemy()
{
    BaseEnemy* randEnemy = nullptr;

	//the boss position is a bit too high for the random enemy to spawn at, so we will spawn it slightly below the boss's current position
    float spawnX = transform.GetPosition().x;
    float spawnY = transform.GetPosition().y - 100.0f;

	// there are some map boundaries we need to respect to prevent the random enemy from spawning inside walls, so we will calculate those first and adjust the spawn coordinates if necessary
    float tileSize = map->GetTileSize();
    float leftWallBuffer = tileSize * 7.0f;
    float ceilingBuffer = tileSize * 2.0f;
    float safeBuffer = tileSize * 2.0f;

	// absolute minimum and maximum coordinates that the random enemy can spawn at, taking into account the boss's position and the map boundaries
    float minX = leftWallBuffer + safeBuffer;
    float maxX = (map->GetNumCol() * tileSize) - safeBuffer;
    float minY = safeBuffer;
    float maxY = (map->GetNumRow() * tileSize) - ceilingBuffer - safeBuffer;

	// constrain the spawn coordinates to be within the safe boundaries
    if (spawnX < minX) spawnX = minX;
    if (spawnX > maxX) spawnX = maxX;

    if (spawnY < minY) spawnY = minY;
    if (spawnY > maxY) spawnY = maxY;

    int choice = rand() % 3;
    if (choice == 0)
    {
        randEnemy = new ArcherEnemy();
        randEnemy->Init(spawnX, spawnY, 85.f, 60.f, "sniperenemy");
    }
    else if (choice == 1)
    {
        randEnemy = new BatEnemy();
        randEnemy->Init(spawnX, spawnY, 85.f, 60.f, "batenemy");
    }
    else
    {
        randEnemy = new ShadowEnemy();
        randEnemy->Init(spawnX, spawnY, 85.f, 60.f, "shadow1");
    }

    if (randEnemy)
    {
        randEnemy->SetVelocity(0, 0);
        pendingSpawns.push_back(randEnemy);
    }
}