#include "AEEngine.h"
#include "System/SceneManager.h"
#include "System/MeshManager.h"
#include "Scene/TestScene.h"
#include <iostream>
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpCmd, int nShow) {
    // 1. Initialize
    if (AESysInit(hInst, nShow, 1600, 900, 1, 60, false, NULL) == 0) {
        return 0;
    }

    // 2. Set Scene
    SceneManager::ChangeScene(new TestScene());

    // 3. Game Loop
    while (AESysDoesWindowExist()) {

        AESysFrameStart();
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
    AESysExit();

    return 1;
}