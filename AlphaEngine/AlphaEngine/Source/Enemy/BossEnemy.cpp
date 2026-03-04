#include "BossEnemy.h"
#include "../System/Map.h"
#include "../Player/Player.h"
#include <cmath>
#include <algorithm>

BossEnemy::BossEnemy() : BaseEnemy()
{
}

BossEnemy::~BossEnemy()
{
}

void BossEnemy::Init(float x, float y, float w, float h, std::string spriteName)
{
    BaseEnemy::Init(x, y, w, h, spriteName);

    //variables
    isAttacking = false;
    attackTimer = 0.0f;
    hasFired = false;

    isAggroed = false;
    isSurprised = false;
    surpriseTimer = 0.0f;

    transform.SetScale(400, 400);
}

void BossEnemy::Update(float deltaTime)
{
    // BaseEnemy handles Projectiles and Death animations
    BaseEnemy::Update(deltaTime);
    
    // If we are stopped or dying dont run AI or movement
    if (stopped || isDying) return;

    //Update Enemy FSM
    EnemyFSM(deltaTime);

    Map* map = Map::GetInstance();
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
}

void BossEnemy::Draw()
{
    BaseEnemy::Draw();
}

void BossEnemy::EnemyFSM(float deltaTime)
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
        if (UpdateTimer(deltaTime))
        {
            EnterMoving();
        }
        break;
    }

    //Enter exhaustion mode thru some means idk yet
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
}

void BossEnemy::EnterMoving()
{
    //Set new state
    currentBossState = BOSS_STATE::MOVING;

    //Set targetPos for moving towards
    targetPos = { AERandFloat() * (maxCoordLimit.x - minCoordLimit.x) + minCoordLimit.x, AERandFloat() * (maxCoordLimit.y - minCoordLimit.y) + minCoordLimit.y };

    //Get vector from target to current pos;
    AEVec2 currentPos = transform.GetPosition();
    float dx = targetPos.x - currentPos.x;
    float dy = targetPos.y - currentPos.y;
    float dist = sqrt(dx * dx + dy * dy);

    float dirX = dx / dist;
    float dirY = dy / dist;

    velocity = { dirX * moveSpeed, dirY * moveSpeed };
}

bool BossEnemy::UpdateMoving(float deltaTime)
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

    //Enable weakpoint or smth idk

    timer = exhaustedTimer;
}

void BossEnemy::EnterDeath()
{
    currentBossState = BOSS_STATE::DEATH;
    timer = deathTimer;
    sprite->PlayAnimation("death", 1, deathTimer);
}

bool BossEnemy::UpdateTimer(float deltaTime)
{
    timer -= deltaTime;

    //Signal to trigger changes
    if (timer <= 0)
    {
        return true;
    }

    return false;
}
