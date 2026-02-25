#pragma once
#include "../Player/GameObject.h"
#include "../Sprite/Sprite.h"
#include "../Projectiles/EnemyProjectile.h"
#include "../System/SettingsManager.h"
#include "../System/Map.h"
#include <vector>
#include <string>

class BaseEnemy : public GameObject
{
public:
    BaseEnemy();
    virtual ~BaseEnemy(); // Must be virtual so children delete properly!

    virtual void Init(float x, float y, float w, float h, std::string spriteName);
    virtual void Update(float deltaTime);
    virtual void Draw();

    virtual void TakeDamage();
    bool IsReadyToDelete();
    std::vector<EnemyProjectile*>& GetProjectiles() { return enemyProjectiles; }
    bool Slowed() const;
    void Slowed(bool active);
    bool isStopped() const;
    void isStopped(bool active);
protected: // Changed to PROTECTED so ShadowEnemy can access these!
    Sprite* sprite = nullptr;

    // Physics
    float gravity = -1400.0f;
    bool isGrounded = false;
    float originalSpeed = 100.f;
    float moveSpeed = originalSpeed;

    bool stopped = false;
    bool slowed = false;

    // Death Logic
    bool isDying = false;
    float deathTimer = 0.0f;

    // Projectile Logic
    std::vector<EnemyProjectile*> enemyProjectiles;
    void Shoot();

    // Animation Helper
    std::string currentAnimState = "";
    void PlayAnim(std::string name);

    SettingsManager* settingsManager = nullptr;
    Map* map = nullptr;

    // Pathfinding lines
    std::vector<AEVec2> path;
};