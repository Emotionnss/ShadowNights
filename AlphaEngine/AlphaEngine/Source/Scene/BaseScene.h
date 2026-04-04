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
    /*!
    @brief    Constructor for BaseScene.
    @param    slot The save slot to load from, defaults to 0 (new game).
    *//*______________________________________________________________*/
    BaseScene(int slot = 0);

    /*!
    @brief    Virtual destructor to ensure proper cleanup of derived scenes.
    *//*______________________________________________________________*/
    virtual ~BaseScene();

    /*!
    @brief    Loads the necessary assets, background, and UI for the scene.
    *//*______________________________________________________________*/
    virtual void Load();

    /*!
    @brief    Initializes scene variables, entities, and camera positions.
    *//*______________________________________________________________*/
    virtual void Init();

    /*!
    @brief    Updates scene logic, entity behaviors, and UI interactions.
    *//*______________________________________________________________*/
    virtual void Update();

    /*!
    @brief    Renders all background elements, entities, and UI to the screen.
    *//*______________________________________________________________*/
    virtual void Draw();

    /*!
    @brief    Frees dynamically allocated memory used during the scene.
    *//*______________________________________________________________*/
    virtual void Free();

    /*!
    @brief    Unloads assets and textures from the graphics memory.
    *//*______________________________________________________________*/
    virtual void Unload();

    /*!
    @brief    Saves the player's current progress to a specified slot.
    @param    slot The save file index to write to (1, 2, or 3).
    *//*______________________________________________________________*/
    void SaveGame(int slot);

    /*!
    @brief    Loads the player's saved progress from a specified slot.
    @param    slot The save file index to read from (1, 2, or 3).
    *//*______________________________________________________________*/
    void LoadGame(int slot);

    /*!
    @brief    Checks if a valid save file exists for the given slot.
    @param    slot The save file index to check.
    @return   True if the file exists, false otherwise.
    *//*______________________________________________________________*/
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
	int menuPage = 0; //Used to determine which menu page the player is currently on when the game is paused (0 = main pause menu, 1 = save menu, 2 = load menu, 3 = settings menu, 4 = confirmation prompt)
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