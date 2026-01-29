#pragma once

#include "AEEngine.h"
#include "../Player/Player.h"
#include "../../Source/Scene/BaseScene.h"
#include "../System/MeshManager.h"
#include "../System/Map.h"
#include "../Abilities/AbilityHandler.h"

class TestScene : public BaseScene {
public:
    void Load() override;
    void Init() override;
    void Update() override;
    void Draw() override;
    void Free() override;
    void Unload() override;

private:
    // Constructor
    Player *player;
    AbilityHandler* abilityHandler = nullptr;;

    //float playerX, playerY;
    //float speed = 200.0f;
    MeshManager* meshManager = nullptr;
    Map* map = nullptr;
};
