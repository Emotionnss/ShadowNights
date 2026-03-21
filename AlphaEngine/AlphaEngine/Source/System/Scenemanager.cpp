#include "SceneManager.h"
#include "AEEngine.h"
#include "../Scene/BaseScene.h"

BaseScene* SceneManager::pCurrentScene = nullptr;
BaseScene* SceneManager::pNextScene = nullptr;

void SceneManager::ChangeScene(BaseScene* pNewScene) {
    // Just store the pointer for later at main
    pNextScene = pNewScene;
}

void SceneManager::Update() {
    // Handle the switch at a safe time (start of the frame)
    if (pNextScene != nullptr) {
        // 1. Clean up old scene
        if (pCurrentScene) {
            pCurrentScene->Free();
            pCurrentScene->Unload();
            delete pCurrentScene;
        }

        // 2. Switch to new scene
        pCurrentScene = pNextScene;
        pNextScene = nullptr;

        // 3. Prepare new scene
        if (pCurrentScene) {
            pCurrentScene->Load();
            pCurrentScene->Init();
        }
    }

    // Run the actual game logic for the current scene
    if (pCurrentScene) {
        pCurrentScene->Update();
    }
}

void SceneManager::Draw() {
    // Only draw if the pointer is valid
    if (pCurrentScene) {
        pCurrentScene->Draw();
    }
}

void SceneManager::Exit() {
    // Final cleanup when the game closes
    if (pCurrentScene) {
        pCurrentScene->Free();
        pCurrentScene->Unload();
        delete pCurrentScene;
        pCurrentScene = nullptr;
    }
}