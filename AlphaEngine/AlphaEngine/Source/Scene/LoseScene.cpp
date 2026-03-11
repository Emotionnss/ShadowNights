#include "LoseScene.h"
#include "../System/Scenemanager.h"
#include "TestScene.h"
#include "MainMenu.h"
#include "AEEngine.h"
#include <fstream>
#include <string>

bool LoseScene::FileExists(int slot) {
    std::ifstream f("save_slot_" + std::to_string(slot) + ".txt");
    return f.good();
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
            SceneManager::ChangeScene(new TestScene(0));
        }
        if (btnLoad->IsClicked()) {
            menuPage = 1; // Swap to the slot menu
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

        if (btnSlot1->IsClicked() && FileExists(1)) SceneManager::ChangeScene(new TestScene(1));
        if (btnSlot2->IsClicked() && FileExists(2)) SceneManager::ChangeScene(new TestScene(2));
        if (btnSlot3->IsClicked() && FileExists(3)) SceneManager::ChangeScene(new TestScene(3));
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
        float textX = -0.15f;
        float textY = 0.35f;

        AEGfxPrint(Button::fontId, "YOU DIED", textX, textY, 1.5f, 1.0f, 1.0f, 1.0f, 1.0f);
    }
}

void LoseScene::Free() {
    delete btnRestart;
    delete btnLoad;
    delete btnMenu;

    delete btnSlot1;
    delete btnSlot2;
    delete btnSlot3;
    delete btnBack;

    Button::FreeResources();
}

void LoseScene::Unload() {
}