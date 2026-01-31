#pragma once

#include "AEEngine.h"
#include "../Player/Player.h"
#include "../../Source/Scene/BaseScene.h"
#include "../System/MeshManager.h"
#include "../System/Map.h"
#include "../Abilities/AbilityHandler.h"
#include "../Enemy/BaseEnemy.h" 

class TestScene : public BaseScene {
public:
    void Load() override;
    void Init() override;
    void Update() override;
    void Draw() override;
    void Free() override;
    void Unload() override;

private:
    Player* player = nullptr;
    AbilityHandler* abilityHandler = nullptr;
    MeshManager* meshManager = nullptr;
    Map* map = nullptr;

    BaseEnemy* myEnemy = nullptr;
};