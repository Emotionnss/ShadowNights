#pragma once
#include "../../Source/Scene/BaseScene.h"
#include "../System/Button.h"

class LoseScene : public BaseScene {
public:
    void Load() override;
    void Init() override;
    void Update() override;
    void Draw() override;
    void Free() override;
    void Unload() override;

private:
    bool FileExists(int slot); // Checks if a save exists

    int menuPage = 0;

    Button* btnRestart = nullptr;
    Button* btnLoad = nullptr;
    Button* btnMenu = nullptr;

    Button* btnSlot1 = nullptr;
    Button* btnSlot2 = nullptr;
    Button* btnSlot3 = nullptr;
    Button* btnBack = nullptr;
};