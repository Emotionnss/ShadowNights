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
    void Load() override;
    void Init() override;
    void Update() override;
    void Draw() override;
    void Free() override;
    void Unload() override;

private:
    Button* btnMenu = nullptr;
};

