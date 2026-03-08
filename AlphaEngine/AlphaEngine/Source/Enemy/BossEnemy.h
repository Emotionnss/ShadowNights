#pragma once
#include "BaseEnemy.h"

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

    u32 stateCounter;
    u32 maxStateCounter = 3;
};

