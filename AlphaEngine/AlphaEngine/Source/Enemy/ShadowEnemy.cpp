#include "ShadowEnemy.h"
#include "../System/Map.h"
#include "../Player/Player.h"
#include <cmath>
#include <algorithm>

ShadowEnemy::ShadowEnemy() : BaseEnemy()
{
}

ShadowEnemy::~ShadowEnemy()
{
}

void ShadowEnemy::Init(float x, float y, float w, float h, std::string spriteName)
{
    BaseEnemy::Init(x, y, w, h, spriteName);

    //variables
    isAttacking = false;
    attackTimer = 0.0f;
    hasFired = false;

    isAggroed = false;
    isSurprised = false;
    surpriseTimer = 0.0f;

    PlayAnim("walkright");
}

void ShadowEnemy::Update(float deltaTime)
{
    // BaseEnemy handles Projectiles and Death animations
    BaseEnemy::Update(deltaTime);
    
    // If we are stopped or dying dont run AI or movement
    if (stopped || isDying) return;

    // SHADOW ENEMY AI & COLLISION
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

	//attack state has highest priority, then aggroed state, then patrol state
    if (isAttacking)
    {
        velocity.x = 0.0f;
        attackTimer -= deltaTime;

        if (dx > 0) transform.SetScale(visualWidth, visualHeight);
        else        transform.SetScale(-visualWidth, visualHeight);

        if (!hasFired)
        {
            Shoot(); // Uses BaseEnemy Shoot function
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
            attackTimer = slowed ? originalTimer * 2.f : originalTimer;
            //attackTimer = 1.0f;
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
                // CHASE STATE
                path.clear();

                AEVec2 myTile = map->GetTilePos(myPos.x, myPos.y);
                AEVec2 playerTile = map->GetTilePos(playerPos.x, playerPos.y);

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

                float targetX = playerPos.x;

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
            // PATROL STATE
            isAggroed = false;
            path.clear();

            velocity.x = moveSpeed;
            PlayAnim("walkright");

            if (moveSpeed > 0) transform.SetScale(visualWidth, visualHeight);
            else               transform.SetScale(-visualWidth, visualHeight);
        }
    }

    // PHYSICS & MAP COLLISION
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

void ShadowEnemy::Draw()
{
    BaseEnemy::Draw();
}
