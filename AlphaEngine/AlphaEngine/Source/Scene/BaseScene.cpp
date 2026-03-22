#include "BaseScene.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include "../System/Scenemanager.h"
#include "BossScene.h"
#include "MainMenu.h"
#include "LoseScene.h"
#include "../Utils/utils.h"
#include "../Enemy/ShadowEnemy.h"
#include "../Enemy/BatEnemy.h"
#include "../Enemy/ArcherEnemy.h"
#include "../particle/ParticleManager.h"
#include "../System/SettingsManager.h"

extern void ShutdownSwordResources();
extern void ShutdownKnifeResources();

BaseScene::BaseScene(int slot) : loadFromSlot(slot) {}

BaseScene::~BaseScene() {
    if (nextScene != nullptr) {
        delete nextScene;
        nextScene = nullptr;
    }
}

void BaseScene::Load() {
    AEGfxReset();
    meshManager = MeshManager::GetInstance();
    map = Map::GetInstance();
    player = Player::GetInstance();
    audio = AudioManager::GetInstance();

    bg = new Background;
    map->LoadMap(levelMapName);

    for (s32 i = 0; i < map->GetNumRow(); ++i) {
        for (s32 j = 0; j < map->GetNumCol(); ++j) {
            if (map->GetMap()[i][j].tileType == TILE_TYPE::CLOSED_DOOR) {
                AEVec2Set(&DoorTilePosition, static_cast<f32>(i), static_cast<f32>(j));
                break;
            }
        }
    }

    Button::LoadResources();
    btnResume = new Button(0.0f, 0.0f, 200.0f, 50.0f, "Resume");
    btnSaveMenu = new Button(0.0f, 0.0f, 200.0f, 50.0f, "Save Game");
    btnLoadMenu = new Button(0.0f, 0.0f, 200.0f, 50.0f, "Load Game");
    btnExit = new Button(0.0f, 0.0f, 200.0f, 50.0f, "Quit to Menu");

    btnSlot1 = new Button(0.0f, 0.0f, 200.0f, 50.0f, "Slot 1");
    btnSlot2 = new Button(0.0f, 0.0f, 200.0f, 50.0f, "Slot 2");
    btnSlot3 = new Button(0.0f, 0.0f, 200.0f, 50.0f, "Slot 3");
    btnBack = new Button(0.0f, 0.0f, 200.0f, 50.0f, "Back");
}

void BaseScene::Init() {
    player->GetTransform().SetPosition(300.0f, 900.0f);
    audio->PlayMusic(levelBGM, 1.f);
    abilityHandler = new AbilityHandler();

    isPaused = false;
    menuPage = 0;
    popupTimer = 0.0f;

    s32 maxRows = map->GetNumRow();
    for (s32 i = 0; i < maxRows; ++i) {

        s32 maxCols = map->GetNumCol();
        for (s32 j = 0; j < maxCols; ++j) {

            AEVec2 pos = map->GetTileWorldPos(map->GetTilePos(map->GetMap()[i][j].transform.GetPosition().x, map->GetMap()[i][j].transform.GetPosition().y));

            if (map->GetMap()[i][j].tileType == TILE_TYPE::BAT_ENEMY) {
                BaseEnemy* batEnemy = new BatEnemy();
                batEnemy->Init(pos.x, pos.y, 85.f, 60.f, "batenemy");
                batEnemy->SetVelocity(0, 0);
                enemyList.push_back(batEnemy);
                map->GetMap()[i][j].tileType = TILE_TYPE::NONE;
            }
            else if (map->GetMap()[i][j].tileType == TILE_TYPE::ARCHER_ENEMY) {
                BaseEnemy* archerEnemy = new ArcherEnemy();
                archerEnemy->Init(pos.x, pos.y, 85.f, 60.f, "sniperenemy");
                archerEnemy->SetVelocity(0, 0);
                enemyList.push_back(archerEnemy);
                map->GetMap()[i][j].tileType = TILE_TYPE::NONE;
            }
            else if (map->GetMap()[i][j].tileType == TILE_TYPE::SHADOW_ENEMY) {
                BaseEnemy* shadowEnemy = new ShadowEnemy();
                shadowEnemy->Init(pos.x, pos.y, 85.f, 60.f, "shadow1");
                shadowEnemy->SetVelocity(0, 0);
                enemyList.push_back(shadowEnemy);
                map->GetMap()[i][j].tileType = TILE_TYPE::NONE;
            }
        }
    }

    if (loadFromSlot > 0) {
        LoadGame(loadFromSlot);
        loadFromSlot = 0;
    }
}

