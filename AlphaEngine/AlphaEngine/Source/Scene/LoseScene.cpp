#include "LoseScene.h"
#include "../System/Scenemanager.h"
#include "TutorialScene.h"
#include "Level1Scene.h"
#include "BossScene.h"
#include "MainMenu.h"
#include "AEEngine.h"
#include <fstream>
#include <string>

LoseScene::LoseScene(int idToRestart) {
    restartLevelID = idToRestart;
	player = Player::GetInstance();
	player->SetHealth(100);
}

void LoseScene::Load() {
    AEGfxReset();
    Button::LoadResources();

    btnRestart = new Button(0.0f, 40.0f, 200.0f, 50.0f, "Try Again");
    btnLoad = new Button(0.0f, -30.0f, 200.0f, 50.0f, "Load Game");
    btnMenu = new Button(0.0f, -100.0f, 200.0f, 50.0f, "Main Menu");

    btnSlot1 = new Button(0.0f, 40.0f, 200.0f, 50.0f, "Slot 1");
    btnSlot2 = new Button(0.0f, -30.0f, 200.0f, 50.0f, "Slot 2");
    btnSlot3 = new Button(0.0f, -100.0f, 200.0f, 50.0f, "Slot 3");
    btnBack = new Button(0.0f, -170.0f, 200.0f, 50.0f, "Back");
}

void LoseScene::Init() {
    menuPage = 0;
}

void LoseScene::Update() {
    if (menuPage == 0) {
        if (btnRestart->IsClicked()) {
            //holds the level and load them accordingly to where player died
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

void LoseScene::Draw() {
    AEGfxSetCamPosition(0.0f, 0.0f);
    AEGfxSetBackgroundColor(0.3f, 0.0f, 0.0f);

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
        AEGfxPrint(Button::fontId, "YOU DIED!", -0.1f, 0.3f, 1.5f, 1.0f, 1.0f, 1.0f, 1.0f);
    }
}

void LoseScene::Free() {
    delete btnRestart; delete btnLoad; delete btnMenu;
    delete btnSlot1; delete btnSlot2; delete btnSlot3; delete btnBack;
    BaseScene::Free();
    Button::FreeResources();
}

void LoseScene::Unload() {}