#pragma once
#include "../GameObject/GameObject.h"
#include "../Sprite/Sprite.h"
#include "../Projectiles/EnemyProjectile.h"
#include "../System/SettingsManager.h"
#include "../System/Map.h"
#include <vector>
#include <string>

/*!
@file       BaseEnemy.h
@author     Lim Chee Kang (l.cheekang)
@brief      This file contains definitions of functions and variables
             used for the BaseEnemy class, which is the parent class
             of all enemies in the game, and contains common variables and
            functions used across all enemy types.
*//*_______________________________________________________________________*/

class BaseEnemy : public GameObject
{
public:
    /*!
    @brief    Constructor for BaseEnemy.
    *//*______________________________________________________________*/
    BaseEnemy();

    /*!
    @brief    Virtual destructor for proper cleanup of derived enemies.
    *//*______________________________________________________________*/
    virtual ~BaseEnemy();

    /*!
    @brief    Initializes the base enemy properties.
    @param    x Initial x-coordinate.
    @param    y Initial y-coordinate.
    @param    w Width of the bounding box.
    @param    h Height of the bounding box.
    @param    spriteName The string identifier for the texture.
    *//*______________________________________________________________*/
    virtual void Init(float x, float y, float w, float h, std::string spriteName);

    /*!
    @brief    Updates physics, gravity, and base enemy logic.
    @param    deltaTime The time elapsed since the last frame.
    *//*______________________________________________________________*/
    virtual void Update(float deltaTime);

    /*!
    @brief    Renders the base enemy and its health bar.
    *//*______________________________________________________________*/
    virtual void Draw();

    /*!
    @brief    Reduces the enemy's health by the specified damage amount.
    @param    damage The amount of health to deduct.
    *//*______________________________________________________________*/
    virtual void TakeDamage(int damage);

    /*!
    @brief    Checks if the enemy's death animation has finished and is ready to be deleted.
    @return   True if ready to delete, false otherwise.
    *//*______________________________________________________________*/
    bool IsReadyToDelete() const;

    /*!
    @brief    Retrieves a reference to the list of active projectiles fired by this enemy.
    @return   A reference to the vector of EnemyProjectile pointers.
    *//*______________________________________________________________*/
    std::vector<EnemyProjectile*>& GetProjectiles() { return enemyProjectiles; }

    /*!
    @brief    Checks if the enemy is currently affected by a slow debuff.
    @return   True if slowed, false otherwise.
    *//*______________________________________________________________*/
    bool Slowed() const;

    /*!
    @brief    Sets the slowed status of the enemy.
    @param    active True to apply slow, false to remove.
    *//*______________________________________________________________*/
    void Slowed(bool active);

    /*!
    @brief    Checks if the enemy is currently stopped/stunned.
    @return   True if stopped, false otherwise.
    *//*______________________________________________________________*/
    bool isStopped() const;

    /*!
    @brief    Sets the stopped/stunned status of the enemy.
    @param    active True to apply stop, false to remove.
    *//*______________________________________________________________*/
    void isStopped(bool active);

    /*!
    @brief    Checks if the enemy's health has reached zero and it is playing its death animation.
    @return   True if currently dying, false otherwise.
    *//*______________________________________________________________*/
    bool IsDying() const { return isDying; }

    /*!
    @brief    Casts a ray to check if there are no walls between the enemy and a target position.
    @param    targetPos The destination coordinate to check sightlines against.
    @return   True if the line of sight is clear, false if blocked by a wall.
    *//*______________________________________________________________*/
    bool HasLineOfSight(AEVec2 targetPos);

    /*!
    @brief    Checks for collision between a specific projectile and the map's geometry.
    @param    proj The projectile to check.
    @param    map Pointer to the current active map.
    @return   True if a collision occurred, false otherwise.
    *//*______________________________________________________________*/
    bool CheckProjectileCollision(BaseProjectile* proj, Map* map);

    /*!
    @brief    Resolves horizontal (X-axis) collisions with the map geometry.
    @param    position Reference to the enemy's current position to be corrected.
    *//*______________________________________________________________*/
    void ResolveX(AEVec2& position);

    /*!
    @brief    Resolves vertical (Y-axis) collisions with the map geometry.
    @param    position Reference to the enemy's current position to be corrected.
    *//*______________________________________________________________*/
    void ResolveY(AEVec2& position);

    bool isBoss = false;

    /*!
    @brief    Retrieves the current health of the enemy.
    @return   The current health value.
    *//*______________________________________________________________*/
    int GetHealth() const { return health; }

    /*!
    @brief    Sets or overrides the current health of the enemy.
    @param    hp The new health value to apply.
    *//*______________________________________________________________*/
    void SetHealth(int hp) { health = hp; }
    
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

    Transform2D healthbarTransform;
    int health = 10;
    int maxHealth = 10;

    // Projectile Logic
    std::vector<EnemyProjectile*> enemyProjectiles;

    /*!
    @brief    Base virtual function for shooting. Overridden by derived classes.
    *//*______________________________________________________________*/
    virtual void Shoot();

    // Animation Helper
    std::string currentAnimState = "";

    /*!
    @brief    Changes the current sprite animation state.
    @param    name The string identifier for the animation to play.
    *//*______________________________________________________________*/
    void PlayAnim(std::string name);

    float origAnimSpeed{ 1.f };
    float animSpeed{ origAnimSpeed };

    SettingsManager* settingsManager = nullptr;
    Map* map = nullptr;

    std::string deathAnimName = "shadowdeath";

    // Pathfinding lines
    std::vector<AEVec2> path;
};