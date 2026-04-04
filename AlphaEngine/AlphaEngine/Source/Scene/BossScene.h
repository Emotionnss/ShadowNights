/*!
@file       BossScene.h
@author     Yap Yu Xuan, Daevon (yuxuandaevon.yap), Bay Yeok Hao (yeokhao.bay)
@brief      This file contains the declaration of the BossScene class, which
            implements the boss encounter level.
*//*_______________________________________________________________________*/

#pragma once
#include "BaseScene.h"
#include "../Enemy/BossEnemy.h"

class BossScene : public BaseScene {
public:
    /*!
    @brief    Constructs a BossScene and configures default map, music, and level ID.
    @param    slot The save slot to load from, defaults to 0.
    *//*______________________________________________________________*/
    BossScene(int slot = 0) : BaseScene(slot) {
        levelMapName = "Bossmap";
        levelBGM = "boss_BGM";
        levelID = 2;
    }

    /*!
    @brief    Initializes scene-specific resources and state (called after Load).
    @param    void
    @return   void
    *//*______________________________________________________________*/
    void Init() override;

    /*!
    @brief    Updates boss logic, cutscene timers, and scene entities each frame.
    @param    void
    @return   void
    *//*______________________________________________________________*/
    void Update() override;

    /*!
    @brief    Renders background, boss, enemies, UI and any cutscene visuals.
    @param    void
    @return   void
    *//*______________________________________________________________*/
    void Draw() override;

    /*!
    @brief    Frees dynamically allocated memory and scene-specific resources.
    @param    void
    @return   void
    *//*______________________________________________________________*/
    void Free() override;

private:
    BossEnemy* boss = nullptr;
    float cutsceneDuration = 3.f;
};