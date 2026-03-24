#pragma once
#include "../../Source/Scene/BaseScene.h"
#include "../System/Button.h"

class LoseScene : public BaseScene {
public:
    LoseScene(int idToRestart = 1);

    void Load() override;
    void Init() override;
    void Update() override;
    void Draw() override;
    void Free() override;
    void Unload() override;

private:
    Button* btnRestart = nullptr;
    Button* btnLoad = nullptr;
    Button* btnMenu = nullptr;

    int restartLevelID;
};