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
    virtual ~BaseEnemy();

    virtual void Init(float x, float y, float w, float h, std::string spriteName);
    virtual void Update(float deltaTime);
    virtual void Draw();

    virtual void TakeDamage();
    bool IsReadyToDelete() const;
    std::vector<EnemyProjectile*>& GetProjectiles() { return enemyProjectiles; }
    bool Slowed() const;
    void Slowed(bool active);
    bool isStopped() const;
    void isStopped(bool active);
    bool IsDying() const { return isDying; }

protected:
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
    float oriDeathTimer = 1.0f;

    // Projectile Logic
    std::vector<EnemyProjectile*> enemyProjectiles;
    virtual void Shoot();

    // Animation Helper
    std::string currentAnimState = "";
    void PlayAnim(std::string name);

    float origAnimSpeed{ 1.f };
    float animSpeed{ origAnimSpeed };

    SettingsManager* settingsManager = nullptr;
    Map* map = nullptr;

    std::string deathAnimName = "shadowdeath";

    // Pathfinding lines
    std::vector<AEVec2> path;
};