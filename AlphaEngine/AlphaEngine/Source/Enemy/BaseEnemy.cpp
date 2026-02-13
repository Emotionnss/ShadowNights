#include "BaseEnemy.h"
#include "../System/CSVManager.h"
#include "../System/Map.h"
#include "../Player/Player.h"
#include <algorithm> 
#include <iostream>  
#include <cmath>     

BaseEnemy::BaseEnemy()
{
    isStatic = false;
    velocity = { 0.0f, 0.0f };
    sprite = nullptr;
}

BaseEnemy::~BaseEnemy()
{
    if (sprite) {
        delete sprite;
        sprite = nullptr;
    }

    // [NEW] Clean up all active projectiles when enemy dies
    for (EnemyProjectile* p : enemyProjectiles) {
        delete p;
    }
    enemyProjectiles.clear();
}

void BaseEnemy::Init(float x, float y, float w, float h, std::string spriteName)
{
    transform.SetPosition(x, y);
    transform.SetScale(w, h);

    if (sprite) delete sprite;
    sprite = new Sprite(CSVManager::ReadInfo(SPRITESHEET_DATA, spriteName));

    // Reset Defaults
    moveSpeed = 100.0f;
    velocity = { moveSpeed, 0.0f };
    isGrounded = false;

    isAttacking = false;
    attackTimer = 0.0f;
    hasFired = false; // Reset firing flag

    // Reset Death State
    isDying = false;
    deathTimer = 0.0f;

    currentAnimState = "";
    PlayAnim("walkright");
}

void BaseEnemy::TakeDamage()
{
    if (!isDying)
    {
        isDying = true;
        velocity = { 0.0f, 0.0f }; // Freeze!
        deathTimer = 1.0f;         // Duration of death anim
        PlayAnim("shadowdeath");
    }
}

bool BaseEnemy::IsReadyToDelete()
{
    return isDying && (deathTimer <= 0.0f);
}

bool BaseEnemy::isStopped()
{
    return stopped;
}

// [NEW] Shoot Function
void BaseEnemy::Shoot()
{
    // Determine direction based on visual scale (Face Right vs Left)
    float dir = transform.GetScale().x;

    // Spawn projectile at Enemy's current position
    // Note: You might want to offset Y slightly so it comes from the "hand" not the feet
    AEVec2 spawnPos = transform.GetPosition();
    spawnPos.y += 10.0f;

    EnemyProjectile* newProj = new EnemyProjectile(spawnPos, dir);
    enemyProjectiles.push_back(newProj);
}