void BaseScene::Update() {
    if (AEInputCheckTriggered(AEVK_ESCAPE)) {
        isPaused = !isPaused;
        menuPage = 0;
    }

    f32 dt = (f32)AEFrameRateControllerGetFrameTime();

    if (!isPaused) {
        audio->ResumeMusic();
        player->Update(dt);
        ParticleManager::GetInstance()->Update(dt);
        if (abilityHandler) abilityHandler->AbilityUpdate(enemyList, dt);

        for (BaseEnemy* enemy : enemyList) {
            enemy->Update(dt);
        }

        if (enemyList.size() == 0&&map->GetMap()[(int)DoorTilePosition.x][(int)DoorTilePosition.y].tileType == TILE_TYPE::CLOSED_DOOR) {
            AudioManager::GetInstance()->PlaySFXOneShot("door_open");
            map->SetTile(TILE_TYPE::OPEN_DOOR, DoorTilePosition.y, DoorTilePosition.x);
        }

        // DOOR COLLISION CHECK
        if (player->GetCollidedXTile().tileType == TILE_TYPE::OPEN_DOOR) {
            if (nextScene != nullptr) {
                BaseScene* pNext = nextScene;
                nextScene = nullptr;
                SceneManager::ChangeScene(pNext);
            }
            else {
                SceneManager::ChangeScene(new MainMenu());
            }
        }

        if (AEInputCheckTriggered(AEVK_0)) {
            SceneManager::ChangeScene(new BossScene());
        }

        for (BaseEnemy* enemy : enemyList) {
            std::vector<EnemyProjectile*>& enemyProjs = enemy->GetProjectiles();
            for (auto it = enemyProjs.begin(); it != enemyProjs.end(); ) {
                EnemyProjectile* proj = *it;

                AEVec2 projPos = proj->transform.GetPosition();
                AEVec2 projHalf = { 20.0f, 20.0f };
                AEVec2 playerPos = player->GetTransform().GetPosition();
                AEVec2 playerHalf = { 35.0f, 35.0f };

                if (AABBOverlap(projPos, projHalf, playerPos, playerHalf) && !abilityHandler->Reverse()) {
                    player->TakeDamage(3);
                    delete proj;
                    it = enemyProjs.erase(it);
                }
                else {
                    ++it;
                }
            }
        }

        if (player->GetHealth() <= 0) {
            if (abilityHandler->Anchored()) {
                abilityHandler->TriggerAnchor();
            }
            else {
                player->SetHealth(100);
            }
        }

        std::vector<BaseProjectile*>& projs = player->projectileList;
        for (auto it = projs.begin(); it != projs.end(); ) {
            BaseProjectile* pProj = *it;
            AEVec2 projPos = pProj->transform.GetPosition();
            AEVec2 projHalf = pProj->transform.GetScale();
            AEVec2Scale(&projHalf, &projHalf, 0.5f);

            bool destroyProjectile = false;
            for (int j = 0; j < enemyList.size(); j++) {
                BaseEnemy* enemy = enemyList[j];
                if (enemy->IsDying()) continue;

                AEVec2 enemyPos = enemy->transform.GetPosition();
                AEVec2 enemyHalf = { fabs(enemy->transform.GetScale().x) / 2.0f, enemy->transform.GetScale().y / 2.0f };

                if (AABBOverlap(projPos, projHalf, enemyPos, enemyHalf)) {
                    if (dynamic_cast<SwordProjectile*>(pProj) == nullptr) {
                        enemy->TakeDamage(3);
                        destroyProjectile = true;
                        break;
                    }
                    else {
                        enemy->TakeDamage(5);
                    }
                }
            }

            if (destroyProjectile || pProj->GetLifespan() <= 0.f) {
                delete pProj;
                it = projs.erase(it);
            }
            else {
                ++it;
            }
        }

        for (auto it = enemyList.begin(); it != enemyList.end(); ) {
            if ((*it)->IsReadyToDelete()) {
                delete* it;
                it = enemyList.erase(it);
            }
            else {
                ++it;
            }
        }
    }
    else {
        audio->PauseMusic();
        AEVec2 CamPos = { 0.0f, 0.0f };
        AEGfxGetCamPosition(&CamPos.x, &CamPos.y);

        if (menuPage == 0) {
            btnResume->SetPosition(CamPos.x, CamPos.y + 100.0f);
            btnSaveMenu->SetPosition(CamPos.x, CamPos.y + 30.0f);
            btnLoadMenu->SetPosition(CamPos.x, CamPos.y - 40.0f);
            btnExit->SetPosition(CamPos.x, CamPos.y - 110.0f);

            if (btnResume->IsClicked()) isPaused = false;
            if (btnSaveMenu->IsClicked()) {
                menuPage = 1;
                btnSlot1->SetLabel(FileExists(1) ? "Slot 1 (Used)" : "Slot 1 (Empty)");
                btnSlot2->SetLabel(FileExists(2) ? "Slot 2 (Used)" : "Slot 2 (Empty)");
                btnSlot3->SetLabel(FileExists(3) ? "Slot 3 (Used)" : "Slot 3 (Empty)");
            }
            if (btnLoadMenu->IsClicked()) {
                menuPage = 2;
                btnSlot1->SetLabel(FileExists(1) ? "Load Slot 1" : "Empty");
                btnSlot2->SetLabel(FileExists(2) ? "Load Slot 2" : "Empty");
                btnSlot3->SetLabel(FileExists(3) ? "Load Slot 3" : "Empty");
            }
            if (btnExit->IsClicked()) SceneManager::ChangeScene(new MainMenu());
        }
        else {
            btnSlot1->SetPosition(CamPos.x, CamPos.y + 100.0f);
            btnSlot2->SetPosition(CamPos.x, CamPos.y + 30.0f);
            btnSlot3->SetPosition(CamPos.x, CamPos.y - 40.0f);
            btnBack->SetPosition(CamPos.x, CamPos.y - 110.0f);

            if (btnBack->IsClicked()) menuPage = 0;
            if (btnSlot1->IsClicked()) { if (menuPage == 1) SaveGame(1); else LoadGame(1); menuPage = 0; }
            if (btnSlot2->IsClicked()) { if (menuPage == 1) SaveGame(2); else LoadGame(2); menuPage = 0; }
            if (btnSlot3->IsClicked()) { if (menuPage == 1) SaveGame(3); else LoadGame(3); menuPage = 0; }
        }
    }
}

