#include "BossEnemy.h"
#include "../System/Map.h"
#include "../Player/Player.h"
#include "../System/MeshManager.h"
#include <cmath>
#include <algorithm>

BossEnemy::BossEnemy() : BaseEnemy()
{
}

BossEnemy::~BossEnemy()
{
    AEGfxTextureUnload(weakpoint.tex);
}

void BossEnemy::Init(f32 x, f32 y, f32 w, f32 h, std::string spriteName)
{
    BaseEnemy::Init(x, y, w, h, spriteName);

    deathAnimName = "death";

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
}

void BossEnemy::Update(f32 deltaTime)
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

    //Enable weakpoint or smth idk
    ActivateWeakpoint();

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

void BossEnemy::UpdateWeakpoint(f32 deltaTime)
{
    AEVec2 pos = weakpoint.transform.GetPosition();

    if (pos.y >= weakpoint.maxY)
    {
        weakpoint.dir = -1;
        weakpoint.lerpVector = { pos.x, weakpoint.minY - 2.f };
    }
    else if (pos.y <= weakpoint.minY)
    {
        weakpoint.dir = 1;
        weakpoint.lerpVector = { pos.x, weakpoint.maxY + 2.f };
    }

    //pos.y += weakpoint.dir * 25.f * deltaTime;

    AEVec2Lerp(&pos, &pos, &weakpoint.lerpVector, deltaTime);

    weakpoint.transform.SetPosition(pos.x, pos.y);
}

Weakpoint BossEnemy::GetWeakpoint() const
{
    return weakpoint;
}
