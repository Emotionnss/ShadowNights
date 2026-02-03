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
                pProj->currLifespan = -1.0f;
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

    player->Draw(meshManager->GetMesh(SQUARE));
    abilityHandler->Draw();

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