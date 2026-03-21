#pragma once
#include "BaseScene.h"
#include "BossScene.h"

class Level1Scene : public BaseScene {
public:
    Level1Scene(int slot = 0) : BaseScene(slot) {
        levelMapName = "Level1";
        levelBGM = "bgm_level1";

        nextScene = new BossScene();
    }
};