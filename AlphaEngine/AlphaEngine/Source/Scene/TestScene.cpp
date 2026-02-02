#include "TestScene.h"
#include <cmath>
#include <iostream>
#include "../System/Scenemanager.h"
#include "TestScene2.h"

void TestScene::Load() {
    AEGfxReset();
    meshManager = MeshManager::GetInstance();
    map = Map::GetInstance();
    player = Player::GetInstance();

    map->LoadMap("Level1");

    myEnemy = new BaseEnemy();
}

void TestScene::Init() {
    player->GameObject.GetTransform().SetPosition(300, 900);
    abilityHandler = new AbilityHandler();

    if (myEnemy) {

        myEnemy->Init(500.0f, 0.0f, 85.0f, 60.0f, "shadow1 - Idle_test");

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

    if (AEInputCheckTriggered(VK_SPACE)) {
        std::cout << "DEBUG: Space IS Held Down!" << std::endl;
        SceneManager::ChangeScene(new TestScene2());
        return;
    }
}

void TestScene::Draw() {
    AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
    map->RenderMap();

    AEGfxSetRenderMode(AE_GFX_RM_COLOR);

    // Draw Player & Abilities
    player->Draw(meshManager->GetMesh(SQUARE));
    abilityHandler->Draw();

    if (myEnemy) {
        myEnemy->Draw();
    }

    // Floor placeholder
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