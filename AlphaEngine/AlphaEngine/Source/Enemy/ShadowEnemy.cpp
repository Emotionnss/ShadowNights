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
    BaseEnemy::Update(deltaTime);

    if (isDying || stopped) return;

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

    float attackRadius = 300.0f;
    float aggroRadius = 300.0f;

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
        // Since both ranges are the same, we only need to check if the player is within range
        if (dist < attackRadius)
        {
            if (!isAggroed)
            {
                // Trigger the surprise reaction the moment they enter the radius
                isAggroed = true;
                isSurprised = true;
                surpriseTimer = 0.5f;
            }
            else
            {
                // Once the surprise timer finishes, begin attacking immediately!
                isAttacking = true;

                // If you have a slow effect implemented, keep this, otherwise just use originalTimer
                attackTimer = originalTimer;
                PlayAnim("attack1");
            }
        }
        else
        {
            // PATROL STATE
            isAggroed = false;

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