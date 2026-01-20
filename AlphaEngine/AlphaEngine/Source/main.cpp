#include "AEEngine.h"
#include "System/SceneManager.h"
#include "System/MeshManager.h"
#include "System/Map.h"
#include "Scene/TestScene.h"
#include <iostream>
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) 
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // 1. Initialize
    if (AESysInit(hInstance, nCmdShow, 1600, 900, 1, 60, false, NULL) == 0) {
        return 0;
    }

    bool gGameRunning = true;

    // 2. Set Scene
    SceneManager::ChangeScene(new TestScene());

    // 3. Game Loop
    while (gGameRunning) {

        AESysFrameStart();

        // check if forcing the application to quit
        if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
            gGameRunning = false;

        SceneManager::Update();


        bool balls = AEInputCheckTriggered(VK_SPACE);
        if (balls)
            std::cout << std::boolalpha << balls << std::endl;
        // Rendering
        AEGfxStart();
        AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
        SceneManager::Draw();

        AESysFrameEnd();
    }

    // 4. Cleanup
    SceneManager::Exit();
    MeshManager::DestroyInstance();
    Map::DestroyInstance();
    AESysExit();

    return 1;
}