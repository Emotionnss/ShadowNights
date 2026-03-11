#include "BossScene.h"
#include <cmath>
#include <iostream>
#include "../System/Scenemanager.h"
#include "TestScene2.h"
#include "../Utils/utils.h"
#include "../Enemy/ShadowEnemy.h"
#include "../Enemy/BatEnemy.h"
#include "../Enemy/ArcherEnemy.h"
#include "../particle/ParticleManager.h"
#include "WinScene.h"


void BossScene::Load() {
    AEGfxReset();
    meshManager = MeshManager::GetInstance();
    map = Map::GetInstance();
    player = Player::GetInstance();

    bg = new Background;
    bg->SetStatic(true);

    map->LoadMap("Level2");
}

void BossScene::Init() {
    player->GetTransform().SetPosition(300.0f, 200.0f);
    abilityHandler = new AbilityHandler();

    boss = new BossEnemy();
    boss->Init(1400.f, 300.0f, 85.0f, 60.0f, "boss");
}

void BossScene::Update() {
    f32 dt = (f32)AEFrameRateControllerGetFrameTime();

    player->HandleInput();
    player->Update(dt);
    if (abilityHandler) abilityHandler->AbilityUpdate(enemyList,dt);

    // UPDATE ALL ENEMIES (IMPT)
    for (BaseEnemy* enemy : enemyList) {
        enemy->Update(dt);
    }

    boss->Update(dt);

    ParticleManager::GetInstance()->Update(dt);

    //Go back to main menu
    /*if (player->GetCollidedTile().tileType == TILE_TYPE::BACKGROUND && enemyList.size() == 0)
    {
        SceneManager::ChangeScene();
    }*/

    // N***A enemy projectile
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

                delete proj;
                it = enemyProjs.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    // SWORD PEEPEEPOOPOO
    std::vector<BaseProjectile*>& projs = player->projectileList;

    for (auto it = projs.begin(); it != projs.end(); )
    {
        BaseProjectile* pProj = *it;

        AEVec2 projPos = pProj->transform.GetPosition();
        AEVec2 projHalf = pProj->transform.GetScale();
        AEVec2Scale(&projHalf, &projHalf, 0.5f);

        bool destroyProjectile = false;
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

                    SwordProjectile* swordProj = dynamic_cast<SwordProjectile*>(pProj);
                    if (!swordProj)
                    {
                        destroyProjectile = true; 
                        break;
                    }
                }
            }
        }

        //Only update if its active
        if (boss->GetWeakpoint().active)
        {
            Transform2D bossWeakpointTransform = boss->GetWeakpoint().transform;
            AEVec2 bossWeakpointHalf = { fabs(bossWeakpointTransform.GetScale().x) / 2.0f, bossWeakpointTransform.GetScale().y / 2.0f };

            if (AABBOverlap(projPos, projHalf, bossWeakpointTransform.GetPosition(), bossWeakpointHalf))
            {
                if (!boss->IsReadyToDelete())
                {
                    boss->TakeDamage();

                    SwordProjectile* swordProj = dynamic_cast<SwordProjectile*>(pProj);
                    if (!swordProj)
                    {
                        destroyProjectile = true;
                        break;
                    }
                }
            }
        }

        // erase the projectile (zav is gay)
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

    // enemy die alr or nut punde
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

    if (boss && boss->IsReadyToDelete())
    {
        SceneManager::ChangeScene(new WinScene());
        return;
    }

}

void BossScene::Draw() {
    AEGfxSetBackgroundColor(0.4f, 0.4f, 0.4f);

    bg->Draw(meshManager->GetMesh(TEXTURE_SQUARE));

    if (boss != nullptr)
        boss->Draw();

    map->RenderMap();

    AEGfxSetRenderMode(AE_GFX_RM_COLOR);

    if (abilityHandler) abilityHandler->Draw();
    player->Draw(meshManager->GetMesh(CORNER_SQUARE));

   /* for (BaseEnemy* enemy : enemyList) {
        enemy->Draw();
    }*/

    ParticleManager::GetInstance()->Draw(meshManager->GetMesh(SQUARE));

    player->Draw(meshManager->GetMesh(CORNER_SQUARE));
}

void BossScene::Free() {
    player->DestroyInstance();
    if (abilityHandler) delete abilityHandler;

    // CLEANUP KUKUBIRD CCB KNN OBAMA RASENGAN
    for (BaseEnemy* enemy : enemyList) {
        delete enemy;
    }
    enemyList.clear();
    delete boss;
    delete bg;
    ParticleManager::GetInstance()->ClearAll();
}

void BossScene::Unload()
{
    player = nullptr;
    abilityHandler = nullptr;
    meshManager = nullptr;
    enemyList.clear();
}