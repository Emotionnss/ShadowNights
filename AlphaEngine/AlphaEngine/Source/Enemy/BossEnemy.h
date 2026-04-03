/*!
@file       BossEnemy.h
@author     Yap Yu Xuan Daevon (yuxuandaevon.yap), Bay Yeok Hao (yeokhao.bay)
@brief      This file contains declarations of functions used by BossEnemy for
            attacking the player. Also contains definitions of BOSS_STATE enum,
            PendingAttack enum, Weakpoint struct
*//*_______________________________________________________________________*/

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
    /**
     @brief Default contructor
     */
    BossEnemy();

    /**
     @brief Overriden destructor
     */
    ~BossEnemy() override;

    /**
     @brief     Initializes the boss. Overrides Init function from BaseEnemy
     @param f32 x - x position
     @param f32 y - y position
     @param f32 w - width of boss
     @param f32 h - height of boss
     @return void
     */
    void Init(f32 x, f32 y, f32 w, f32 h, std::string spriteName) override;
    void Update(f32 deltaTime) override;
    void Draw() override;

    Weakpoint GetWeakpoint() const;
    /*!
    @brief      StartCast function

    Queues the next pending ability, and calculates the casting bar

    @param		PendingAttack attackType - attack to queue
    */
    void StartCast(PendingAttack attackType);
    /*!
    @brief      FireLaser function

    Activates boss laser ability
    */
    void FireLaser();
    /*!
    @brief      FireLaser function

    Activates boss explosive ability
    */
    void FireExplosive();
    /*!
    @brief      RandomEnemy function

    Activates spawning of random enemy ability
    */
    void SpawnRandomEnemy();
    /*!
    @brief      GetPendingSpawns function

    Gets the pending random enemy to be spawned by SpawnRandomEnemy.
    This function is called inside BossScene to empty the internal vector of enemies
    into the scene's enemyList vector, so boss does not have to keep track of enemies internally
    */
    std::vector<BaseEnemy*>& GetPendingSpawns();

    void SpawnHealthBar();
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

    /**
     @brief Main enemy finite-state machine handler called from Update
     @param f32 deltaTime - elapsed time since last frame
     @return void
     */
    void EnemyFSM(f32 deltaTime);

    /**
     @brief Transition logic when entering the idling state
     @param void
     @return void
     */
    void EnterIdling();

    /**
     @brief Transition logic when entering the moving state
     @param void
     @return void
     */
    void EnterMoving();

    /**
     @brief Update logic for the moving state
     @param f32 deltaTime - elapsed time since last frame
     @return bool - true if movement completed / transition should occur, false otherwise
     */
    bool UpdateMoving(f32 deltaTime);

    /**
     @brief Transition logic when entering the exhausted state
     @param void
     @return void
     */
    void EnterExhausted();

    /**
     @brief Transition logic when entering the death state
     @param void
     @return void
     */
    void EnterDeath();

    /**
     @brief Utility to update and clamp timers used by FSM
     @param f32 deltaTime - elapsed time since last frame
     @return bool - true if timer expired, false otherwise
     */
    bool UpdateTimer(f32 deltaTime);

    /**
     @brief Activate the boss weakpoint (visuals/mechanics)
     @param void
     @return void
     */
    void ActivateWeakpoint();

    /**
     @brief Update weakpoint movement and state each frame
     @param f32 deltaTime - elapsed time since last frame
     @return void
     */
    void UpdateWeakpoint(f32 deltaTime);

    Transform2D healthbarOverlayTfm;
    Sprite* barOverlaySprite = nullptr;
    Sprite* barSprite = nullptr;

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
};

