/*!
@file       Credits.cpp
@author     Chew Zavier (c.zavier)
@brief      This file contains definitions of functions used by the Credits scene
*//*_______________________________________________________________________*/
#include "Credits.h"
#include "../System/SceneManager.h"
#include "MainMenu.h"
#include "AEEngine.h"

void CreditsScene::Load() {
    meshManager = MeshManager::GetInstance();
    AEGfxReset();
    Button::LoadResources();
    backBtn = new Button(-700.f,400.f, 200.0f, 50.0f, "Back");
}

void CreditsScene::Init() 
{
    creditsImage = AEGfxTextureLoad("Assets/Credits.png");
    creditsTfm.SetPosition(0,static_cast<f32>( - AEGfxGetWindowHeight() * 2));
    creditsTfm.SetScale(1600, 3000);
    playerSprite = new Sprite(CSVManager::ReadInfo("tempPlayer"));
    archerSprite = new Sprite(CSVManager::ReadInfo("sniperenemy"));
    batSprite = new Sprite(CSVManager::ReadInfo("batenemy"));
    shadowSprite = new Sprite(CSVManager::ReadInfo("shadow1"));
    bossSprite = new Sprite(CSVManager::ReadInfo("boss"));

    pSTfm.SetPosition(-500.f, -1000.f);
    pSTfm.SetScale(200.f, 200.f);
    playerSprite->PlayAnimation("walk_forward");

    archerTfm.SetPosition(0.f, 0.f);
    archerTfm.SetScale(200.f, 200.f);
    archerSprite->PlayAnimation("archerattack");

    batTfm.SetPosition(500.f, -1800.f);
    batTfm.SetScale(200.f, 200.f);
    batSprite->PlayAnimation("batidle");

    shadowTfm.SetPosition(-500.f, -2400.f);
    shadowTfm.SetScale(200.f, 200.f);
    shadowSprite->PlayAnimation("idle");

    bossTfm.SetPosition(0.f, -3000.f);
    bossTfm.SetScale(300.f, 300.f);
    bossSprite->PlayAnimation("idle");
}

void CreditsScene::Update() {
    if (backBtn->IsClicked()) {
        SceneManager::ChangeScene(new MainMenu()); 
    }
    scrollSpeed = AEInputCheckCurr(AEVK_LBUTTON) ? 500.f : 100.f;
    AEVec2 tfm = creditsTfm.GetPosition();
    tfm.y += scrollSpeed * static_cast<f32>(AEFrameRateControllerGetFrameTime());
    creditsTfm.SetPosition(tfm);

    tfm = pSTfm.GetPosition();
    tfm.y += scrollSpeed * static_cast<f32>(AEFrameRateControllerGetFrameTime());
    pSTfm.SetPosition(tfm);

    tfm = archerTfm.GetPosition();
    tfm.y += scrollSpeed * static_cast<f32>(AEFrameRateControllerGetFrameTime());
    archerTfm.SetPosition(tfm);

    tfm = batTfm.GetPosition();
    tfm.y += scrollSpeed * static_cast<f32>(AEFrameRateControllerGetFrameTime());
    batTfm.SetPosition(tfm);

    tfm = shadowTfm.GetPosition();
    tfm.y += scrollSpeed * static_cast<f32>(AEFrameRateControllerGetFrameTime());
    shadowTfm.SetPosition(tfm);

    tfm = bossTfm.GetPosition();
    tfm.y += scrollSpeed * static_cast<f32>(AEFrameRateControllerGetFrameTime());
    bossTfm.SetPosition(tfm);

    playerSprite->Update();
    archerSprite->Update();
    batSprite->Update();
    shadowSprite->Update();
    bossSprite->Update();
}

void CreditsScene::Draw() {
    AEGfxSetCamPosition(0.0f, 0.0f);
    //AEGfxSetBackgroundColor(0.8f, 0.6f, 0.1f);

    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
    AEGfxSetColorToAdd(0.f, 0.f, 0.f, 0.0f);

    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetTransparency(100);

    AEGfxTextureSet(creditsImage, 0, 0);

    AEGfxSetTransform(creditsTfm.GetMatrix().m);
    AEGfxMeshDraw(meshManager->GetMesh(TEXTURE_SQUARE), AE_GFX_MDM_TRIANGLES);

    backBtn->Draw();
    playerSprite->Render(pSTfm);
    archerSprite->Render(archerTfm);
    batSprite->Render(batTfm);
    shadowSprite->Render(shadowTfm);
    bossSprite->Render(bossTfm);
}

void CreditsScene::Free() {
    delete backBtn;
    Button::FreeResources();
    if (creditsImage)
        AEGfxTextureUnload(creditsImage);
    if (playerSprite)
    {
        delete playerSprite;
        playerSprite = nullptr;
    }
    if (archerSprite)
    {
        delete archerSprite;
        archerSprite = nullptr;
    }
    if (batSprite)
    {
        delete batSprite;
        batSprite = nullptr;
    }
    if (shadowSprite)
    {
        delete shadowSprite;
        shadowSprite = nullptr;
    }
    if (bossSprite)
    {
        delete bossSprite;
        bossSprite = nullptr;
    }
}

void CreditsScene::Unload() {}