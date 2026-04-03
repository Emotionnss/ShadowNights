#pragma once
#include "BaseScene.h"
#include "../System/Button.h"

/*!
@file       WinScene.h
@author     Lim Chee Kang (l.cheekang)
@brief      This file contains definitions of functions and variables
             used for the WinScene class, which is the scene that is shown
             when the player defeats the boss, and contains a button to return
             to the main menu.
*//*_______________________________________________________________________*/

class WinScene : public BaseScene {
public:
    /*!
    @brief    Loads the victory background, music, and return button.
    *//*______________________________________________________________*/
    void Load() override;

    /*!
    @brief    Initializes the win screen properties.
    *//*______________________________________________________________*/
    void Init() override;

    /*!
    @brief    Checks for button clicks to return to the Main Menu.
    *//*______________________________________________________________*/
    void Update() override;

    /*!
    @brief    Renders the victory message and the background.
    *//*______________________________________________________________*/
    void Draw() override;

    /*!
    @brief    Frees memory used by the victory screen UI.
    *//*______________________________________________________________*/
    void Free() override;

    /*!
    @brief    Unloads the victory scene assets.
    *//*______________________________________________________________*/
    void Unload() override;

private:
    Button* btnMenu = nullptr;
};