void BaseEnemy::Update(float deltaTime)
{
    if (!stopped)
    {
        // ------------------------------------------
        // 1. UPDATE PROJECTILES (Always do this!)
        // ------------------------------------------
        for (int i = 0; i < enemyProjectiles.size(); i++)
        {
            enemyProjectiles[i]->Update(deltaTime);

            // Remove if expired
            if (enemyProjectiles[i]->IsDead())
            {
                delete enemyProjectiles[i];
                enemyProjectiles.erase(enemyProjectiles.begin() + i);
                i--;
            }
        }

        // ------------------------------------------
        // 2. DEATH STATE (Highest Priority)
        // ------------------------------------------
        if (isDying)
        {
            deathTimer -= deltaTime;
            if (sprite) sprite->Update();
            return; // Stop here
        }

        // ------------------------------------------
        // 3. CONFIG & DATA
        // ------------------------------------------
        const float gravity = -1400.f;
        float colliderWidth = 40.0f;
        float colliderHeight = 60.0f;

        Map* map = Map::GetInstance();
        Player* player = Player::GetInstance();

        AEVec2 myPos = transform.GetPosition();
        AEVec2 playerPos = player->GetTransform().GetPosition();

        float dx = playerPos.x - myPos.x;
        float dy = playerPos.y - myPos.y;
        float dist = sqrt(dx * dx + dy * dy);

        Node node;
        if (map->GeneratePath(transform.GetPosition(), player->transform.GetPosition(), { 0, 0 }, { 100, 100 }, node))
        {
            path = map->BuildPath(path, node);
            map->RenderPathDebug(path);
        }

        // ------------------------------------------
        // 4. STATE MACHINE (Attack vs Patrol)
        // ------------------------------------------
        float visualWidth = fabs(transform.GetScale().x);
        float visualHeight = transform.GetScale().y;

        if (isAttacking)
        {
            velocity.x = 0.0f;
            attackTimer -= deltaTime;

            // Face the player
            if (dx > 0) transform.SetScale(visualWidth, visualHeight);
            else        transform.SetScale(-visualWidth, visualHeight);

            // [NEW] SHOOT LOGIC
            if (!hasFired)
            {
                Shoot();
                hasFired = true; // Lock: Only shoot once per attack cycle
            }

            if (attackTimer <= 0.0f)
            {
                isAttacking = false;
                hasFired = false; // Reset for next time
                PlayAnim("walkright");
            }
        }
        else
        {
            if (dist < detectionRadius)
            {
                isAttacking = true;
                attackTimer = 1.0f; // Attack lasts 1 second
                PlayAnim("attack1");
            }
            else
            {
                velocity.x = moveSpeed;
                PlayAnim("walkright");

                if (moveSpeed > 0) transform.SetScale(visualWidth, visualHeight);
                else               transform.SetScale(-visualWidth, visualHeight);
            }
        }

        // ------------------------------------------
        // 5. PHYSICS & COLLISION
        // ------------------------------------------
        velocity.y += gravity * deltaTime;

        AEVec2 position = transform.GetPosition();
        position.x += velocity.x * deltaTime;
        position.y += velocity.y * deltaTime;

        AEVec2 halfSize{ colliderWidth / 2.f, colliderHeight / 2.f };

        // Standard Map Collision Logic
        float left = position.x - halfSize.x;
        float right = position.x + halfSize.x;
        float bottom = position.y - halfSize.y;
        float top = position.y + halfSize.y;

        AEVec2 minTile = map->GetTilePos(left, bottom);
        AEVec2 maxTile = map->GetTilePos(right, top);

        int xMinTile = static_cast<int>((std::min)(minTile.x, maxTile.x));
        int xMaxTile = static_cast<int>((std::max)(minTile.x, maxTile.x));
        int yMinTile = static_cast<int>((std::min)(minTile.y, maxTile.y));
        int yMaxTile = static_cast<int>((std::max)(minTile.y, maxTile.y));

        isGrounded = false;
        bool hitWall = false;

        for (int y = yMinTile; y <= yMaxTile; ++y)
        {
            for (int x = xMinTile; x <= xMaxTile; ++x)
            {
                if (x < 0 || y < 0 || y >= map->GetNumRow() || x >= map->GetNumCol()) continue;

                Tile& tile = map->GetMap()[y][x];
                if (!map->CheckTile(tile.tileType)) continue;

                AEVec2 tilePosition = tile.transform.GetPosition();
                AEVec2 tileHalf = { map->GetTileSize() * 0.5f, map->GetTileSize() * 0.5f };

                float colDx = position.x - tilePosition.x;
                float px = (halfSize.x + tileHalf.x) - fabs(colDx);

                if (px > 0)
                {
                    float colDy = position.y - tilePosition.y;
                    float py = (halfSize.y + tileHalf.y) - fabs(colDy);

                    if (py > 0)
                    {
                        if (px < py)
                        {
                            if (colDx > 0) position.x += px;
                            else           position.x -= px;

                            velocity.x = 0.f;
                            hitWall = true;
                        }
                        else
                        {
                            if (colDy > 0) position.y += py;
                            else           position.y -= py;

                            velocity.y = 0.f;
                            if (colDy > 0 && velocity.y <= 0.f) isGrounded = true;
                        }
                    }
                }
            }
        }

        if (hitWall && !isAttacking)
        {
            moveSpeed *= -1.0f;
        }

        if (position.y <= -400.f)
        {
            position.y = -400.f;
            velocity.y = 0.f;
            isGrounded = true;
        }

        transform.SetPosition(position.x, position.y);

        if (sprite) sprite->Update();
    }
}

void BaseEnemy::Draw()
{
    if (sprite) sprite->Render(transform);

    // [NEW] Draw Projectiles
    for (EnemyProjectile* p : enemyProjectiles)
    {
        p->Draw();
    }
}

void BaseEnemy::PlayAnim(std::string name)
{
    if (currentAnimState != name)
    {
        currentAnimState = name;
        if (sprite) sprite->PlayAnimation(name, -1, 1.0f);
    }
}