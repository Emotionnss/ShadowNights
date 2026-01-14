#include "TestScene.h"
#include <cmath>
#include <iostream>
#include "../System/Scenemanager.h"
#include "TestScene2.h"

void TestScene::Load() {
    
    AEGfxReset();
    AEGfxMeshStart();

    // Use a simple single triangle to test first
    AEGfxTriAdd(
        -0.5f, -0.5f, 0xFFFF0000, 0.0f, 0.0f,
        0.5f, -0.5f, 0xFFFF0000, 0.0f, 0.0f,
        0.0f, 0.5f, 0xFFFF0000, 0.0f, 0.0f
    );

    pMesh = AEGfxMeshEnd();

    if (pMesh == nullptr) {
        std::cout << "ERROR: Mesh Creation Failed!" << std::endl;
    }
    else {
        std::cout << "SUCCESS: Mesh Created." << std::endl;
    }
}

void TestScene::Init() {
    playerX = 0.0f;
    playerY = 0.0f;
}

void TestScene::Update() {
    float dt = (float)AEFrameRateControllerGetFrameTime();

    if (AEInputCheckCurr(VK_UP))    playerY += speed * dt;
    if (AEInputCheckCurr(VK_DOWN))  playerY -= speed * dt;
    if (AEInputCheckCurr(VK_LEFT))  playerX -= speed * dt;
    if (AEInputCheckCurr(VK_RIGHT)) playerX += speed * dt; 


    if (AEInputCheckTriggered(VK_SPACE)) {
        std::cout << "DEBUG: Space IS Held Down!" << std::endl;
        SceneManager::ChangeScene(new TestScene2());
        return;
    }
}

void TestScene::Draw() {

    AEGfxSetCamPosition(0.0f, 0.0f);

    AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);

    AEMtx33 scaleMtx, transMtx, finalMtx;
    AEMtx33Scale(&scaleMtx, 100.0f, 100.0f);
    AEMtx33Trans(&transMtx, playerX, playerY);
    AEMtx33Concat(&finalMtx, &transMtx, &scaleMtx);

    AEGfxSetTransform(finalMtx.m);
    AEGfxSetTransparency(1.0f);

    if (pMesh != nullptr) {
        AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
    }
}

void TestScene::Free() {
    
}

void TestScene::Unload() {
    if (pMesh != nullptr) {
        AEGfxMeshFree(pMesh);
        pMesh = nullptr;
    }
}