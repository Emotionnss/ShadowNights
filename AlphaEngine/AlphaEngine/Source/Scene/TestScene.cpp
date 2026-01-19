#include "TestScene.h"
#include <cmath>
#include <iostream>
#include "../System/Scenemanager.h"
#include "TestScene2.h"

//* Player Testing

void TestScene::Load() {
    AEGfxReset();

    meshManager = MeshManager::GetInstance();
}

void TestScene::Init() {
    //playerX = 0.0f;
    //playerY = 0.0f;

    player = new Player;
}

void TestScene::Update() {
    float dt = (float)AEFrameRateControllerGetFrameTime();

    player->HandleInput();

    /*
    if (AEInputCheckCurr(VK_UP))    playerY += speed * dt;
    if (AEInputCheckCurr(VK_DOWN))  playerY -= speed * dt;
    if (AEInputCheckCurr(VK_LEFT))  playerX -= speed * dt;
    if (AEInputCheckCurr(VK_RIGHT)) playerX += speed * dt;
    */

    //Player Update
    player->Update(dt);

    if (AEInputCheckTriggered(VK_SPACE)) {
        std::cout << "DEBUG: Space IS Held Down!" << std::endl;
        SceneManager::ChangeScene(new TestScene2());
        return;
    }
}

void TestScene::Draw() {

    //AEGfxSetCamPosition(0.0f, 0.0f);

    AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);

    player->Draw(meshManager->GetMesh(SQUARE));

    // Floor placeholder lol
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
    
}

void TestScene::Unload() {}