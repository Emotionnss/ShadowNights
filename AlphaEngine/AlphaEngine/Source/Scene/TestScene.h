#pragma once

#include "AEEngine.h"
#include "../Player/Player.h"
#include "../../Source/Scene/BaseScene.h"
#include "../System/MeshManager.h"
#include "../System/Map.h"
#include "../Abilities/AbilityHandler.h"
#include "../Enemy/BaseEnemy.h"
#include "background.h"
#include "../System/Button.h" 

class TestScene : public BaseScene {
public:
    TestScene(int slot = 0) : loadFromSlot(slot) {}

    void Load() override;
    void Init() override;
    void Update() override;
    void Draw() override;
    void Free() override;
    void Unload() override;

    void SaveGame(int slot);
    void LoadGame(int slot);
    bool FileExists(int slot) const;

private:
    int loadFromSlot = 0;

    Player* player = nullptr;
    AbilityHandler* abilityHandler = nullptr;
    MeshManager* meshManager = nullptr;
    Map* map = nullptr;
    AEVec2 DoorTilePosition = { 0.0f, 0.0f };
    Background* bg = nullptr;

    std::vector<BaseEnemy*> enemyList;

    bool isPaused = false;
    int menuPage = 0; // 0 = Main Pause, 1 = Save Menu, 2 = Load Menu
    float popupTimer = 0.0f;

    // Main Menu Buttons
    Button* btnResume = nullptr;
    Button* btnSaveMenu = nullptr;
    Button* btnLoadMenu = nullptr;
    Button* btnExit = nullptr;

    // Slot Menu Buttons
    Button* btnSlot1 = nullptr;
    Button* btnSlot2 = nullptr;
    Button* btnSlot3 = nullptr;
    Button* btnBack = nullptr;
};