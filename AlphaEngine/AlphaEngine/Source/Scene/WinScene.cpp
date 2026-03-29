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
    AEGfxReset();
    Button::LoadResources();
    btnMenu = new Button(0.0f, -100.0f, 200.0f, 50.0f, "Main Menu");
}

void WinScene::Init() {}

void WinScene::Update() {
    if (btnMenu->IsClicked()) {
        SceneManager::ChangeScene(new MainMenu());
    }
}

void WinScene::Draw() {
    AEGfxSetCamPosition(0.0f, 0.0f);
    AEGfxSetBackgroundColor(0.8f, 0.6f, 0.1f);

    btnMenu->Draw();

    if (Button::fontId != -1) {
        AEGfxPrint(Button::fontId, "BOSS DEFEATED!", -0.15f, 0.2f, 1.5f, 1.0f, 1.0f, 1.0f, 1.0f);
    }
}

void WinScene::Free() {
    delete btnMenu;
    Button::FreeResources();
}

void WinScene::Unload() {}