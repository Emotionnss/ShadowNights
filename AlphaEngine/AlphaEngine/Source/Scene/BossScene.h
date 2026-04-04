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

    /*!
    @brief    Saves a complete "snapshot" of the boss encounter to a file slot.

    This override first calls BaseScene::SaveGame to handle player health and mana,
    then appends boss-specific data to the file. It records the exact positions
    of all active lasers, explosives, and projectiles, along with the Boss's
    current AI state and weakpoint timer.

    @param    slot The save file index (1, 2, or 3) to append the boss data to.
    *//*______________________________________________________________*/
    void SaveGame(int slot) override;

    /*!
    @brief    Loads a boss encounter snapshot and reconstructs the active battle.

    This function restores the fight to its exact saved state. It re-spawns
    all projectiles and lasers at their saved coordinates and manually
    resyncs the Boss's animations and AI phases to ensure the fight resumes
    seamlessly without graphical glitches.

    @param    slot The save file index to retrieve the snapshot from.
    *//*______________________________________________________________*/
    void LoadGame(int slot) override;

private:
    BossEnemy* boss = nullptr;

    // Controls how long the camera locks onto the boss before the fight begins
    float cutsceneDuration = 3.f;
};