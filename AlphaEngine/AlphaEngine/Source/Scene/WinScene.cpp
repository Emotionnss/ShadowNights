#include "WinScene.h"
#include "../System/SceneManager.h"
#include "MainMenu.h"
#include "AEEngine.h"

/*!
@file       WinScene.cpp
@author     Lim Chee Kang (l.cheekang)
@brief      This file contains definitions of functions and variables
             used for the WinScene class, which is the scene that is shown
             when the player defeats the boss, and contains a button to return
             to the main menu.
*//*_______________________________________________________________________*/

void WinScene::Load() {
    levelBGM = "win_BGM";
    AEGfxReset();
    Button::LoadResources();
    btnMenu = new Button(0.0f, -100.0f, 200.0f, 50.0f, "Main Menu");
    AudioManager::GetInstance()->PlayMusic(levelBGM);
}

void WinScene::Init() { }

void WinScene::Update() {
    if (btnMenu->IsClicked()) {
        SceneManager::ChangeScene(new MainMenu());
    }
}

void WinScene::Draw() {
    AEGfxSetCamPosition(0.0f, 0.0f);

    AEGfxSetBackgroundColor(0.3f, 0.2f, 0.05f);

    char winMsg[64];
    snprintf(winMsg, sizeof(winMsg), "BOSS DEFEATED!");

    float w = 0.0f, h = 0.0f;
    AEGfxGetPrintSize(Button::fontId, winMsg, 1.0f, &w, &h);

    
    AEGfxPrint(Button::fontId, winMsg, -(w / 2.0f), 0.15f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    btnMenu->Draw();
}

void WinScene::Free() {
    delete btnMenu;
    Button::FreeResources();
}

void WinScene::Unload() {}