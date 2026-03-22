#pragma once
#include "BaseEnemy.h"
#include "../Abilities/BossLaser.h"
#include "../Abilities/BossExplosive.h"

enum class BOSS_STATE
{
    SPAWNING,
    IDLING,
    MOVING,
    EXHAUSTED,
    ABILITY1,
    ABILITY2,
    ABILITY3,
    DEATH
};

enum class PendingAttack
{
    LASER,
    EXPLOSIVE,
    SPAWN_RANDOM_ENEMY,
    NONE
};

struct Weakpoint
{
    Transform2D transform;
    AEVec2 lerpVector{};
    f32 maxY = 0.0f;
    f32 minY = 0.0f;
    s32 dir = 1;    //1 to go up, -1 to go down
    AEGfxTexture* tex = nullptr;
    bool active = false;
};

class BossEnemy : public BaseEnemy
{
public:
    BossEnemy();
    ~BossEnemy() override;

    void Init(f32 x, f32 y, f32 w, f32 h, std::string spriteName) override;
    void Update(f32 deltaTime) override;
    void Draw() override;

    Weakpoint GetWeakpoint() const;

    void StartCast(PendingAttack attackType);
    void FireLaser();
    void FireExplosive();
    
    std::vector<BaseEnemy*>& GetPendingSpawns();

private:
    Weakpoint weakpoint;

    //FSM Variables
    BOSS_STATE currentBossState = BOSS_STATE::SPAWNING;
    f32 timer{1.0f};
    AEVec2 targetPos{};
    AEVec2 minCoordLimit{}, maxCoordLimit{};

    //Const timer nums
    const f32 idleTimer = 4.0f;
    const f32 exhaustedTimer = 15.0f;
    const f32 deathTimer = 1.0f;

    //FSM Functions
    void EnemyFSM(f32 deltaTime);
    void EnterIdling();
    void EnterMoving();
    bool UpdateMoving(f32 deltaTime);
    void EnterExhausted();
    void EnterDeath();
    bool UpdateTimer(f32 deltaTime);

    void ActivateWeakpoint();
    void UpdateWeakpoint(f32 deltaTime);

    // Attack Logic
    bool isAttacking = false;
    f32 attackTimer = 0.0f;
    f32 originalTimer = 1.0f;
    bool hasFired = false;

    // Aggro Logic
    bool isAggroed = false;
    bool isSurprised = false;
    f32 surpriseTimer = 0.0f;

    u32 stateCounter = 0;
    u32 maxStateCounter = 3;

    // Boss Ability Logic
    float attackCooldown = 0.f;
    bool isCasting = false;
    float castProgress = 0.f;
    float castDuration = 0.f;
    PendingAttack pendingAttack = PendingAttack::NONE;
    Transform2D castbarTransform;

    std::vector<BossLaser*> bossLasers;
    std::vector<BossExplosive*> bossExplosives;

    std::vector<BaseEnemy*> pendingSpawns;

    void SpawnRandomEnemy();
};

