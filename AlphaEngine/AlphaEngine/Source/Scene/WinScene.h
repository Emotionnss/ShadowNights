#pragma once
#include "BaseScene.h"
#include "../System/Button.h"

class WinScene : public BaseScene {
public:
    void Load() override;
    void Init() override;
    void Update() override;
    void Draw() override;
    void Free() override;
    void Unload() override;

private:
    Button* btnMenu = nullptr;
};

