#pragma once
#include "../../Source/Scene/BaseScene.h"
#include "../System/Button.h"

class MainMenu : public BaseScene {
public:
    void Load() override;
    void Init() override;
    void Update() override;
    void Draw() override;
    void Free() override;
    void Unload() override;

private:
    Button* btnPlay = nullptr;
    Button* btnSettings = nullptr;
    Button* btnQuit = nullptr;
};