#pragma once
#include "../../Source/Scene/BaseScene.h"
#include "../System/Button.h"
#include "../System/AudioManager.h"

class MainMenu : public BaseScene {
public:
    void Load() override;
    void Init() override;
    void Update() override;
    void Draw() override;
    void Free() override;
    void Unload() override;

private:
    void DeleteSave(int slot);

    Button* btnPlay = nullptr;
    Button* btnLoad = nullptr;
    Button* btnSettings = nullptr;
    Button* btnCredits = nullptr;
    Button* btnQuit = nullptr;
};