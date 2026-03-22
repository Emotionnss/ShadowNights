#include "WinScene.h"
#include "../System/SceneManager.h"
#include "MainMenu.h"
#include "AEEngine.h"

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