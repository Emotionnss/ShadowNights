#include "AEEngine.h"
#include "System/SceneManager.h"
#include "System/MeshManager.h"
#include "System/Map.h"
#include "Scene/TestScene.h"
#include "Scene/MainMenu.h"
#include "System/SettingsManager.h"
#include <iostream>
#include <vector>
#include <string>
#include "particle/ParticleManager.h"

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

    SceneManager::ChangeScene(new MainMenu());

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

    // 4. Cleanup
    SceneManager::Exit();
    MeshManager::DestroyInstance();
    Map::DestroyInstance();
    Button::FreeResources();
    SettingsManager::DestroyInstance();
    sManager = nullptr;
    ParticleManager::DestroyInstance();
    AESysExit();

    return 1;
}