#include "MainMenu.h"
#include "../System/Scenemanager.h"
#include "TestScene.h"
#include "TestScene2.h"
#include "AEEngine.h"
#include <iostream>

extern bool gGameRunning;

void MainMenu::Load() {
    //Ensure global button resources (texture/font/mesh) are loaded
    Button::LoadResources();

    // Top Button
    btnPlay = new Button(0.0f, 0.0f, 200.0f, 50.0f, "Play");

    // Middle Button
    btnSettings = new Button(0.0f, -60.0f, 200.0f, 50.0f, "Settings");

    // Bottom Button
    btnQuit = new Button(0.0f, -120.0f, 200.0f, 50.0f, "Quit");
}

void MainMenu::Init() {
    //leave this pls
}

void MainMenu::Update() {

    if (btnPlay->IsClicked()) {
        std::cout << "Play Pressed! Loading Game..." << std::endl;
        SceneManager::ChangeScene(new TestScene());
        return;
    }
    
    if (btnSettings->IsClicked()) {
        std::cout << "Settings Pressed (Feature not implemented yet)" << std::endl;
        // SceneManager::ChangeScene(new SettingsScene());
    }

    if (btnQuit->IsClicked()) {
        std::cout << "Quit Pressed! Exiting..." << std::endl;
        gGameRunning = false; //shall change this if have time
    }
}

void MainMenu::Draw() {
    AEGfxSetCamPosition(0.0f, 0.0f);

    AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

    btnPlay->Draw();
    btnSettings->Draw();
    btnQuit->Draw();
}

void MainMenu::Free() {
    if (btnPlay) delete btnPlay;
    if (btnSettings) delete btnSettings;
    if (btnQuit) delete btnQuit;
}

void MainMenu::Unload() {
   //leave this pls
}
