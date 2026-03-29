#include "EnemyProjectile.h"
#include "../System/CSVManager.h"
#include <cmath>
#include "../Player/Player.h"
#include "../particle/ParticleManager.h"

EnemyProjectile::EnemyProjectile(AEVec2 pos, AEVec2 dir)
    : BaseProjectile(pos, dir, true)
{
    oriSpeed = 400.0f;
    speed = oriSpeed;

    transform.SetScale(40.0f, 40.0f);
    transform.SetPosition(pos.x, pos.y);
    SetLifespan(3.0f);

    // Initialize your animated sprite
    sprite = new Sprite(CSVManager::ReadInfo("shadoworb"));
    sprite->PlayDefaultAnimation(-1, 1.0f);
}

EnemyProjectile::~EnemyProjectile()
{
    if (sprite)
    {
        delete sprite;
        sprite = nullptr;
    }
    Exit();
}

void EnemyProjectile::Update(float deltaTime)
{
    if (GetLifespan() > 0.f)
    {
        if (!stopped)
        {
            SetLifespan(GetLifespan() - 1.f * deltaTime);

            // Homing AI Logic
            if (isHoming)
            {
                Player* player = Player::GetInstance();
                AEVec2 playerPos = player->GetTransform().GetPosition();
                AEVec2 myPos = transform.GetPosition();

                float dx = playerPos.x - myPos.x;
                float dy = playerPos.y - myPos.y;
                float dist = sqrt(dx * dx + dy * dy);

                if (dist > 0.0f)
                {
                    float perfectX = dx / dist;
                    float perfectY = dy / dist;

                    float turnSpeed = 3.0f;
                    AEVec2 currentDir = GetDirection();

                    float newDirX = currentDir.x + (perfectX - currentDir.x) * turnSpeed * deltaTime;
                    float newDirY = currentDir.y + (perfectY - currentDir.y) * turnSpeed * deltaTime;

                    float newMag = sqrt(newDirX * newDirX + newDirY * newDirY);
                    SetDirection({ newDirX / newMag, newDirY / newMag });
                }
            }

            speed = slowed ? (oriSpeed * 0.5f) : oriSpeed;

            AEVec2 position = transform.GetPosition();
            position.x += speed * GetDirection().x * deltaTime;
            position.y += speed * GetDirection().y * deltaTime;

            transform.SetRotation(atan2f(GetDirection().y, GetDirection().x) + (3.14159f * 1.25f));
            transform.SetPosition(position.x, position.y);

            if (sprite) sprite->Update();

            // Spawn an Ominous Dark Purple Particle Trail
            AEVec2 trailScale = { transform.GetScale().x * 0.7f, transform.GetScale().y * 0.7f };
            ParticleManager::GetInstance()->SpawnTrail(
                transform.GetPosition(),
                trailScale,
                0.25f,
                0.5f, 0.0f, 0.8f
            );
        }
    }
}

void EnemyProjectile::Draw(AEGfxVertexList* mesh)
{
    if (sprite != nullptr && sprite->GetSpriteTexture() != nullptr)
    {
        AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
        sprite->Render(transform);
    }
    else
    {
        AEGfxSetRenderMode(AE_GFX_RM_COLOR);
        AEGfxTextureSet(nullptr, 0, 0);

        AEGfxSetBlendMode(AE_GFX_BM_ADD);
        AEGfxSetColorToMultiply(0.8f, 0.0f, 1.0f, 1.0f);
        AEGfxSetColorToAdd(0.4f, 0.0f, 0.6f, 1.0f);

        AEGfxSetTransform(transform.GetMatrix().m);
        AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
    }

    // --- THE UNIVERSAL CLEANUP BRUSH ---
    AEGfxTextureSet(nullptr, 0, 0);
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
    AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f); 

    AEMtx33 identity;
    AEMtx33Identity(&identity);
    AEGfxSetTransform(identity.m);
}

// Getters and Setters
bool EnemyProjectile::IsDead() { return currLifespan <= 0.0f; }
void EnemyProjectile::Speed(float newSpeed) { this->speed = newSpeed; }
float EnemyProjectile::Speed() const { return this->speed; }
float EnemyProjectile::OriginalSpeed() const { return this->oriSpeed; }