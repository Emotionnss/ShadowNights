#include "ArcherEnemy.h"
#include "../System/Map.h"
#include "../Player/Player.h"
#include <cmath>
#include <algorithm>

/*!
@file       ArcherEnemy.cpp
@author     Lim Chee Kang (l.cheekang)
@brief      This file contains definitions of functions and variables
             used for the ArcherEnemy class, which is a type of enemy
             that shoots projectiles at the player from a distance, and contains
             the specific implementation of the archer's behavior, such as its
			 shooting logic and animations.
*//*_______________________________________________________________________*/

ArcherEnemy::ArcherEnemy() : BaseEnemy()
{
}

ArcherEnemy::~ArcherEnemy()
{
}

void ArcherEnemy::Init(float x, float y, float w, float h, std::string spriteName)
{
    BaseEnemy::Init(x, y, w, h, spriteName);

    deathAnimName = "archerdeath";
    PlayAnim("archeridle");

    moveSpeed = 0.0f;
    velocity = { 0.0f, 0.0f };
    shootTimer = fireRate;
}

void ArcherEnemy::Update(float deltaTime)
{
    BaseEnemy::Update(deltaTime);

    if (stopped || isDying) return;

    // --- ARCHER AI ---
    Player* player = Player::GetInstance();
    AEVec2 myPos = transform.GetPosition();
    AEVec2 playerPos = player->GetTransform().GetPosition();

    float dx = playerPos.x - myPos.x;
    float dy = playerPos.y - myPos.y;
    float dist = sqrt(dx * dx + dy * dy);

    float visualWidth = fabs(transform.GetScale().x);
    float visualHeight = transform.GetScale().y;

    if (dx > 0) transform.SetScale(visualWidth, visualHeight);
    else        transform.SetScale(-visualWidth, visualHeight);

    if (HasLineOfSight(playerPos))
    {
        shootTimer -= deltaTime;

        if (shootTimer <= 0.0f)
        {
            Shoot();
            shootTimer = fireRate;
        }

        // 2. Animation Logic
        if (shootTimer > fireRate - 0.5f)
        {
            PlayAnim("archerattack");
        }
        else
        {
            PlayAnim("archeridle");
        }
    }
    else
    {
        shootTimer = fireRate;
        PlayAnim("archeridle");
    }

    velocity.y += gravity * deltaTime;

    velocity.x = 0.0f;

    AEVec2 position = transform.GetPosition();
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;

    float colliderWidth = 40.0f;
    float colliderHeight = 60.0f;
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
                        if (colDy > 0 && velocity.y <= 0.f) isGrounded = true;
                    }
                }
            }
        }
    }

    transform.SetPosition(position.x, position.y);
    if (sprite) sprite->Update();
}

void ArcherEnemy::Draw()
{
    BaseEnemy::Draw();
}

void ArcherEnemy::Shoot()
{
    Player* player = Player::GetInstance();
    AEVec2 myPos = transform.GetPosition();
    AEVec2 playerPos = player->GetTransform().GetPosition();

    float dx = playerPos.x - myPos.x;
    float dy = playerPos.y - myPos.y;
    float angle = atan2f(dy, dx);

    AEVec2 bulletDir = { cosf(angle), sinf(angle) };

    EnemyProjectile* newProj = new EnemyProjectile(myPos, bulletDir);
    newProj->slowed = this->slowed;

    newProj->isHoming = true;
    newProj->SetLifespan(4.0f);

    enemyProjectiles.push_back(newProj);
}
