#include "BatEnemy.h"
#include "../System/Map.h"
#include "../Player/Player.h"
#include <cmath>
#include <algorithm>
#include <cstdlib>

/*!
@file       BatEnemy.cpp
@author     Lim Chee Kang (l.cheekang)
@brief      This file contains definitions of functions and variables
             used for the BatEnemy class, which is a flying enemy that chases
			 the player when they get close, and shoots projectiles at them.
*//*_______________________________________________________________________*/

BatEnemy::BatEnemy() : BaseEnemy()
{
}

BatEnemy::~BatEnemy()
{
}

void BatEnemy::Init(float x, float y, float w, float h, std::string spriteName)
{
    BaseEnemy::Init(x, y, w, h, spriteName);

    deathAnimName = "batdeath";

    startX = x;
    startY = y;

    isAwake = false;
    moveSpeed = 150.0f;

    PlayAnim("batidle");
}

void BatEnemy::Update(float deltaTime)
{
    BaseEnemy::Update(deltaTime);
    if (stopped || isDying) return;

    // --- BAT AI ---
    Player* player = Player::GetInstance();
    AEVec2 myPos = transform.GetPosition();
    AEVec2 playerPos = player->GetTransform().GetPosition();

    float dx = playerPos.x - myPos.x;
    float dy = playerPos.y - myPos.y;
    float dist = sqrt(dx * dx + dy * dy);

    float visualWidth = fabs(transform.GetScale().x);
    float visualHeight = transform.GetScale().y;

    map = Map::GetInstance();

    // --- STATE MACHINE ---
    if (!isAwake)
    {
        // [IDLE STATE]
        velocity.x = 0.0f;
        velocity.y = 0.0f;

        if (dist < aggroRadius)
        {
            isAwake = true;
            PlayAnim("batmove");
        }
    }
    else
    {
        // [CHASE STATE]
        if (dist > aggroRadius * 1.5f)
        {
            isAwake = false;
            PlayAnim("batidle");
        }
        else
        {
            // A* PATHFINDING LOGIC
            //RECALCULATE PATH PERIODICALLY
            pathTimer -= deltaTime;
            if (pathTimer <= 0.0f)
            {
                Node targetNode;
                AEVec2 minLimits = { 0.0f, 0.0f };
                AEVec2 maxLimits = { static_cast<float>(map->GetNumCol() - 1),
                                     static_cast<float>(map->GetNumRow() - 1) };

                // Run your A* logic
                if (map->GeneratePath(myPos, playerPos, minLimits, maxLimits, targetNode))
                {
                    map->BuildPath(path, targetNode);
                    pathIndex = 0; // Reset to the first node in the new path
                }

                pathTimer = 0.5f; // Wait 0.5 seconds before recalculating
            }

            //MOVE ALONG THE PATH
            if (!path.empty() && pathIndex < path.size())
            {
                AEVec2 targetWaypoint = path[pathIndex];

                float wpDx = targetWaypoint.x - myPos.x;
                float wpDy = targetWaypoint.y - myPos.y;
                float wpDist = sqrt(wpDx * wpDx + wpDy * wpDy);

                // If we are close enough to the waypoint target the next one
                if (wpDist < 10.0f)
                {
                    pathIndex++;
                }
                else
                {
                    velocity.x = (wpDx / wpDist) * moveSpeed;
                    velocity.y = (wpDy / wpDist) * moveSpeed;
                }
            }
            else
            {
                // If no path is found, just fly straight at the player
                float dirX = dx / dist;
                float dirY = dy / dist;
                velocity.x = dirX * moveSpeed;
                velocity.y = dirY * moveSpeed;
            }

            //SPRITE FACING AND SHOOTING
            if (velocity.x > 0) transform.SetScale(-visualWidth, visualHeight);
            else                transform.SetScale(visualWidth, visualHeight);

            shootTimer -= deltaTime;
            if (shootTimer <= 0.0f)
            {
                //Only shoot if there isn't a wall in the way
                if (HasLineOfSight(playerPos))
                {
                    Shoot();
                }

                // Always reset the timer
                // This prevents the bat from machine-gunning player
                shootTimer = 2.0f;
            }
        }
    }

    // PHYSICS & MAP COLLISION
    float colliderWidth = 40.0f;
    float colliderHeight = 40.0f;

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
                    }
                    else
                    {
                        if (colDy > 0) position.y += py;
                        else           position.y -= py;
                        velocity.y = 0.f;
                    }
                }
            }
        }
    }

    transform.SetPosition(position.x, position.y);
    if (sprite) sprite->Update();
}

void BatEnemy::Shoot()
{
    Player* player = Player::GetInstance();
    AEVec2 myPos = transform.GetPosition();
    AEVec2 playerPos = player->GetTransform().GetPosition();

    float dx = playerPos.x - myPos.x;
    float dy = playerPos.y - myPos.y;
    float baseAngle = atan2f(dy, dx);

    float randomOffset = ((rand() % 100) / 100.0f - 0.5f) * 0.2f;

    baseAngle += randomOffset;

    float spread = 0.25f;

    for (int i = -1; i <= 1; i++)
    {
        float currentAngle = baseAngle + (i * spread);
        AEVec2 bulletDir = { cosf(currentAngle), sinf(currentAngle) };

        EnemyProjectile* newProj = new EnemyProjectile(myPos, bulletDir);
        newProj->slowed = this->slowed;
        enemyProjectiles.push_back(newProj);
    }
}

void BatEnemy::Draw()
{
    BaseEnemy::Draw();
}