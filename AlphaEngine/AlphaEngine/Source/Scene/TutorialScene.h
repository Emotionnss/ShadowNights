#pragma once
#include "BaseScene.h"
#include "Level1Scene.h"

class TutorialScene : public BaseScene {
public:
    TutorialScene(int slot = 0) : BaseScene(slot) {
        levelMapName = "TutorialLevel";
        levelBGM = "bgm_level1";
        nextScene = new Level1Scene();
    }
};