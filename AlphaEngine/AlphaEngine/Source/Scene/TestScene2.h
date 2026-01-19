#pragma once

#include "AEEngine.h"
#include "../../Source/Scene/BaseScene.h"
#include "../System/MeshManager.h"

class TestScene2 : public BaseScene {
public:
    void Load() override;
    void Init() override;
    void Update() override;
    void Draw() override;
    void Free() override;
    void Unload() override;

private:
    float playerX, playerY;
    float speed = 200.0f;

    MeshManager* meshManager = nullptr;
};

