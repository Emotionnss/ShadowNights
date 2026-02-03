#pragma once
#include "../Player/GameObject.h"
#include "../System/CSVManager.h"
#include "../Sprite/Sprite.h" // Ensure Sprite is included

class EnemyProjectile : public GameObject
{
public:
    // Constructor: Spawns projectile at position, moving in direction (1 or -1)
    EnemyProjectile(AEVec2 startPos, float directionX)
    {
        // 1. Setup Position
        transform.SetPosition(startPos.x, startPos.y);
        transform.SetScale(40.0f, 40.0f); // Size of projectile visuals

        // 2. Setup Movement (Speed = 400)
        float speed = 400.0f;
        velocity.x = (directionX > 0) ? speed : -speed;
        velocity.y = 0.0f;

        // 3. Setup Sprite & Animation
        sprite = new Sprite(CSVManager::ReadInfo(SPRITESHEET_DATA, "shadoworb"));

        // Play the default animation (infinite loop)
        sprite->PlayDefaultAnimation(-1, 1.0f);
    }

    ~EnemyProjectile()
    {
        if (sprite) {
            delete sprite;
            sprite = nullptr;
        }
    }

    void Update(float dt) override
    {
        // Move Object
        AEVec2 pos = transform.GetPosition();
        pos.x += velocity.x * dt;
        transform.SetPosition(pos.x, pos.y);

        // Update Animation
        if (sprite) sprite->Update();

        // Lifespan Logic
        lifespan -= dt;
    }

    void Draw() override
    {
        if (sprite) sprite->Render(transform);
    }

    bool IsDead() { return lifespan <= 0.0f; }

private:
    float lifespan = 3.0f; // Disappears after 3 seconds
};