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
    levelBGM = "creditsBGM";//the name of the BGM for the level , variable is used bc overrided from basescene
    meshManager = MeshManager::GetInstance();//get the meshmanagers instance
    AEGfxReset();//reset the visuals
    Button::LoadResources();//load resources from the button class
    backBtn = new Button(-700.f,400.f, 200.0f, 50.0f, "Back");//initialize a back button
    AudioManager::GetInstance()->PlayMusic(levelBGM);//play BGM
}

void CreditsScene::Init() 
{
    creditsImage = AEGfxTextureLoad("Assets/Credits/Credits.png");//load the credits png
    creditsTfm.SetPosition(0,static_cast<f32>( - AEGfxGetWindowHeight() * 2));//set its position
    creditsTfm.SetScale(1600, 3000);//set its scale to the pixel sizes of the image

    //initialize all the sprites with its respective spritesheets
    playerSprite = new Sprite(CSVManager::ReadInfo("tempPlayer"));
    archerSprite = new Sprite(CSVManager::ReadInfo("sniperenemy"));
    batSprite = new Sprite(CSVManager::ReadInfo("batenemy"));
    shadowSprite = new Sprite(CSVManager::ReadInfo("shadow1"));
    bossSprite = new Sprite(CSVManager::ReadInfo("boss"));

    //load texture of the bg and the logo
    logoTex = AEGfxTextureLoad("Assets/Credits/logo.png");
    bgTex = AEGfxTextureLoad("Assets/Sprites/Background/Dark Gothic Castle.png");

    //set all the sprite positions and scales and make them loop an animation while scrolling
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

    bossTfm.SetPosition(0.f, -3200.f);
    bossTfm.SetScale(300.f, 300.f);
    bossSprite->PlayAnimation("idle");

    logoTfm.SetScale(600.f, 400.f);
    logoTfm.SetPosition(0.f, -3700.f);

    bgTfm.SetPosition(0.f, 0.f);
    bgTfm.SetScale(static_cast<f32>(AEGfxGetWindowWidth()), static_cast<f32>(AEGfxGetWindowHeight()));
}

void CreditsScene::Update() {
    //check if the back button is clicked, if so
    if (backBtn->IsClicked()) {
        SceneManager::ChangeScene(new MainMenu()); //change back to the main menu
    }
    //set the scroll speed depending on if the user wants to fast forward
    scrollSpeed = AEInputCheckCurr(AEVK_LBUTTON) ? 500.f : 100.f;

    //automatic scroll up for the credits and all the sprites
    creditsTfm.SetPosition(creditsTfm.GetPosition().x,creditsTfm.GetPosition().y + scrollSpeed * static_cast<f32>(AEFrameRateControllerGetFrameTime() ));

    pSTfm.SetPosition(pSTfm.GetPosition().x, pSTfm.GetPosition().y + scrollSpeed * static_cast<f32>(AEFrameRateControllerGetFrameTime()));

    archerTfm.SetPosition(archerTfm.GetPosition().x, archerTfm.GetPosition().y + scrollSpeed * static_cast<f32>(AEFrameRateControllerGetFrameTime()));

    batTfm.SetPosition(batTfm.GetPosition().x, batTfm.GetPosition().y + scrollSpeed * static_cast<f32>(AEFrameRateControllerGetFrameTime()));

    shadowTfm.SetPosition(shadowTfm.GetPosition().x, shadowTfm.GetPosition().y + scrollSpeed * static_cast<f32>(AEFrameRateControllerGetFrameTime()));

    bossTfm.SetPosition(bossTfm.GetPosition().x, bossTfm.GetPosition().y + scrollSpeed * static_cast<f32>(AEFrameRateControllerGetFrameTime()));

    //if the logo is not at position yet
    if (logoTfm.GetPosition().y <= 190)
    {
        //scroll it up
        logoTfm.SetPosition(logoTfm.GetPosition().x, logoTfm.GetPosition().y + scrollSpeed * static_cast<f32>(AEFrameRateControllerGetFrameTime()));
    }
    else//if it is at position
    {
        //set its position straight
        logoTfm.SetPosition(0.f, 200.f);
        logoAlpha += 0.5f * static_cast<f32>(AEFrameRateControllerGetFrameTime());//increase the alpha of the logo
        bgAlpha += 0.5f * static_cast<f32>(AEFrameRateControllerGetFrameTime());//increase the alpha of the bg
        if (logoAlpha >= 1.f && bgAlpha >=1.f)//once both of fully visible
        {
            SceneManager::ChangeScene(new MainMenu());//change scenes
        }
    }
    //update all the sprites
    playerSprite->Update();
    archerSprite->Update();
    batSprite->Update();
    shadowSprite->Update();
    bossSprite->Update();
}

void CreditsScene::Draw() {
    AEGfxSetCamPosition(0.0f, 0.0f);
    //AEGfxSetBackgroundColor(0.8f, 0.6f, 0.1f);
    //drawing of the credits image
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
    AEGfxSetColorToAdd(0.f, 0.f, 0.f, 0.0f);

    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetTransparency(1);

    AEGfxTextureSet(creditsImage, 0, 0);

    AEGfxSetTransform(creditsTfm.GetMatrix().m);
    AEGfxMeshDraw(meshManager->GetMesh(TEXTURE_SQUARE), AE_GFX_MDM_TRIANGLES);
    //drawing of the background texture
    AEGfxTextureSet(bgTex, 0, 0);

    AEGfxSetTransparency(bgAlpha);

    AEGfxSetTransform(bgTfm.GetMatrix().m);
    AEGfxMeshDraw(meshManager->GetMesh(TEXTURE_SQUARE), AE_GFX_MDM_TRIANGLES);
    //drawing of the logo
    AEGfxSetTransparency(logoAlpha);

    AEGfxTextureSet(logoTex, 0, 0);

    AEGfxSetTransform(logoTfm.GetMatrix().m);
    AEGfxMeshDraw(meshManager->GetMesh(TEXTURE_SQUARE), AE_GFX_MDM_TRIANGLES);
    //draw the button and the sprites
    backBtn->Draw();
    playerSprite->Render(pSTfm);
    archerSprite->Render(archerTfm);
    batSprite->Render(batTfm);
    shadowSprite->Render(shadowTfm);
    bossSprite->Render(bossTfm);
}

void CreditsScene::Free() {
    delete backBtn;//delete button
    Button::FreeResources();//free resources

    //if the texture or sprite is loaded, unload and delete them.
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
    if (logoTex)
    {
        AEGfxTextureUnload(logoTex);
    }
    if (bgTex)
    {
        AEGfxTextureUnload(bgTex);
    }
}

void CreditsScene::Unload() {}