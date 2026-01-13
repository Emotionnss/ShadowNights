#include "TestScene2.h"
#include "../System/Scenemanager.h"
#include "TestScene.h"
#include <iostream>

void TestScene2::Load() {
    AEGfxReset();
    AEGfxMeshStart();

    AEGfxTriAdd(
        -0.5f, -0.5f, 0xFFFFFF00, 0.0f, 0.0f,
        0.5f, -0.5f, 0xFFFFFF00, 0.0f, 0.0f,
        0.0f, 0.5f, 0xFFFFFF00, 0.0f, 0.0f
    );

    pMesh = AEGfxMeshEnd();
}

void TestScene2::Init() {
    playerX = 100.0f;
    playerY = 0.0f;
}

void TestScene2::Update() {
    float dt = (float)AEFrameRateControllerGetFrameTime();

    if (AEInputCheckCurr(VK_UP))    playerY += speed * dt;
    if (AEInputCheckCurr(VK_DOWN))  playerY -= speed * dt;
    if (AEInputCheckCurr(VK_LEFT))  playerX -= speed * dt;
    if (AEInputCheckCurr(VK_RIGHT)) playerX += speed * dt;

    if (AEInputCheckCurr(VK_SPACE)) {
        std::cout << "DEBUG: Space IS Held Down!" << std::endl;
        SceneManager::ChangeScene(new TestScene());
        return;
    }
}

void TestScene2::Draw() {
    AEGfxSetCamPosition(0.0f, 0.0f);
    AEGfxSetBackgroundColor(0.2f, 0.2f, 0.2f); // Dark Grey BG to differentiate scenes
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);

    AEMtx33 scaleMtx, transMtx, finalMtx;
    AEMtx33Scale(&scaleMtx, 100.0f, 100.0f);
    AEMtx33Trans(&transMtx, playerX, playerY);
    AEMtx33Concat(&finalMtx, &transMtx, &scaleMtx);
    AEGfxSetTransform(finalMtx.m);

    if (pMesh) AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
}

void TestScene2::Free() {}
void TestScene2::Unload() {
    if (pMesh) AEGfxMeshFree(pMesh);
}