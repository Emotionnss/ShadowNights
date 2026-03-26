#include "Credits.h"
#include "../System/SceneManager.h"
#include "MainMenu.h"
#include "AEEngine.h"

void CreditsScene::Load() {
    AEGfxReset();
    Button::LoadResources();
    backBtn = new Button(0.0f, -100.0f, 200.0f, 50.0f, "Main Menu");
}

void CreditsScene::Init() {}

void CreditsScene::Update() {
    if (backBtn->IsClicked()) {
        SceneManager::ChangeScene(new MainMenu());
    }
}

void CreditsScene::Draw() {
    AEGfxSetCamPosition(0.0f, 0.0f);
    AEGfxSetBackgroundColor(0.8f, 0.6f, 0.1f);

    backBtn->Draw();

    if (Button::fontId != -1) {
        AEGfxPrint(Button::fontId, "CREDITS SCENE YIIPPPEEEE", -0.15f, 0.2f, 1.5f, 1.0f, 1.0f, 1.0f, 1.0f);
    }
}

void CreditsScene::Free() {
    delete backBtn;
    Button::FreeResources();
}

void CreditsScene::Unload() {}