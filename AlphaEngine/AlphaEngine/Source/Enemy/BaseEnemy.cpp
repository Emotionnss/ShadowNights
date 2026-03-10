#include "BaseEnemy.h"
#include "../System/CSVManager.h"

BaseEnemy::BaseEnemy()
{
    isStatic = false;
    velocity = { 0.0f, 0.0f };
    sprite = nullptr;

    settingsManager = SettingsManager::GetInstance();
    map = Map::GetInstance();
}

BaseEnemy::~BaseEnemy()
{
    if (sprite) {
        delete sprite;
        sprite = nullptr;
    }

    for (EnemyProjectile* p : enemyProjectiles) {
        delete p;
    }
    enemyProjectiles.clear();

    settingsManager = nullptr;
    map = nullptr;
}

void BaseEnemy::Init(float x, float y, float w, float h, std::string spriteName)
{
    transform.SetPosition(x, y);
    transform.SetScale(w, h);

    if (sprite) delete sprite;
    sprite = new Sprite(CSVManager::ReadInfo(spriteName));

    originalSpeed = 100.f;
    moveSpeed = originalSpeed;
    velocity = { moveSpeed, 0.0f };
    isGrounded = false;
    
    // Initialize Death Variables
    isDying = false;
    oriDeathTimer = 1.0f;
    deathTimer = 0.0f;

    currentAnimState = "";
    origAnimSpeed = 1.f;
    animSpeed = origAnimSpeed;
}

void BaseEnemy::TakeDamage()
{
    if (!isDying)
    {
        isDying = true;
        velocity = { 0.0f, 0.0f };
        
        deathTimer = oriDeathTimer; 

        PlayAnim(deathAnimName); 
    }
}

bool BaseEnemy::IsReadyToDelete() const
{
    return isDying && (deathTimer <= 0.0f);
}

void BaseEnemy::Update(float deltaTime)
{
    if (!stopped)
    {
        // Update Projectiles
        for (auto it = enemyProjectiles.begin(); it != enemyProjectiles.end(); )
        {
            (*it)->Speed(slowed ? (*it)->OriginalSpeed() / 20.f : (*it)->OriginalSpeed());
            (*it)->Update(deltaTime);

            if ((*it)->IsDead())
            {
                delete* it;
                it = enemyProjectiles.erase(it);
            }
            else
            {
                ++it;
            } 
        }

        if (isDying)
        {
            deathTimer -= deltaTime;
            
            // Continue ticking the animation frames while dying
            if (sprite) sprite->Update(); 
        }
    }
}

void BaseEnemy::Draw()
{
    float alpha = 1.0f;

    if (isDying)
    {
        alpha = deathTimer / oriDeathTimer;
        
        // Safety clamp to ensure alpha stays within 0.0 to 1.0
        if (alpha < 0.0f) alpha = 0.0f;
        if (alpha > 1.0f) alpha = 1.0f;
    }

    // Pass the calculated transparency to the Render function
    if (sprite) sprite->Render(transform, alpha);

    for (EnemyProjectile* p : enemyProjectiles)
    {
        p->Draw();
    }

    // Debug lines
    if (path.size() > 1)
    {
        if (settingsManager->GetTilesDebug())
        {
            map->RenderCheckedTilesDebug();
        }

        if (settingsManager->GetPathDebug())
        {
            map->RenderPathDebug(path);
        }
    }
}

void BaseEnemy::PlayAnim(std::string name)
{
    if (currentAnimState != name)
    {
        currentAnimState = name;

        s32 loopCount = (name == deathAnimName) ? 1 : -1;

        if (sprite) sprite->PlayAnimation(name, loopCount, animSpeed);
    }
}

// Slow/Stop logic remains the same
bool BaseEnemy::Slowed() const { return slowed; }
void BaseEnemy::Slowed(bool active)
{
    slowed = active;
    switch (slowed)
    {
    case true:
        animSpeed = origAnimSpeed * 10.f;
        moveSpeed = originalSpeed / 10.f;
        break;
    case false:
        moveSpeed = originalSpeed;
        animSpeed = origAnimSpeed;
        break;
    }
}

bool BaseEnemy::isStopped() const { return stopped; }
void BaseEnemy::isStopped(bool active) { stopped = active; }

void BaseEnemy::Shoot()
{
    float dirX = (transform.GetScale().x > 0) ? 1.0f : -1.0f;
    AEVec2 spawnPos = transform.GetPosition();
    spawnPos.y += 10.0f;
    EnemyProjectile* newProj = new EnemyProjectile(spawnPos, { dirX, 0.0f });
    newProj->slowed = this->slowed;
    enemyProjectiles.push_back(newProj);
}