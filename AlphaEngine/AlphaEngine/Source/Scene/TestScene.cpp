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

    myEnemy = new BaseEnemy();
}

void TestScene::Init() {
    player->GetTransform().SetPosition(300.0f, 900.0f);
    abilityHandler = new AbilityHandler();

    if (myEnemy) {
        myEnemy->Init(900.0f, 300.0f, 85.0f, 60.0f, "shadow1");
        myEnemy->SetVelocity(0.0f, 0.0f);
        std::vector<EnemyProjectile*>& enemyProjs = myEnemy->GetProjectiles();
        for (int i = 0; i < enemyProjs.size(); i++)
        {
            AEVec2 projPos = enemyProjs[i]->GetTransform().GetPosition();
            AEVec2 projHalf = { 20.0f, 20.0f };

            AEVec2 playerPos = player->GetTransform().GetPosition();
            AEVec2 playerHalf = { 35.0f, 35.0f };

            if (AABBOverlap(projPos, projHalf, playerPos, playerHalf))
            {
                // Deal Damage
                player->TakeDamage(10);

                // Destroy Bullet
                delete enemyProjs[i];
                enemyProjs.erase(enemyProjs.begin() + i);
                i--;
            }
        }
    }
}

void TestScene::Update() {
    f32 dt = (f32)AEFrameRateControllerGetFrameTime();

    player->HandleInput();
    player->Update(dt);
    abilityHandler->AbilityUpdate(dt);

    if (myEnemy) {
        myEnemy->Update(dt);
    }

    //CHECK IF ENEMY PROJECTILES HIT PLAYER
    if (myEnemy)
    {
        std::vector<EnemyProjectile*>& enemyProjs = myEnemy->GetProjectiles();

        for (int i = 0; i < enemyProjs.size(); i++)
        {
            EnemyProjectile* proj = enemyProjs[i];

            // 1. Get Hitbox Info
            AEVec2 projPos = proj->GetTransform().GetPosition();
            AEVec2 projHalf = { 20.0f, 20.0f }; // 40x40 size (Half is 20)

            AEVec2 playerPos = player->GetTransform().GetPosition();
            AEVec2 playerHalf = { 35.0f, 35.0f }; // 70x70 size (Half is 35)

            // 2. Check Overlap
            if (AABBOverlap(projPos, projHalf, playerPos, playerHalf))
            {
                std::cout << "OUCH! Player Hit!" << std::endl;

                // Deal Damage to Player
                player->TakeDamage(10);

                // Destroy the Projectile immediately
                delete proj;
                enemyProjs.erase(enemyProjs.begin() + i);
                i--; // Adjust index since we removed an item
            }
        }
    }

    // --- PROJECTILE LOGIC ---
    std::vector<BaseProjectile*>& projs = player->projectileList;

    for (int i = 0; i < projs.size(); i++)
    {
        // Safety Check
        if (myEnemy == nullptr) break;

        BaseProjectile* pProj = projs[i];
        pProj->Update(dt);

        // --- Collision Check ---
        AEVec2 projPos = pProj->transform.GetPosition();
        AEVec2 projHalf = { 12.5f, 12.5f };

        AEVec2 enemyPos = myEnemy->transform.GetPosition();
        AEVec2 enemyHalf = { fabs(myEnemy->transform.GetScale().x) / 2.0f, myEnemy->transform.GetScale().y / 2.0f };

        //Check Overlap -> Trigger Damage
        if (AABBOverlap(projPos, projHalf, enemyPos, enemyHalf))
        {
            // Don't hit him again if he's already dying
            if (!myEnemy->IsReadyToDelete())
            {
                myEnemy->TakeDamage();

                // Destroy Projectile
                pProj->SetLifespan(-1.0f);
            }
        }

        // Cleanup Projectiles
        if (pProj->GetLifespan() <= 0.f)
        {
            delete pProj;
            projs.erase(projs.begin() + i);
            i--;
        }
    }

    // Delete enemy only when animation is done
    if (myEnemy && myEnemy->IsReadyToDelete())
    {
        delete myEnemy;
        myEnemy = nullptr;
        std::cout << "Enemy Died and Removed!" << std::endl;
    }

}

void TestScene::Draw() {
    AEGfxSetBackgroundColor(0.4f, 0.4f, 0.4f);
    map->RenderMap();

    AEGfxSetRenderMode(AE_GFX_RM_COLOR);

    abilityHandler->Draw();
    player->Draw(meshManager->GetMesh(SQUARE));

    if (myEnemy) {
        myEnemy->Draw();
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
    delete abilityHandler;

    if (myEnemy) {
        delete myEnemy;
        myEnemy = nullptr;
    }
}

void TestScene::Unload()
{
    player = nullptr;
    abilityHandler = nullptr;
    meshManager = nullptr;
    myEnemy = nullptr;
}