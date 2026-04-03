#include "BossScene.h"
#include "../System/Scenemanager.h"
#include "WinScene.h" 

void BossScene::Init() {
    BaseScene::Init();
    player->GetTransform().SetPosition(300.0f, 200.0f);
    player->SetMoveable(false);
    abilityHandler->ShowUI(false);
    boss = new BossEnemy();
    boss->Init(1400.f, 300.0f, 85.0f, 60.0f, "boss");
    enemyList.push_back(boss);
    boss->isBoss = true;
	levelID = 2;
    //AudioManager::GetInstance()->PlaySFXOneShot("");
}

void BossScene::Update() {
    // drain pending enemy spawns into enemylist of scene BEFORE scene update
    if (boss)
    {
        for (BaseEnemy* e : boss->GetPendingSpawns())
            enemyList.push_back(e);
        boss->GetPendingSpawns().clear();
    }

    BaseScene::Update();

    f32 dt = (f32)AEFrameRateControllerGetFrameTime();


    if (cutsceneDuration >= 0.f && boss)
    {
        AEGfxSetCamPosition(boss->transform.GetPosition().x, boss->transform.GetPosition().y);
        cutsceneDuration -= 1 * dt;
        if (cutsceneDuration < 0.f)
        {
            player->SetMoveable(true);
            abilityHandler->ShowUI(true);
        }
    }

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