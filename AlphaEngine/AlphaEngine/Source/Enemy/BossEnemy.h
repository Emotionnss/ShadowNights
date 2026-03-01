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

class BossEnemy : public BaseEnemy
{
public:
    BossEnemy();
    ~BossEnemy() override;

    void Init(float x, float y, float w, float h, std::string spriteName) override;
    void Update(float deltaTime) override;
    void Draw() override;

private:
    //FSM Variables
    BOSS_STATE currentBossState = BOSS_STATE::SPAWNING;
    float timer{1.0f};
    AEVec2 targetPos{};
    AEVec2 minCoordLimit{}, maxCoordLimit{};

    //Const timer nums
    const float idleTimer = 4.0f;
    const float exhaustedTimer = 6.0f;
    const float deathTimer = 1.0f;

    //FSM Functions
    void EnemyFSM(float deltaTime);
    void EnterIdling();
    void EnterMoving();
    bool UpdateMoving(float deltaTime);
    void EnterExhausted();
    void EnterDeath();
    bool UpdateTimer(float deltaTime);

    // Attack Logic
    bool isAttacking = false;
    float attackTimer = 0.0f;
    float originalTimer = 1.0f;
    bool hasFired = false;

    // Aggro Logic
    bool isAggroed = false;
    bool isSurprised = false;
    float surpriseTimer = 0.0f;
};

