#pragma once
#include "../../Source/Scene/BaseScene.h"
#include "../System/Button.h"

/*!
@file       LoseScene.h
@author     Lim Chee Kang (l.cheekang)
@brief      This file contains definitions of functions and variables
            used for the LoseScene class, which is the scene that is shown
            when the player dies, and contains buttons to restart the level,
            load from a save, or return to the main menu
*//*_______________________________________________________________________*/

class LoseScene : public BaseScene {
public:
    /*!
    @brief    Constructor for LoseScene that tracks which level to reload.
    @param    idToRestart The ID of the level the player died in (Defaults to 1).
    *//*______________________________________________________________*/
    LoseScene(int idToRestart = 1);

    /*!
    @brief    Loads the game over music, background, and menu buttons.
    *//*______________________________________________________________*/
    void Load() override;

    /*!
    @brief    Initializes the lose screen menu state.
    *//*______________________________________________________________*/
    void Init() override;

    /*!
    @brief    Handles UI interactions for retrying, loading, or quitting.
    *//*______________________________________________________________*/
    void Update() override;

    /*!
    @brief    Renders the game over background, buttons, and "YOU DIED" text.
    *//*______________________________________________________________*/
    void Draw() override;

    /*!
    @brief    Frees memory allocated for the lose screen buttons.
    *//*______________________________________________________________*/
    void Free() override;

    /*!
    @brief    Unloads resources specific to the game over screen.
    *//*______________________________________________________________*/
    void Unload() override;

private:
    Button* btnRestart = nullptr;
    Button* btnLoad = nullptr;
    Button* btnMenu = nullptr;

    int restartLevelID;
};