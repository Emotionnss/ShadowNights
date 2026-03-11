#include "MainMenu.h"
#include "../System/Scenemanager.h"
#include "TestScene.h"
#include "TestScene2.h"
#include "AEEngine.h"
#include <iostream>
#include <fstream>
#include <string>

extern bool gGameRunning;

bool MainMenu::FileExists(int slot) {
    std::ifstream f("save_slot_" + std::to_string(slot) + ".txt");
    return f.good();
}

void MainMenu::DeleteSave(int slot) {
    std::string path = "save_slot_" + std::to_string(slot) + ".txt";
    std::remove(path.c_str());
}

void MainMenu::Load() {
    AEGfxReset();
    Button::LoadResources();

    btnPlay = new Button(0.0f, 90.0f, 200.0f, 50.0f, "New Game");
    btnLoad = new Button(0.0f, 30.0f, 200.0f, 50.0f, "Load Game");
    btnSettings = new Button(0.0f, -30.0f, 200.0f, 50.0f, "Settings");
    btnQuit = new Button(0.0f, -90.0f, 200.0f, 50.0f, "Quit");

    btnSlot1 = new Button(0.0f, 90.0f, 200.0f, 50.0f, "Slot 1");
    btnSlot2 = new Button(0.0f, 30.0f, 200.0f, 50.0f, "Slot 2");
    btnSlot3 = new Button(0.0f, -30.0f, 200.0f, 50.0f, "Slot 3");
    btnBack = new Button(0.0f, -90.0f, 200.0f, 50.0f, "Back");
}

void MainMenu::Init() {
    menuPage = 0;
}

void MainMenu::Update() {
    if (menuPage == 0) {
        if (btnPlay->IsClicked()) {

            DeleteSave(1);
            DeleteSave(2);
            DeleteSave(3);

            SceneManager::ChangeScene(new TestScene(0));
        }

        if (btnLoad->IsClicked()) {
            menuPage = 1;
            btnSlot1->SetLabel(FileExists(1) ? "Load Slot 1" : "Empty");
            btnSlot2->SetLabel(FileExists(2) ? "Load Slot 2" : "Empty");
            btnSlot3->SetLabel(FileExists(3) ? "Load Slot 3" : "Empty");
        }
        if (btnSettings->IsClicked()) {
            std::cout << "Settings Pressed (Feature not implemented yet)" << std::endl;
        }
        if (btnQuit->IsClicked()) {
            gGameRunning = false;
        }

        if (AEInputCheckTriggered(AEVK_SPACE)) {
            std::cout << "Zavier Sucks!!!" << std::endl;
        }
    }
    else {
        if (btnBack->IsClicked()) menuPage = 0;

        if (btnSlot1->IsClicked() && FileExists(1)) SceneManager::ChangeScene(new TestScene(1));
        if (btnSlot2->IsClicked() && FileExists(2)) SceneManager::ChangeScene(new TestScene(2));
        if (btnSlot3->IsClicked() && FileExists(3)) SceneManager::ChangeScene(new TestScene(3));
    }
}

void MainMenu::Draw() {
    AEGfxSetCamPosition(0.0f, 0.0f);
    AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

    if (menuPage == 0) {
        btnPlay->Draw();
        btnLoad->Draw();
        btnSettings->Draw();
        btnQuit->Draw();
    }
    else {
        btnSlot1->Draw();
        btnSlot2->Draw();
        btnSlot3->Draw();
        btnBack->Draw();
    }
}

void MainMenu::Free() {
    delete btnPlay;
    delete btnLoad;
    delete btnSettings;
    delete btnQuit;

    delete btnSlot1;
    delete btnSlot2;
    delete btnSlot3;
    delete btnBack;

    Button::FreeResources();
}

void MainMenu::Unload() {}