void BaseScene::Draw() {
    bg->Draw(meshManager->GetMesh(TEXTURE_SQUARE));
    map->RenderMap();

    AEGfxSetRenderMode(AE_GFX_RM_COLOR);

    ParticleManager::GetInstance()->Draw(meshManager->GetMesh(SQUARE));
    for (BaseEnemy* enemy : enemyList) {
        enemy->Draw();
    }

    if (abilityHandler) abilityHandler->Draw();
    player->Draw(meshManager->GetMesh(CORNER_SQUARE));

    if (isPaused) {
        AEVec2 CamPos = { 0.0f, 0.0f };
        AEGfxGetCamPosition(&CamPos.x, &CamPos.y);

        AEGfxSetRenderMode(AE_GFX_RM_COLOR);
        AEGfxSetBlendMode(AE_GFX_BM_BLEND);
        AEGfxSetTransparency(0.6f);
        AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
        AEGfxSetColorToMultiply(0.0f, 0.0f, 0.0f, 1.0f);

        AEMtx33 scale, trans, final;
        AEMtx33Scale(&scale, 3000.0f, 3000.0f);
        AEMtx33Trans(&trans, CamPos.x, CamPos.y);
        AEMtx33Concat(&final, &trans, &scale);
        AEGfxSetTransform(final.m);

        AEGfxMeshDraw(meshManager->GetMesh(SQUARE), AE_GFX_MDM_TRIANGLES);

        AEGfxSetTransparency(1.0f);
        AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

        if (menuPage == 0) {
            btnResume->Draw(); btnSaveMenu->Draw(); btnLoadMenu->Draw(); btnExit->Draw();
        }
        else {
            btnSlot1->Draw(); btnSlot2->Draw(); btnSlot3->Draw(); btnBack->Draw();
        }

        if (popupTimer > 0.0f) {
            AEGfxPrint(Button::fontId, "Game Saved!", -0.1f, 0.35f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f);
            popupTimer -= (f32)AEFrameRateControllerGetFrameTime();
        }
    }
}

void BaseScene::SaveGame(int slot) {
    std::string path = "save_slot_" + std::to_string(slot) + ".txt";
    std::ofstream f(path);
    if (f.is_open()) {
        AEVec2 pos = player->GetTransform().GetPosition();
        f << pos.x << " " << pos.y << " " << player->GetHealth();
        f.close();
        popupTimer = 2.0f;
    }
}

void BaseScene::LoadGame(int slot) {
    std::string path = "save_slot_" + std::to_string(slot) + ".txt";
    std::ifstream f(path);
    if (f.is_open()) {
        float x, y, hp;
        if (f >> x >> y >> hp) {
            player->GetTransform().SetPosition(x, y);
            player->SetHealth(hp);
            isPaused = false;
        }
        f.close();
    }
}

bool BaseScene::FileExists(int slot) const {
    std::ifstream f("save_slot_" + std::to_string(slot) + ".txt");
    return f.good();
}

void BaseScene::Free() {
    if (abilityHandler) delete abilityHandler;

    delete btnResume; delete btnSaveMenu; delete btnLoadMenu; delete btnExit;
    delete btnSlot1; delete btnSlot2; delete btnSlot3; delete btnBack;

    for (BaseEnemy* enemy : enemyList) {
        delete enemy;
    }
    enemyList.clear();

    if (bg) {
        delete bg;
        bg = nullptr;
    }

    ParticleManager::GetInstance()->ClearAll();
}

void BaseScene::Unload() {
    player = nullptr;
    abilityHandler = nullptr;
    meshManager = nullptr;
    enemyList.clear();

    ShutdownSwordResources();
    ShutdownKnifeResources();
}