#include "TestScene2.h"
#include "../System/Scenemanager.h"
#include "TestScene.h"
#include <iostream>

void TestScene2::Load() {
	AEGfxReset();

	meshManager = MeshManager::GetInstance();
	map = Map::GetInstance();
	player = Player::GetInstance();

	bg = new Background;
	bg->SetStatic(true);

	map->LoadMap("Level2");
}

void TestScene2::Init() {
	player->GetTransform().SetPosition(300.0f, 150.0f);
	abilityHandler = new AbilityHandler();
}

void TestScene2::Update() {
	float dt = (float)AEFrameRateControllerGetFrameTime();

	player->Update(dt);

	if (player->GetHealth() <= 0)
	{
		if (abilityHandler->Anchored())
		{
			abilityHandler->TriggerAnchor();
		}
		else
		{
			player->SetHealth(100);
		}
	}


	if (AEInputCheckTriggered(AEVK_9)) {
		SceneManager::ChangeScene(new TestScene());
		return;
	}
}

void TestScene2::Draw() {
	//AEGfxSetBackgroundColor(0.4f, 0.4f, 0.4f);
	bg->Draw(meshManager->GetMesh(TEXTURE_SQUARE));
	map->RenderMap();
	if (abilityHandler) abilityHandler->Draw();
	player->Draw(meshManager->GetMesh(CORNER_SQUARE));
}

void TestScene2::Free() {
	player->DestroyInstance();
	if (abilityHandler) delete abilityHandler;
	delete bg;
}

void TestScene2::Unload()
{
	player = nullptr;
	abilityHandler = nullptr;
	meshManager = nullptr;
	enemyList.clear();
}