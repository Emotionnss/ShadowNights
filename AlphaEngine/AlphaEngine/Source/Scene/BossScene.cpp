#include "BossScene.h"
#include "../System/Scenemanager.h"
#include "WinScene.h" 

void BossScene::Init() {
    BaseScene::Init();
    player->GetTransform().SetPosition(300.0f, 200.0f);

    boss = new BossEnemy();
    boss->Init(1400.f, 300.0f, 85.0f, 60.0f, "boss");
    enemyList.push_back(boss);
}

void BossScene::Update() {
    BaseScene::Update();

    if (boss && boss->IsReadyToDelete()) {
        SceneManager::ChangeScene(new WinScene());
        return;
    }
}

void BossScene::Draw() {
    AEGfxSetBackgroundColor(0.2f, 0.0f, 0.0f);
    BaseScene::Draw();
}

void BossScene::Free() {
    BaseScene::Free();
    boss = nullptr;
}