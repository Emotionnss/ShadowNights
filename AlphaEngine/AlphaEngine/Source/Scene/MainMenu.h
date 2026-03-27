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
    void Load() override;
    void Init() override;
    void Update() override;
    void Draw() override;
    void Free() override;
    void Unload() override;

private:
    void DeleteSave(int slot);

    Button* btnPlay = nullptr;
    Button* btnLoad = nullptr;
    Button* btnSettings = nullptr;
    Button* btnCredits = nullptr;
    Button* btnQuit = nullptr;
};