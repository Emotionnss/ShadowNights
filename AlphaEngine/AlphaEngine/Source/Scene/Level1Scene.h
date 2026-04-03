#pragma once
#include "BaseScene.h"
#include "BossScene.h"

/*!
@file       Level1Scene.h
@author     Lim Chee Kang (l.cheekang)
@brief      This file contains definitions of functions and variables
            used for the Level1Scene class, which is the first level of the game,
            and contains the specific implementation of the scene, such as the
            enemies that spawn and the map used
*//*_______________________________________________________________________*/

class Level1Scene : public BaseScene {
public:
    /*!
    @brief    Constructor for Level1Scene, sets up map names, music, and the next scene.
    @param    slot The save slot to load from, defaults to 0.
    *//*______________________________________________________________*/
    Level1Scene(int slot = 0) : BaseScene(slot) {
        levelMapName = "Level1";
        levelBGM = "bgm_level1";

        nextScene = new BossScene();

        levelID = 1;
    }
};