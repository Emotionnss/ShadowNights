#include "BaseEnemy.h"
#include "../System/CSVManager.h"
#include "../Utils/utils.h"
#include "../System/Map.h"

/*!
@file       BaseEnemy.cpp
@author     Lim Chee Kang (l.cheekang)
@brief      This file contains definitions of functions and variables
             used for the BaseEnemy class, which is the parent class
             of all enemies in the game, and contains common variables and
			functions used across all enemy types.
*//*_______________________________________________________________________*/

BaseEnemy::BaseEnemy()
{
    isStatic = false;
    velocity = { 0.0f, 0.0f };
    sprite = nullptr;

    settingsManager = SettingsManager::GetInstance();
    map = Map::GetInstance();
}

BaseEnemy::~BaseEnemy()
{
    if (sprite) {
        delete sprite;
        sprite = nullptr;
    }

    for (EnemyProjectile* p : enemyProjectiles) {
        delete p;
    }
    enemyProjectiles.clear();

    settingsManager = nullptr;
    map = nullptr;
}

void BaseEnemy::Init(float x, float y, float w, float h, std::string spriteName)
{
    transform.SetPosition(x, y);
    transform.SetScale(w, h);

    if (sprite) delete sprite;
    sprite = new Sprite(CSVManager::ReadInfo(spriteName));

    originalSpeed = 100.f;
    moveSpeed = originalSpeed;
    velocity = { moveSpeed, 0.0f };
    isGrounded = false;

    isDying = false;
    oriDeathTimer = 1.0f;
    deathTimer = 0.0f;

    currentAnimState = "";
    origAnimSpeed = 1.f;
    animSpeed = origAnimSpeed;
}

void BaseEnemy::TakeDamage(int damage)
{
    health -= damage;

    if (health <= 0)
    {
        isDying = true;
        velocity = { 0.0f, 0.0f };
        deathTimer = oriDeathTimer;
        PlayAnim(deathAnimName);
    }
}

bool BaseEnemy::IsReadyToDelete() const
{
    return isDying && (deathTimer <= 0.0f);
}

void BaseEnemy::Update(float deltaTime)
{
    if (!stopped)
    {
        for (auto it = enemyProjectiles.begin(); it != enemyProjectiles.end(); )
        {
            (*it)->Speed(slowed ? (*it)->OriginalSpeed() / 20.f : (*it)->OriginalSpeed());
            (*it)->Update(deltaTime);

            if (CheckProjectileCollision(*it, map))
                (*it)->SetLifespan(-1.f);


            if ((*it)->IsDead())
            {
                delete* it;
                it = enemyProjectiles.erase(it);
            }
            else
            {
                ++it;
            }
        }

        if (isDying)
        {
            deathTimer -= deltaTime;
            if (sprite) sprite->Update();
        }
    }
}

