#pragma once
#include "../Player/Player.h"
#include "../Enemy/BaseEnemy.h"
#include "../System/MeshManager.h"
#include "../Sprite/Sprite.h"
class TimeSlow
{
private:
    Player* player = nullptr;
    bool active{};
    bool startDespawn{};
    Sprite* timeSlowSprite = nullptr;
    std::string timeSlow{};

    f32 despawningTime{};
    f32 despawningTimer{};
    Transform2D timeSlowTransform{};//transform the sprite
public:
    TimeSlow();
    ~TimeSlow();
    void Update(float dt);
    bool ToggleSlow(std::vector<BaseEnemy*> enemyList);
    void Draw();
};