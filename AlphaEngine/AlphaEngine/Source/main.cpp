#include "AEEngine.h"
#include "System/SceneManager.h"
#include "System/MeshManager.h"
#include "System/Map.h"
#include "Scene/TutorialScene.h"
#include "Scene/MainMenu.h"
#include "Scene/Splashscreen.h"
#include "System/SettingsManager.h"
#include <iostream>
#include <vector>
#include <string>
#include "particle/ParticleManager.h"
#include "System/AudioManager.h"

bool gGameRunning = true;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) 
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // 1. Initialize
    if (AESysInit(hInstance, nCmdShow, 1600, 900, 1, 60, false, NULL) == 0) {
        return 0;
    }

    //Init settings
    std::vector<std::string> settingsInfo = CSVManager::ReadSettingsInfo();
    SettingsManager* sManager = SettingsManager::GetInstance(settingsInfo);

    AudioManager* aManager = AudioManager::GetInstance();

    SceneManager::ChangeScene(new Splashscreen());

    while (gGameRunning) {

        AESysFrameStart();

        if (0 == AESysDoesWindowExist())
            gGameRunning = false;

        sManager->CheckSettingsInputs();

        SceneManager::Update();

        AEGfxStart();
        AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
        SceneManager::Draw();

        AESysFrameEnd();
    }
    SceneManager::Exit();

    Player::GetInstance()->DestroyInstance();
    MeshManager::DestroyInstance();
    Map::GetInstance()->DestroyInstance();

    AudioManager::DestroyInstance();
    aManager = nullptr;
    SettingsManager::DestroyInstance();
    sManager = nullptr;
    ParticleManager::DestroyInstance();

    Button::FreeResources();

    AESysExit();

    return 0;
}