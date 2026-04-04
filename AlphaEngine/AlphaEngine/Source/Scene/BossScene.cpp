/*!
@file       BossScene.cpp
@author     Yap Yu Xuan, Daevon (yuxuandaevon.yap), Bay Yeok Hao (yeokhao.bay)
@brief      This file contains the definition of the BossScene class, which
            implements the boss encounter level.
*//*_______________________________________________________________________*/

#include "BossScene.h"
#include "../System/Scenemanager.h"
#include "WinScene.h" 

void BossScene::Init() {
    BaseScene::Init();
    player->GetTransform().SetPosition(300.0f, 200.0f);
    player->SetMoveable(false);
    abilityHandler->ShowUI(false);

    // Disable the pause menu so they can't interrupt the cutscene!
    canPause = false;

    boss = new BossEnemy();
    boss->Init(1400.f, 300.0f, 85.0f, 60.0f, "boss");
    enemyList.push_back(boss);
    boss->isBoss = true;
    levelID = 2;
    //AudioManager::GetInstance()->PlaySFXOneShot("");
    boss->SpawnHealthBar();
}

void BossScene::Update() {
    // drain pending enemy spawns into enemylist of scene BEFORE scene update
    if (boss)
    {
        for (BaseEnemy* e : boss->GetPendingSpawns())
            enemyList.push_back(e);
        boss->GetPendingSpawns().clear();
    }

    BaseScene::Update();

    // During the cutscene, the camera will be locked onto the boss and the player cannot move until the cutscene ends. Once the cutscene ends, control is returned to the player and the UI is shown again.
    if (!isPaused) {
        // Cutscene timer logic
        f32 dt = (f32)AEFrameRateControllerGetFrameTime();

        // If the cutscene duration is greater than or equal to 0 and the boss exists, lock the camera onto the boss's position and decrement the cutscene timer by the delta time. Once the cutscene timer reaches 0, allow the player to move again and show the UI.
        if (cutsceneDuration >= 0.f && boss)
        {
            AEGfxSetCamPosition(boss->transform.GetPosition().x, boss->transform.GetPosition().y);
            cutsceneDuration -= dt;

            // During the cutscene, we want to make sure the player cannot move and the UI is hidden, so we set those states every frame just in case
            if (cutsceneDuration < 0.f)
            {
                player->SetMoveable(true);
                abilityHandler->ShowUI(true);

                // The cutscene is over, the player is allowed to pause again!
                canPause = true;
            }
        }

        // If the boss exists and is ready to be deleted (which means its death animation has finished), transition to the WinScene
        if (boss && boss->IsReadyToDelete()) {
            SceneManager::ChangeScene(new WinScene());
            return;
        }
    }
}

void BossScene::Draw() {
    AEGfxSetBackgroundColor(0.2f, 0.0f, 0.0f);
    BaseScene::Draw();
}

void BossScene::Free() {
    player->SetMoveable(true);
    abilityHandler->ShowUI(true);
    BaseScene::Free();
    boss = nullptr;
}