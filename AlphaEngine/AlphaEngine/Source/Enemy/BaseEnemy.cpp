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

    moveSpeed = 100.0f;
    velocity = { moveSpeed, 0.0f };
    isGrounded = false;

    isAttacking = false;
    attackTimer = 0.0f;
    hasFired = false;

    isDying = false;
    deathTimer = 0.0f;

    isAggroed = false;
    isSurprised = false;
    surpriseTimer = 0.0f;

    currentAnimState = "";
    PlayAnim("walkright");
}

void BaseEnemy::TakeDamage()
{
    if (!isDying)
    {
        isDying = true;
        velocity = { 0.0f, 0.0f };
        deathTimer = 1.0f;
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

void BaseEnemy::Shoot()
{
    float dir = transform.GetScale().x;
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
        // 1. UPDATE PROJECTILES
        // ------------------------------------------
        for (auto it = enemyProjectiles.begin(); it != enemyProjectiles.end(); )
        {
            (*it)->Update(deltaTime);

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
            return;
        }

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

        float attackRadius = 150.0f;
        float aggroRadius = 450.0f;

        float visualWidth = fabs(transform.GetScale().x);
        float visualHeight = transform.GetScale().y;

        // ------------------------------------------
        // 2. STATE MACHINE
        // ------------------------------------------
        if (isAttacking)
        {
            velocity.x = 0.0f;
            attackTimer -= deltaTime;

            if (dx > 0) transform.SetScale(visualWidth, visualHeight);
            else        transform.SetScale(-visualWidth, visualHeight);

            if (!hasFired)
            {
                Shoot();
                hasFired = true;
            }

            if (attackTimer <= 0.0f)
            {
                isAttacking = false;
                hasFired = false;
                PlayAnim("walkright");
            }
        }
        else if (isSurprised)
        {
            velocity.x = 0.0f;
            surpriseTimer -= deltaTime;

            if (dx > 0) transform.SetScale(visualWidth, visualHeight);
            else        transform.SetScale(-visualWidth, visualHeight);

            PlayAnim("aggro");

            if (surpriseTimer <= 0.0f)
            {
                isSurprised = false;
            }
        }
        else
        {
            if (dist < attackRadius)
            {
                isAttacking = true;
                attackTimer = 1.0f;
                PlayAnim("attack1");
            }
            else if (dist < aggroRadius)
            {
                if (!isAggroed)
                {
                    isAggroed = true;
                    isSurprised = true;
                    surpriseTimer = 0.5f;
                }
                else
                {
                    // --- CHASE STATE ---
                    path.clear();

                    AEVec2 myTile = map->GetTilePos(myPos.x, myPos.y);
                    AEVec2 playerTile = map->GetTilePos(playerPos.x, playerPos.y);

                    // Generate the path PURELY for the visual debug lines
                    if (myTile.x >= 0 && myTile.x < map->GetNumCol() &&
                        myTile.y >= 0 && myTile.y < map->GetNumRow() &&
                        playerTile.x >= 0 && playerTile.x < map->GetNumCol() &&
                        playerTile.y >= 0 && playerTile.y < map->GetNumRow())
                    {
                        AEVec2 maxBounds = { (float)map->GetNumCol() - 1.0f, (float)map->GetNumRow() - 1.0f };
                        Node node;

                        if (map->GeneratePath(myPos, playerPos, { 0, 0 }, maxBounds, node))
                        {
                            path = map->BuildPath(path, node);
                        }
                    }

                    // MOVEMENT: Completely ignore the grid
                    float targetX = playerPos.x;

                    // prevent jitter
                    if (targetX > myPos.x + 5.0f)
                    {
                        velocity.x = fabs(moveSpeed);
                        transform.SetScale(visualWidth, visualHeight);
                    }
                    else if (targetX < myPos.x - 5.0f)
                    {
                        velocity.x = -fabs(moveSpeed);
                        transform.SetScale(-visualWidth, visualHeight);
                    }
                    else
                    {
                        velocity.x = 0.0f;
                    }

                    PlayAnim("walkright");
                }
            }
            else
            {
                // --- PATROL STATE ---
                isAggroed = false;
                path.clear();

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

        if (hitWall && !isAggroed)
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

    for (EnemyProjectile* p : enemyProjectiles)
    {
        p->Draw();
    }

    if (!path.empty())
    {
        Map::GetInstance()->RenderPathDebug(path);
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