#pragma once
#include "../Player/GameObject.h"
#include "../Sprite/Sprite.h"
#include "../Projectiles/EnemyProjectile.h"
#include <vector>
#include <string>

class BaseEnemy : public GameObject
{
public:
    BaseEnemy();
    ~BaseEnemy();

    void Init(float x, float y, float w, float h, std::string spriteName);
    void Update(float deltaTime) override;
    void Draw() override;

    // Call this when hit by player projectile
    void TakeDamage();

    // Check if death animation is done so TestScene can delete me
    bool IsReadyToDelete();

    // Accessor for TestScene to check collisions
    std::vector<EnemyProjectile*>& GetProjectiles() { return enemyProjectiles; }

    bool isStopped();
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

    // Death Logic
    bool isDying = false;
    float deathTimer = 0.0f;

    // [NEW] Projectile Logic
    std::vector<EnemyProjectile*> enemyProjectiles;
    bool hasFired = false; // Flag to ensure we shoot only once per attack
    void Shoot();          // Internal helper to spawn projectile

    // Animation Helper
    std::string currentAnimState = "";
    void PlayAnim(std::string name);

    //stopped in timestop zone
    bool stopped = false;
};