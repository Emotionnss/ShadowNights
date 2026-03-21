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

    isDying = false;
    oriDeathTimer = 1.0f;
    deathTimer = 0.0f;

    currentAnimState = "";
    origAnimSpeed = 1.f;
    animSpeed = origAnimSpeed;
}

void BaseEnemy::TakeDamage(int damage)
{
    health -= damage;

    if (health <= 0)
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
        if (alpha < 0.0f) alpha = 0.0f;
        if (alpha > 1.0f) alpha = 1.0f;
    }

    if (sprite) sprite->Render(transform, alpha);

    if (!isDying)
    {
        // Safely wipe the paintbrush
        AEGfxTextureSet(nullptr, 0, 0);

        AEGfxSetRenderMode(AE_GFX_RM_COLOR);
        AEGfxSetColorToMultiply(1.0f, 0.0f, 0.0f, 1.0f);
        AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
        AEGfxSetBlendMode(AE_GFX_BM_BLEND);
        AEGfxSetTransparency(1.0f);

        healthbarTransform = transform;

        healthbarTransform.SetPosition(
            transform.GetPosition().x,
            transform.GetPosition().y + (0.5f * transform.GetScale().y) + 15.f
        );
        healthbarTransform.SetScale((static_cast<float>(health) / static_cast<float>(maxHealth)) * 100.f, 10.f);

        AEGfxSetTransform(healthbarTransform.GetMatrix().m);
        AEGfxMeshDraw(MeshManager::GetInstance()->GetMesh(SQUARE), AE_GFX_MDM_TRIANGLES);
        AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
    }

    for (EnemyProjectile* p : enemyProjectiles)
    {
        p->Draw(nullptr);
    }

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

    AEGfxTextureSet(nullptr, 0, 0);
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    AEGfxSetTransparency(1.0f);
    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
    AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

    // This specifically stops the Giant Red Bars!
    AEMtx33 identity;
    AEMtx33Identity(&identity);
    AEGfxSetTransform(identity.m);
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