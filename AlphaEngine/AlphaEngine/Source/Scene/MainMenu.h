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
    bool FileExists(int slot);
    void DeleteSave(int slot);

    int menuPage = 0;

    Button* btnPlay = nullptr;
    Button* btnLoad = nullptr;
    Button* btnSettings = nullptr;
    Button* btnQuit = nullptr;

    Button* btnSlot1 = nullptr;
    Button* btnSlot2 = nullptr;
    Button* btnSlot3 = nullptr;
    Button* btnBack = nullptr;
};