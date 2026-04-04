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

//this scene is shown when the player defeats the boss, and contains a button to return to the main menu. The background and music are also changed to reflect the victory.
void WinScene::Load() {
    levelBGM = "win_BGM";
    AEGfxReset();

    meshManager = MeshManager::GetInstance();
    bg = new Background("Assets/Sprites/Background/Bluemoon.png");

    Button::LoadResources();
    btnMenu = new Button(0.0f, -100.0f, 200.0f, 50.0f, "Main Menu");
    AudioManager::GetInstance()->PlayMusic(levelBGM);
}

//Initializes the win scene, currently does not have any variables to initialize, but is defined for consistency with other scenes and potential future use
void WinScene::Init() {}

//Handles button interaction for the main menu button, which returns the player to the main menu when clicked
void WinScene::Update() {
    if (btnMenu->IsClicked()) {
        SceneManager::ChangeScene(new MainMenu());
    }
}

//Renders the win scene background and a victory message, as well as the main menu button
void WinScene::Draw() {
    AEGfxSetCamPosition(0.0f, 0.0f);

    if (bg && meshManager) {
        bg->Draw(meshManager->GetMesh(TEXTURE_SQUARE));
    }

    char winMsg[64];
    snprintf(winMsg, sizeof(winMsg), "BOSS DEFEATED!");

    float w = 0.0f, h = 0.0f;
    AEGfxGetPrintSize(Button::fontId, winMsg, 1.5f, &w, &h);

    AEGfxPrint(Button::fontId, winMsg, -(w / 2.0f), 0.15f, 1.5f, 0.0f, 0.0f, 0.0f, 1.0f);

    btnMenu->Draw();
}

//Frees memory allocated for the main menu button and the background, and also calls the Button class's FreeResources function to free the shared button resources
void WinScene::Free() {
    delete btnMenu;

    if (bg) {
        delete bg;
        bg = nullptr;
    }
    
    Button::FreeResources();
}

void WinScene::Unload() {}