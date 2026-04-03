#pragma once
#include "../../Source/Scene/BaseScene.h"
#include "../System/Button.h"
#include "../System/AudioManager.h"

/*!
@file       MainMenu.h
@author     Lim Chee Kang (l.cheekang)
@brief      This file contains definitions of functions and variables
            used for the MainMenu class, which is the first scene of the game,
            and contains buttons to start a new game, load from a save, access settings,
            view credits, or quit the game
*//*_______________________________________________________________________*/

class MainMenu : public BaseScene {
public:
    /*!
    @brief    Loads the menu assets, UI buttons, and background music.
    *//*______________________________________________________________*/
    void Load() override;

    /*!
    @brief    Initializes menu states and animation timers.
    *//*______________________________________________________________*/
    void Init() override;

    /*!
    @brief    Handles button interactions and UI slide-in animations.
    *//*______________________________________________________________*/
    void Update() override;

    /*!
    @brief    Renders the background, logo, and active menu buttons.
    *//*______________________________________________________________*/
    void Draw() override;

    /*!
    @brief    Frees dynamically allocated memory for the menu buttons.
    *//*______________________________________________________________*/
    void Free() override;

    /*!
    @brief    Unloads the logo texture and other menu-specific resources.
    *//*______________________________________________________________*/
    void Unload() override;

private:
    /*!
    @brief    Deletes the physical save file from the directory.
    @param    slot The save file index to delete.
    *//*______________________________________________________________*/
    void DeleteSave(int slot);

    Button* btnPlay = nullptr;
    Button* btnLoad = nullptr;
    Button* btnSettings = nullptr;
    Button* btnCredits = nullptr;
    Button* btnQuit = nullptr;
    Button* btnYes = nullptr;
    Button* btnNo = nullptr;

    AEGfxTexture* logo{};
    Transform2D logoTfm{};
    float animT = 0.0f;
};