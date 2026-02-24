#include "TestScene.h"
#include <cmath>
#include <iostream>
#include "../System/Scenemanager.h"
#include "TestScene2.h"
#include "../utils.h" 

void TestScene::Load() {
    AEGfxReset();
    meshManager = MeshManager::GetInstance();
    map = Map::GetInstance();
    player = Player::GetInstance();

    map->LoadMap("Level1");
}

void TestScene::Init() {
    player->GetTransform().SetPosition(300.0f, 900.0f);
    abilityHandler = new AbilityHandler();

    // --- SPAWN MULTIPLE ENEMIES ---
    int numberOfEnemies = 3;
    float startX = 600.0f;
    float spacing = 300.0f;

    for (int e = 0; e < numberOfEnemies; e++)
    {
        BaseEnemy* newEnemy = new BaseEnemy();
        float spawnX = startX + (e * spacing);

        newEnemy->Init(spawnX, 300.0f, 85.0f, 60.0f, "shadow1");
        newEnemy->SetVelocity(0.0f, 0.0f);
        enemyList.push_back(newEnemy);
    }

    // NOTE: Removed the buggy collision check that was sitting in here!
}

void TestScene::Update() {
    f32 dt = (f32)AEFrameRateControllerGetFrameTime();

    player->HandleInput();
    player->Update(dt);
    if (abilityHandler) abilityHandler->AbilityUpdate(dt);

    // UPDATE ALL ENEMIES
    for (BaseEnemy* enemy : enemyList) {
        enemy->Update(dt);
    }

    // ---------------------------------------------------
    // SAFE ENEMY PROJECTILE COLLISION 
    // ---------------------------------------------------
    for (BaseEnemy* enemy : enemyList)
    {
        std::vector<EnemyProjectile*>& enemyProjs = enemy->GetProjectiles();

        for (auto it = enemyProjs.begin(); it != enemyProjs.end(); )
        {
            EnemyProjectile* proj = *it;

            AEVec2 projPos = proj->transform.GetPosition();
            AEVec2 projHalf = { 20.0f, 20.0f };

            AEVec2 playerPos = player->GetTransform().GetPosition();
            AEVec2 playerHalf = { 35.0f, 35.0f };

            if (AABBOverlap(projPos, projHalf, playerPos, playerHalf))
            {
                std::cout << "OUCH! Player Hit!" << std::endl;
                player->TakeDamage(10);

                // Destroy the Projectile safely
                delete proj;
                it = enemyProjs.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    // ---------------------------------------------------
    // SAFE PLAYER PROJECTILE COLLISION
    // ---------------------------------------------------
    std::vector<BaseProjectile*>& projs = player->projectileList;

    for (auto it = projs.begin(); it != projs.end(); )
    {
        BaseProjectile* pProj = *it;

        AEVec2 projPos = pProj->transform.GetPosition();
        AEVec2 projHalf = pProj->transform.GetScale();
        AEVec2Scale(&projHalf, &projHalf, 0.5f);

        bool destroyProjectile = false; // Flag to safely delete

        for (int j = 0; j < enemyList.size(); j++)
        {
            BaseEnemy* enemy = enemyList[j];

            AEVec2 enemyPos = enemy->transform.GetPosition();
            AEVec2 enemyHalf = { fabs(enemy->transform.GetScale().x) / 2.0f, enemy->transform.GetScale().y / 2.0f };

            if (AABBOverlap(projPos, projHalf, enemyPos, enemyHalf))
            {
                if (!enemy->IsReadyToDelete())
                {
                    enemy->TakeDamage();

                    // Check if it's a sword
                    SwordProjectile* swordProj = dynamic_cast<SwordProjectile*>(pProj);
                    if (!swordProj)
                    {
                        destroyProjectile = true; // Mark for deletion
                        break; // Stop checking other enemies, bullet is gone!
                    }
                }
            }
        }

        // Safely erase the projectile ONLY AFTER the loops are completely done with it
        if (destroyProjectile || pProj->GetLifespan() <= 0.f)
        {
            delete pProj;
            it = projs.erase(it);
        }
        else
        {
            ++it;
        }
    }

    // ---------------------------------------------------
    // SAFE ENEMY DELETION
    // ---------------------------------------------------
    for (auto it = enemyList.begin(); it != enemyList.end(); )
    {
        if ((*it)->IsReadyToDelete())
        {
            delete* it;
            it = enemyList.erase(it);
            std::cout << "Enemy Died and Removed!" << std::endl;
        }
        else
        {
            ++it;
        }
    }
}

void TestScene::Draw() {
    AEGfxSetBackgroundColor(0.4f, 0.4f, 0.4f);
    map->RenderMap();

    AEGfxSetRenderMode(AE_GFX_RM_COLOR);

    if (abilityHandler) abilityHandler->Draw();
    player->Draw(meshManager->GetMesh(CORNER_SQUARE));

    // DRAW ALL ENEMIES
    for (BaseEnemy* enemy : enemyList) {
        enemy->Draw();
    }

    AEMtx33 scaleMtx, transMtx, finalMtx;
    AEMtx33Scale(&scaleMtx, 800.0f, 100.0f);
    AEMtx33Trans(&transMtx, 0.f, -475.f);
    AEMtx33Concat(&finalMtx, &transMtx, &scaleMtx);

    AEGfxSetColorToAdd(1.f, 1.f, 1.f, 1.f);
    AEGfxSetTransform(finalMtx.m);
    AEGfxSetTransparency(1.0f);
    AEGfxMeshDraw(meshManager->GetMesh(SQUARE), AE_GFX_MDM_TRIANGLES);
}

void TestScene::Free() {
    player->DestroyInstance();
    if (abilityHandler) delete abilityHandler;

    // CLEANUP ALL ENEMIES
    for (BaseEnemy* enemy : enemyList) {
        delete enemy;
    }
    enemyList.clear();
}

void TestScene::Unload()
{
    player = nullptr;
    abilityHandler = nullptr;
    meshManager = nullptr;
    enemyList.clear();
}