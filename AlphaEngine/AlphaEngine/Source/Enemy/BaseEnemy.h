#pragma once
#include "../Player/GameObject.h"
#include "../Sprite/Sprite.h"
#include <string>

class BaseEnemy : public GameObject
{
public:
    BaseEnemy();
    ~BaseEnemy();

    void Init(float x, float y, float w, float h, std::string spriteName);
    void Update(float deltaTime) override;
    void Draw() override;

    // [NEW] Call this when hit by projectile
    void TakeDamage();

    // [NEW] Check if animation is finished so TestScene can delete me
    bool IsReadyToDelete();

private:
    Sprite* sprite = nullptr;

    // Physics
    float gravity = -1400.0f;
    bool isGrounded = false;

    // Movement
    float moveSpeed = 100.0f;

    // Attack Logic
    bool isAttacking = false;
    float attackTimer = 0.0f;
    float detectionRadius = 150.0f;

    // [NEW] Death Logic
    bool isDying = false;
    float deathTimer = 0.0f;

    // Helper to switch animations
    std::string currentAnimState = "";
    void PlayAnim(std::string name);
};