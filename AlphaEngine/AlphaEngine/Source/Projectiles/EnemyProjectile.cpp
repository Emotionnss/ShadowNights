#include "EnemyProjectile.h"
#include "../System/CSVManager.h"
#include <cmath>
#include "../Player/Player.h"

EnemyProjectile::EnemyProjectile(AEVec2 pos, AEVec2 dir)
    : BaseProjectile(pos, dir, true)
{
    oriSpeed = 400.0f;
    speed = oriSpeed;
    
    transform.SetScale(40.0f, 40.0f);
    transform.SetPosition(pos.x, pos.y);
    SetLifespan(3.0f);

    sprite = new Sprite(CSVManager::ReadInfo(SPRITESHEET_DATA, "shadoworb"));
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
            float currentSpeed = slowed ? (oriSpeed * 0.5f) : oriSpeed;
            speed = currentSpeed;

            AEVec2 position = transform.GetPosition();
            position.x += currentSpeed * GetDirection().x * deltaTime;
            position.y += currentSpeed * GetDirection().y * deltaTime;

            transform.SetRotation(atan2f(GetDirection().y, GetDirection().x) + (3.14159f * 1.25f));
            transform.SetPosition(position.x, position.y);

            if (sprite) sprite->Update();
        }
    }
}

void EnemyProjectile::Draw()
{
    if (sprite) sprite->Render(transform);
}

bool EnemyProjectile::IsDead()
{
    return currLifespan <= 0.0f;
}

void EnemyProjectile::Speed(float newSpeed)
{
    this->speed = newSpeed;
}

float EnemyProjectile::Speed() const
{
    return this->speed;
}

float EnemyProjectile::OriginalSpeed() const
{
    return this->oriSpeed;
}