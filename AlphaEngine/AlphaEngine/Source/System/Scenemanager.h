
#pragma once
#include "../../Source/Scene/BaseScene.h"

class SceneManager {
public:
    // Replaces SetCurrentScene to handle transitions safely
    static void ChangeScene(BaseScene* pNewScene);

    // Call this every frame in your WinMain loop
    static void Update();

    // Call this inside the AEGfxStart/End block in WinMain
    static void Draw();

    // Call this once before AESysExit to clean up the final scene
    static void Exit();

private:
    static BaseScene* pCurrentScene;
    static BaseScene* pNextScene; // Buffer to hold the incoming scene
};
