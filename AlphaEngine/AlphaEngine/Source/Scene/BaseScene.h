#pragma once

#include "AEEngine.h"
#include "../Player/Player.h"
#include "../System/MeshManager.h"
#include "../System/Map.h"
#include "../Abilities/AbilityHandler.h"
#include "../Enemy/BaseEnemy.h"
#include "background.h"
#include "../System/Button.h"
#include "../System/AudioManager.h"
#include <vector>
#include <string>

/*!
@file       BaseScene.h
@author     Lim Chee Kang (l.cheekang)
@brief      This file contains definitions of functions and variables
            used for the BaseScene class, which is the parent class
            of all scenes in the game, and contains common variables and
			functions used across all scenes
*//*_______________________________________________________________________*/

class BaseScene {
public:
    BaseScene(int slot = 0);

    virtual ~BaseScene();

    virtual void Load();
    virtual void Init();
    virtual void Update();
    virtual void Draw();
    virtual void Free();
    virtual void Unload();

    void SaveGame(int slot);
    void LoadGame(int slot);
    bool FileExists(int slot) const;

protected:
    std::string levelMapName = "Level1";
    std::string levelBGM = "bgm_level1";

    BaseScene* nextScene = nullptr;

    int loadFromSlot = 0;

    Player* player = nullptr;
    AbilityHandler* abilityHandler = nullptr;
    MeshManager* meshManager = nullptr;
    Map* map = nullptr;
    AudioManager* audio = nullptr;

    AEVec2 DoorTilePosition = { 0.0f, 0.0f };
    Background* bg = nullptr;

    std::vector<BaseEnemy*> enemyList;

    bool isPaused = false;
    int menuPage = 0;
    float popupTimer = 0.0f;

    int levelID = 1;

    Button* btnResume = nullptr;
    Button* btnSaveMenu = nullptr;
    Button* btnLoadMenu = nullptr;
    Button* btnExit = nullptr;
    Button* btnSettings = nullptr;

    Button* btnSlot1 = nullptr;
    Button* btnSlot2 = nullptr;
    Button* btnSlot3 = nullptr;
    Button* btnBack = nullptr;
    Button* btnYes = nullptr;
    Button* btnNo = nullptr;
    Button* btnToggleMute = nullptr;
    Button* btnToggleFullscreen = nullptr;
    Button* btnSettingsBack = nullptr;
    Button* btnToggleCameraShake = nullptr;
};