void BaseEnemy::Draw()
{
    float alpha = 1.0f;

    if (isDying)
    {
        alpha = deathTimer / oriDeathTimer;
        if (alpha < 0.0f) alpha = 0.0f;
        if (alpha > 1.0f) alpha = 1.0f;
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
            AEVec2 camPos = {0.f, 0.f};
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
}

void BaseEnemy::PlayAnim(std::string name)
{
    if (currentAnimState != name)
    {
        currentAnimState = name;
        s32 loopCount = (name == deathAnimName) ? 1 : -1;
        if (sprite) sprite->PlayAnimation(name, loopCount, animSpeed);
    }
}

bool BaseEnemy::Slowed() const { return slowed; }
void BaseEnemy::Slowed(bool active)
{
    slowed = active;
    switch (slowed)
    {
    case true:
        animSpeed = origAnimSpeed * 10.f;
        moveSpeed = originalSpeed / 10.f;
        break;
    case false:
        moveSpeed = originalSpeed;
        animSpeed = origAnimSpeed;
        break;
    }
}

bool BaseEnemy::isStopped() const { return stopped; }
void BaseEnemy::isStopped(bool active) { stopped = active; }

void BaseEnemy::Shoot()
{
    float dirX = (transform.GetScale().x > 0) ? 1.0f : -1.0f;
    AEVec2 spawnPos = transform.GetPosition();
    spawnPos.y += 10.0f;
    EnemyProjectile* newProj = new EnemyProjectile(spawnPos, { dirX, 0.0f });
    newProj->slowed = this->slowed;
    enemyProjectiles.push_back(newProj);
}

bool BaseEnemy::HasLineOfSight(AEVec2 targetPos)
{
    AEVec2 startPos = transform.GetPosition();

    //Get the distance and direction to the target
    float dx = targetPos.x - startPos.x;
    float dy = targetPos.y - startPos.y;
    float distance = sqrtf(dx * dx + dy * dy);

    if (distance == 0.0f) return true;

    AEVec2 dir = { dx / distance, dy / distance };

    //check the map in increments of half-a-tile to make sure we don't skip over any thin walls
    float stepSize = map->GetTileSize() * 0.5f;
    int steps = static_cast<int>(distance / stepSize);

    for (int i = 0; i <= steps; ++i)
    {
        float currentX = startPos.x + dir.x * (stepSize * i);
        float currentY = startPos.y + dir.y * (stepSize * i);

        // Convert world position to grid coordinates
        AEVec2 tileCoord = map->GetTilePos(currentX, currentY);
        int tx = static_cast<int>(tileCoord.x);
        int ty = static_cast<int>(tileCoord.y);

        //inside the map boundaries
        if (tx >= 0 && tx < map->GetNumCol() && ty >= 0 && ty < map->GetNumRow())
        {
            Tile& tile = map->GetMap()[ty][tx];
            TILE_TYPE type = tile.tileType;

            // Check if the tile is a solid wall
            // We ignore backgrounds and platforms
            if (map->CheckTile(type) &&
                type != TILE_TYPE::BACKGROUND &&
                type != TILE_TYPE::LEFT_PLATFORM &&
                type != TILE_TYPE::PLATFORM &&
                type != TILE_TYPE::RIGHT_PLATFORM)
            {
                //wall is hit
                return false;
            }
        }
    }

    //if the loop finishes without hitting a wall the enemy can see the player
    return true;
}

bool BaseEnemy::CheckProjectileCollision(BaseProjectile* proj, Map* collisionMap)
{
    // 1. Changed map-> to collisionMap-> on these two lines
    for (s32 i = 0; i < collisionMap->GetNumCol(); ++i)
        for (s32 j = 0; j < collisionMap->GetNumRow(); ++j)
        {
            // 2. Added static_cast<f32> to i and j to fix the data loss warning
            Tile const& tmp = collisionMap->GetTileFromTilePos(static_cast<f32>(i), static_cast<f32>(j));

            AEVec2 projHalf = { proj->transform.GetScale().x * 0.5f, proj->transform.GetScale().y * 0.5f };
            AEVec2 tileHalf = { tmp.transform.GetScale().x * 0.5f, tmp.transform.GetScale().y * 0.5f };

            if (AABBOverlap(proj->transform.GetPosition(), projHalf, tmp.transform.GetPosition(), tileHalf) && proj->collidesWithTiles)
                // 3. Changed map-> to collisionMap-> here as well!
                return collisionMap->CheckTile(tmp.tileType);
        }
    return false;
}


void BaseEnemy::ResolveX(AEVec2& position)
{
    AEVec2 halfSize = { fabsf(transform.GetScale().x) * 0.3f, fabsf(transform.GetScale().y) * 0.5f };
    float tileSizeHalf = map->GetTileSize() * 0.5f;
    AEVec2 tileHalf = { tileSizeHalf, tileSizeHalf };

    AEVec2 minTile = map->GetTilePos(position.x - halfSize.x, position.y - halfSize.y);
    AEVec2 maxTile = map->GetTilePos(position.x + halfSize.x, position.y + halfSize.y);

    int xMin = static_cast<int>((std::min)(minTile.x, maxTile.x));
    int xMax = static_cast<int>((std::max)(minTile.x, maxTile.x));
    int yMin = static_cast<int>((std::min)(minTile.y, maxTile.y));
    int yMax = static_cast<int>((std::max)(minTile.y, maxTile.y));

    for (int y = yMin; y <= yMax; ++y) {
        for (int x = xMin; x <= xMax; ++x) {
            if (x < 0 || y < 0 || y >= map->GetNumRow() || x >= map->GetNumCol()) continue;

            Tile& tile = map->GetMap()[y][x];
            TILE_TYPE type = tile.tileType;
            if (!map->CheckTile(type)) continue;

            if (type == TILE_TYPE::BACKGROUND || type == TILE_TYPE::LEFT_PLATFORM ||
                type == TILE_TYPE::PLATFORM || type == TILE_TYPE::RIGHT_PLATFORM) continue;

            AEVec2 tilePos = tile.transform.GetPosition();
            float dx = position.x - tilePos.x;
            float px = (halfSize.x + tileHalf.x) - fabsf(dx);
            if (px <= 0.f) continue;

            float dy = position.y - tilePos.y;
            float py = (halfSize.y + tileHalf.y) - fabsf(dy);

            //Increased buffer to 15 pixels
            if (py <= 15.0f) continue;

            if ((dx > 0.f && velocity.x < 0.f) || (dx < 0.f && velocity.x > 0.f)) {
                position.x += (dx > 0.f) ? px : -px;
                velocity.x = -velocity.x;
                transform.SetScale(-transform.GetScale().x, transform.GetScale().y);
                return;
            }
        }
    }
}

void BaseEnemy::ResolveY(AEVec2& position)
{
    AEVec2 halfSize = { fabsf(transform.GetScale().x) * 0.3f, fabsf(transform.GetScale().y) * 0.5f };
    float tileSizeHalf = map->GetTileSize() * 0.5f;
    AEVec2 tileHalf = { tileSizeHalf, tileSizeHalf };

    isGrounded = false;
    AEVec2 minTile = map->GetTilePos(position.x - halfSize.x, position.y - halfSize.y);
    AEVec2 maxTile = map->GetTilePos(position.x + halfSize.x, position.y + halfSize.y);

    int xMin = static_cast<int>((std::min)(minTile.x, maxTile.x));
    int xMax = static_cast<int>((std::max)(minTile.x, maxTile.x));
    int yMin = static_cast<int>((std::min)(minTile.y, maxTile.y));
    int yMax = static_cast<int>((std::max)(minTile.y, maxTile.y));

    for (int y = yMin; y <= yMax; ++y) {
        for (int x = xMin; x <= xMax; ++x) {
            if (x < 0 || y < 0 || y >= map->GetNumRow() || x >= map->GetNumCol()) continue;

            Tile& tile = map->GetMap()[y][x];
            TILE_TYPE type = tile.tileType;
            if (!map->CheckTile(type) || type == TILE_TYPE::BACKGROUND) continue;

            AEVec2 tilePos = tile.transform.GetPosition();

            if (type == TILE_TYPE::LEFT_PLATFORM || type == TILE_TYPE::PLATFORM || type == TILE_TYPE::RIGHT_PLATFORM) {
                float surfaceY = tilePos.y + tileSizeHalf;
                float prevBottom = transform.GetPosition().y - halfSize.y;
                float newBottom = position.y - halfSize.y;

                float dx = position.x - tilePos.x;
                float px = (halfSize.x + tileHalf.x) - fabsf(dx);

                // Increased buffer to 15 pixels
                if (px <= 15.0f) continue;

                if (prevBottom >= surfaceY && velocity.y <= 0.f && newBottom <= surfaceY) {
                    position.y = surfaceY + halfSize.y;
                    velocity.y = 0.f;
                    isGrounded = true;
                }
                continue;
            }

            float dx = position.x - tilePos.x;
            float px = (halfSize.x + tileHalf.x) - fabsf(dx);

            // Increased buffer to 15 pixels
            if (px <= 15.0f) continue;

            float dy = position.y - tilePos.y;
            float py = (halfSize.y + tileHalf.y) - fabsf(dy);
            if (py <= 0.f) continue;

            if ((dy > 0.f && velocity.y < 0.f) || (dy < 0.f && velocity.y > 0.f)) {
                position.y += (dy > 0.f) ? py : -py;
                velocity.y = 0.f;
                if (dy > 0.f) isGrounded = true;
            }
        }
    }
}