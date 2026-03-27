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
    LoseScene(int idToRestart = 1);

    void Load() override;
    void Init() override;
    void Update() override;
    void Draw() override;
    void Free() override;
    void Unload() override;

private:
    Button* btnRestart = nullptr;
    Button* btnLoad = nullptr;
    Button* btnMenu = nullptr;

    int restartLevelID;
};