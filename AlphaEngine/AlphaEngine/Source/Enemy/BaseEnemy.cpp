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

    // Reset Death State
    isDying = false;
    deathTimer = 0.0f;

    currentAnimState = "";
    PlayAnim("walkright");
}

// [NEW] Trigger Death
void BaseEnemy::TakeDamage()
{
    if (!isDying)
    {
        isDying = true;
        velocity = { 0.0f, 0.0f }; // Freeze!
        deathTimer = 1.0f;         // Time to play "Death" animation
        PlayAnim("shadowdeath");         // Make sure CSV has "Death" row
    }
}

// [NEW] Helper for TestScene
bool BaseEnemy::IsReadyToDelete()
{
    return isDying && (deathTimer <= 0.0f);
}

void BaseEnemy::Update(float deltaTime)
{
    // ------------------------------------------
    // 0. DEATH STATE (HIGHEST PRIORITY)
    // ------------------------------------------
    if (isDying)
    {
        deathTimer -= deltaTime;
        // Continue playing animation
        if (sprite) sprite->Update();

        // Skip all movement and collision logic below!
        return;
    }

    // ------------------------------------------
    // 1. CONFIG & DATA
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

    // ------------------------------------------
    // 2. STATE MACHINE (Attack vs Patrol)
    // ------------------------------------------

    float visualWidth = fabs(transform.GetScale().x);
    float visualHeight = transform.GetScale().y;

    if (isAttacking)
    {
        velocity.x = 0.0f;
        attackTimer -= deltaTime;

        if (dx > 0) transform.SetScale(visualWidth, visualHeight);
        else        transform.SetScale(-visualWidth, visualHeight);

        if (attackTimer <= 0.0f)
        {
            isAttacking = false;
            PlayAnim("walkright");
        }
    }
    else
    {
        if (dist < detectionRadius)
        {
            isAttacking = true;
            attackTimer = 1.0f;
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
    // 3. PHYSICS & COLLISION
    // ------------------------------------------
    velocity.y += gravity * deltaTime;

    AEVec2 position = transform.GetPosition();
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;

    AEVec2 halfSize{ colliderWidth / 2.f, colliderHeight / 2.f };

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

void BaseEnemy::Draw()
{
    if (sprite) sprite->Render(transform);
}

void BaseEnemy::PlayAnim(std::string name)
{
    if (currentAnimState != name)
    {
        currentAnimState = name;
        if (sprite) sprite->PlayAnimation(name, -1, 1.0f);
    }
}