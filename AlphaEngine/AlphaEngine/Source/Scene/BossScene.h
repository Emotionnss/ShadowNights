#pragma once
#include "BaseScene.h"
#include "../Enemy/BossEnemy.h"

class BossScene : public BaseScene {
public:
    BossScene(int slot = 0) : BaseScene(slot) {
        levelMapName = "Bossmap";
        levelBGM = "boss_BGM";
        levelID = 2;
    }

    

    void Init() override;
    void Update() override;
    void Draw() override;
    void Free() override;

private:
    BossEnemy* boss = nullptr;

    float cutsceneDuration = 3.f;
};