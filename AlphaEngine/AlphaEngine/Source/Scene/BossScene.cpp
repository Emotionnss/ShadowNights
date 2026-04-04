/*!
@file       BossScene.cpp
@author     Yap Yu Xuan, Daevon (yuxuandaevon.yap), Bay Yeok Hao (yeokhao.bay)
@brief      This file contains the definition of the BossScene class, which
            implements the boss encounter level.
*//*_______________________________________________________________________*/

#include "BossScene.h"
#include "../System/Scenemanager.h"
#include "WinScene.h"
#include <fstream>
#include "../Sprite/Sprite.h"

void BossScene::Init() {
    bool isLoading = (loadFromSlot > 0);

    BaseScene::Init();
    levelID = 2;

    if (isLoading) {
        // If we loaded a save file mid-fight, the intro cutscene is skipped completely.
        cutsceneDuration = -1.f;
        canPause = true;

        // We set boss to nullptr here because BaseScene::LoadGame deletes old enemies.
        // The pointer will safely reconnect to the newly loaded boss in Update() on Frame 1.
        boss = nullptr;
    }
    else {
        player->GetTransform().SetPosition(300.0f, 200.0f);

        // Freeze the player and hide UI so the cinematic can play
        player->SetMoveable(false);
        if (abilityHandler) abilityHandler->ShowUI(false);

        // Disable the pause menu so they can't interrupt the cutscene timers
        canPause = false;

        boss = new BossEnemy();
        boss->Init(1400.f, 300.0f, 85.0f, 60.0f, "boss");
        enemyList.push_back(boss);
        boss->isBoss = true;
        boss->SpawnHealthBar();
    }
}

void BossScene::Update() {

    bool bossFound = false;
    BossEnemy* activeBoss = nullptr;

    for (BaseEnemy* e : enemyList) {
        if (e->isBoss) {
            bossFound = true;
            activeBoss = dynamic_cast<BossEnemy*>(e);
            break;
        }
    }

    if (boss != nullptr && !bossFound) {
        SceneManager::ChangeScene(new WinScene());
        return;
    }

    // Safely update the class pointer for this frame
    boss = activeBoss;

    if (boss) {
        for (BaseEnemy* e : boss->GetPendingSpawns()) {
            enemyList.push_back(e);
        }
        boss->GetPendingSpawns().clear();
    }

    BaseScene::Update();

    if (!isPaused) {
        f32 dt = (f32)AEFrameRateControllerGetFrameTime();

        // If the timer is running and the boss exists, play the cinematic!
        if (cutsceneDuration >= 0.f && boss) {
            // Force the camera to stay locked onto the boss
            AEGfxSetCamPosition(boss->transform.GetPosition().x, boss->transform.GetPosition().y);
            cutsceneDuration -= dt;

            // The moment the timer hits 0, the cutscene is over.
            if (cutsceneDuration < 0.f) {
                player->SetMoveable(true);
                if (abilityHandler) abilityHandler->ShowUI(true);
                canPause = true;
            }
        }
    }
}

void BossScene::Draw() {
    AEGfxSetBackgroundColor(0.2f, 0.0f, 0.0f);
    BaseScene::Draw();
}

void BossScene::Free() {
    player->SetMoveable(true);
    if (abilityHandler) abilityHandler->ShowUI(true);

    BaseScene::Free();
    boss = nullptr;
}

void BossScene::SaveGame(int slot) {
    // Let BaseScene save Player pos, Mana, and Boss HP
    BaseScene::SaveGame(slot);

    std::string path = "save_slot_" + std::to_string(slot) + ".txt";
    std::ofstream f(path, std::ios::app); // Append to the end of the file
    if (f.is_open()) {
        f << "BOSS_STATE\n"; // Marker for the loader

        if (boss) {
            // Save Shadow Orbs (Standard Projs)
            std::vector<EnemyProjectile*>& projs = boss->GetProjectiles();
            f << projs.size() << "\n";
            for (EnemyProjectile* proj : projs) {
                if (proj) {
                    AEVec2 pPos = proj->transform.GetPosition();
                    AEVec2 pDir = proj->GetDirection();
                    f << pPos.x << " " << pPos.y << " " << pDir.x << " " << pDir.y << " "
                        << proj->GetLifespan() << " " << proj->isHoming << "\n";
                }
            }

            // Save Lasers and Explosives
            f << boss->bossLasers.size() << "\n";
            for (BossLaser* laser : boss->bossLasers) laser->SaveState(f);

            f << boss->bossExplosives.size() << "\n";
            for (BossExplosive* explosive : boss->bossExplosives) explosive->SaveState(f);

            // Save AI State & Weakpoint
            f << (int)boss->currentBossState << " " << boss->timer << " " << boss->weakpoint.active << "\n";
        }
        f.close();
    }
}

void BossScene::LoadGame(int slot) {
    // Let BaseScene reload the world
    BaseScene::LoadGame(slot);

    // Re-find the boss immediately so we can apply data to it
    boss = nullptr;
    for (BaseEnemy* e : enemyList) {
        if (e->isBoss) {
            boss = dynamic_cast<BossEnemy*>(e);
            break;
        }
    }

    std::string path = "save_slot_" + std::to_string(slot) + ".txt";
    std::ifstream f(path);
    if (f.is_open() && boss) {
        std::string line;
        bool foundBossData = false;

        while (f >> line) {
            if (line == "BOSS_STATE") {
                foundBossData = true;
                break;
            }
        }

        if (foundBossData) {
            //Rebuild Shadow Orbs
            int numProjs = 0;
            if (f >> numProjs) {
                std::vector<EnemyProjectile*>& projs = boss->GetProjectiles();
                for (int i = 0; i < numProjs; i++) {
                    float px, py, dx, dy, life; bool homing;
                    f >> px >> py >> dx >> dy >> life >> homing;
                    EnemyProjectile* newProj = new EnemyProjectile({ px, py }, { dx, dy });
                    newProj->SetLifespan(life);
                    newProj->isHoming = homing;
                    projs.push_back(newProj);
                }
            }

            // Rebuild Lasers and Explosives
            int numLasers = 0;
            if (f >> numLasers) {
                for (int i = 0; i < numLasers; i++) {
                    BossLaser* newLaser = new BossLaser();
                    newLaser->LoadState(f);
                    boss->bossLasers.push_back(newLaser);
                }
            }

            int numExplosives = 0;
            if (f >> numExplosives) {
                for (int i = 0; i < numExplosives; i++) {
                    BossExplosive* newExplosive = new BossExplosive();
                    newExplosive->LoadState(f);
                    boss->bossExplosives.push_back(newExplosive);
                }
            }

            //Restore the weakpoint
            int stateInt;
            bool wpActive;
            if (f >> stateInt >> boss->timer >> wpActive) {
                boss->currentBossState = (BOSS_STATE)stateInt;
                boss->weakpoint.active = wpActive;

				// Finally, set the correct animation based on the loaded state so it doesn't look weird if we load mid-animation
                if (boss->sprite) {
                    if (boss->currentBossState == BOSS_STATE::MOVING) {
                        boss->sprite->PlayAnimation("move", -1, 1.0f);
                    }
                    else if (boss->currentBossState == BOSS_STATE::IDLING ||
                        boss->currentBossState == BOSS_STATE::EXHAUSTED) {
                        boss->sprite->PlayAnimation("idle", -1, 1.5f);
                    }
                }
            }
        }
        f.close();
    }
}