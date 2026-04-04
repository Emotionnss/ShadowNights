#include "LoseScene.h"
#include "../System/Scenemanager.h"
#include "TutorialScene.h"
#include "Level1Scene.h"
#include "BossScene.h"
#include "MainMenu.h"
#include "AEEngine.h"
#include <fstream>
#include <string>

/*!
@file       LoseScene.cpp
@author     Lim Chee Kang (l.cheekang)
@brief      This file contains definitions of functions and variables
             used for the LoseScene class, which is the scene that is shown
             when the player dies, and contains buttons to restart the level,
             load from a save, or return to the main menu.
*//*_______________________________________________________________________*/

//Constructor that sets the restartLevelID variable, which is used to determine which level to reload when the player clicks the "Try Again" button. Also resets the player's health to 100 and sets the levelBGM to the lose music.
LoseScene::LoseScene(int idToRestart) {
    restartLevelID = idToRestart;
    player = Player::GetInstance();
    player->SetHealth(100);
    levelBGM = "lose_BGM";
}

//Helper function to check if a save file exists for a given slot, used to determine if the load buttons should be active or not
void LoseScene::Load() {
    AEGfxReset();

    meshManager = MeshManager::GetInstance();
    bg = new Background("Assets/Sprites/Background/Redmoon.png"); 

    Button::LoadResources();
    btnRestart = new Button(0.0f, 40.0f, 200.0f, 50.0f, "Try Again");
    btnLoad = new Button(0.0f, -30.0f, 200.0f, 50.0f, "Load Game");
    btnMenu = new Button(0.0f, -100.0f, 200.0f, 50.0f, "Main Menu");

    btnSlot1 = new Button(0.0f, 40.0f, 200.0f, 50.0f, "Slot 1");
    btnSlot2 = new Button(0.0f, -30.0f, 200.0f, 50.0f, "Slot 2");
    btnSlot3 = new Button(0.0f, -100.0f, 200.0f, 50.0f, "Slot 3");
    btnBack = new Button(0.0f, -170.0f, 200.0f, 50.0f, "Back");
    AudioManager::GetInstance()->PlayMusic(levelBGM);
}

//Initializes the menuPage variable to 0, which is used to determine which menu page to show when the player clicks the "Load Game" button (0 = main lose menu, 1 = load menu)
void LoseScene::Init() {
    menuPage = 0;
}

//Handles button interactions for the main lose menu and the load menu. In the main lose menu, clicking "Try Again" will reload the level the player died in, clicking "Load Game" will switch to the load menu, and clicking "Main Menu" will return to the main menu. In the load menu, clicking "Back" will return to the main lose menu, and clicking on a save slot will load that save if it exists.
void LoseScene::Update() {
    if (menuPage == 0) {
        if (btnRestart->IsClicked()) {
            if (restartLevelID == 0) {
                SceneManager::ChangeScene(new TutorialScene());
            }
            else if (restartLevelID == 1) {
                SceneManager::ChangeScene(new Level1Scene());
            }
            else if (restartLevelID == 2) {
                SceneManager::ChangeScene(new BossScene());
            }
        }
        if (btnLoad->IsClicked()) {
            menuPage = 2;
            btnSlot1->SetLabel(FileExists(1) ? "Load Slot 1" : "Empty");
            btnSlot2->SetLabel(FileExists(2) ? "Load Slot 2" : "Empty");
            btnSlot3->SetLabel(FileExists(3) ? "Load Slot 3" : "Empty");
        }
        if (btnMenu->IsClicked()) {
            SceneManager::ChangeScene(new MainMenu());
        }
    }
    else {
        if (btnBack->IsClicked()) menuPage = 0;

        if (btnSlot1->IsClicked() && FileExists(1)) SceneManager::ChangeScene(new TutorialScene(1));
        if (btnSlot2->IsClicked() && FileExists(2)) SceneManager::ChangeScene(new TutorialScene(2));
        if (btnSlot3->IsClicked() && FileExists(3)) SceneManager::ChangeScene(new TutorialScene(3));
    }
}

//Draws the lose screen background, buttons, and "YOU DIED" text to the screen. The buttons and text are only drawn if their respective resources have been loaded successfully.
void LoseScene::Draw() {
    AEGfxSetCamPosition(0.0f, 0.0f);

    if (bg && meshManager) {
        bg->Draw(meshManager->GetMesh(TEXTURE_SQUARE));
    }

    if (menuPage == 0) {
        btnRestart->Draw();
        btnLoad->Draw();
        btnMenu->Draw();
    }
    else {
        btnSlot1->Draw();
        btnSlot2->Draw();
        btnSlot3->Draw();
        btnBack->Draw();
    }

    if (Button::fontId != -1) {
        char loseMsg[64];
        snprintf(loseMsg, sizeof(loseMsg), "YOU DIED!");

        float w = 0.0f, h = 0.0f;
        AEGfxGetPrintSize(Button::fontId, loseMsg, 1.5f, &w, &h);

        AEGfxPrint(Button::fontId, loseMsg, -(w / 2.0f), 0.25f, 1.5f, 1.0f, 1.0f, 1.0f, 1.0f);
    }
}

//Frees the memory allocated for the buttons in the lose screen, and also calls the Free function of the BaseScene and Button classes to ensure proper cleanup of resources.
void LoseScene::Free() {
    delete btnRestart; delete btnLoad; delete btnMenu;
    delete btnSlot1; delete btnSlot2; delete btnSlot3; delete btnBack;

    BaseScene::Free();
    Button::FreeResources();
}

void LoseScene::Unload() {}