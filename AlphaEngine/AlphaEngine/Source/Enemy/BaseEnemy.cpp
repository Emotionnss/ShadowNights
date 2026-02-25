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
    sprite = new Sprite(CSVManager::ReadInfo(SPRITESHEET_DATA, spriteName));

    originalSpeed = 100.f;
    moveSpeed = originalSpeed;
    velocity = { moveSpeed, 0.0f };
    isGrounded = false;
    isDying = false;
    deathTimer = 0.0f;
    currentAnimState = "";
}

void BaseEnemy::TakeDamage()
{
    if (!isDying)
    {
        isDying = true;
        velocity = { 0.0f, 0.0f };
        deathTimer = 1.0f;
        PlayAnim(deathAnimName); // Use the variable instead of the hardcoded string!
    }
}

bool BaseEnemy::IsReadyToDelete()
{
    return isDying && (deathTimer <= 0.0f);
}

bool BaseEnemy::Slowed() const
{
    return slowed;
}

void BaseEnemy::Slowed(bool active)
{
    slowed = active;
    switch (slowed)
    {
    case true:
        moveSpeed = originalSpeed / 2.f;
        break;
    case false:
        moveSpeed = originalSpeed;
        break;
    }
}

bool BaseEnemy::isStopped() const
{
    return stopped;
}

void BaseEnemy::isStopped(bool active)
{
    stopped = active;
}

void BaseEnemy::Shoot()
{
    float dir = transform.GetScale().x;
    AEVec2 spawnPos = transform.GetPosition();
    spawnPos.y += 10.0f;

    EnemyProjectile* newProj = new EnemyProjectile(spawnPos, dir);

    newProj->slowed = this->slowed;

    enemyProjectiles.push_back(newProj);
}

void BaseEnemy::Update(float deltaTime)
{
    if (!stopped)
    {
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

        // DEATH TIMER
        if (isDying)
        {
            deathTimer -= deltaTime;
            if (sprite) sprite->Update();
        }
    }
}

void BaseEnemy::Draw()
{
    if (sprite) sprite->Render(transform);

    for (EnemyProjectile* p : enemyProjectiles)
    {
        p->Draw();
    }

    // debug lines
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
        if (sprite) sprite->PlayAnimation(name, -1, 1.0f);
    }
}