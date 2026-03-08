#include "TestScene.h"
#include <cmath>
#include <iostream>
#include "../System/Scenemanager.h"
#include "TestScene2.h"
#include "BossScene.h"
#include "../Utils/utils.h"
#include "../Enemy/ShadowEnemy.h"
#include "../Enemy/BatEnemy.h"
#include "../Enemy/ArcherEnemy.h"


void TestScene::Load() {
	AEGfxReset();
	meshManager = MeshManager::GetInstance();
	map = Map::GetInstance();
	player = Player::GetInstance();

	bg = new Background;

	map->LoadMap("Level1");

	// Collects the Door Tile <Will change to function or refactor> :)
	for (s32 i = 0; i < map->GetNumRow(); ++i)
	{
		for (s32 j = 0; j < map->GetNumCol(); ++j)
		{
			if (map->GetMap()[i][j].tileType == TILE_TYPE::CLOSED_DOOR)
			{
				AEVec2Set(&DoorTilePosition, static_cast<f32>(i), static_cast<f32>(j));
				return;
			}
		}
	}
}

void TestScene::Init() {
	player->GetTransform().SetPosition(300.0f, 900.0f);
	abilityHandler = new AbilityHandler();

	// ENEMY LIST SPAWN
	int numberOfEnemies = 3;
	float startX = 600.0f;
	float spacing = 300.0f;

	for (int e = 0; e < numberOfEnemies; e++)
	{
		BaseEnemy* newEnemy = new ShadowEnemy();
		BaseEnemy* myBat = new BatEnemy();
		BaseEnemy* myArcher = new ArcherEnemy();

		float spawnX = startX + (e * spacing);

		newEnemy->Init(spawnX, 300.0f, 85.0f, 60.0f, "shadow1");
		newEnemy->SetVelocity(0.0f, 0.0f);
		enemyList.push_back(newEnemy);

		myBat->Init(spawnX, 700.0f, 85.0f, 60.0f, "batenemy");
		enemyList.push_back(myBat);

		myArcher->Init(spawnX + 150.0f, 300.0f, 85.0f, 60.0f, "sniperenemy");
		enemyList.push_back(myArcher);
	}
}

void TestScene::Update() {
	f32 dt = (f32)AEFrameRateControllerGetFrameTime();

	player->Update(dt);
	if (abilityHandler) abilityHandler->AbilityUpdate(enemyList, dt);

	// UPDATE ALL ENEMIES (IMPT)
	for (BaseEnemy* enemy : enemyList) {
		enemy->Update(dt);
	}

	// Open Door Once Enemy has been defeated
	if (enemyList.size() == 0)
	{
		map->SetTile(TILE_TYPE::OPEN_DOOR, DoorTilePosition.y, DoorTilePosition.x);
	}

	// Change scene when player collides with an open door
	if (player->GetCollidedTile().tileType == TILE_TYPE::OPEN_DOOR)
	{
		SceneManager::ChangeScene(new TestScene2());
	}

	//Delete later
	if (AEInputCheckTriggered(AEVK_0))
	{
		SceneManager::ChangeScene(new BossScene());
	}


	// N***A enemy projectile
	for (BaseEnemy* enemy : enemyList)
	{
		std::vector<EnemyProjectile*>& enemyProjs = enemy->GetProjectiles();

		for (auto it = enemyProjs.begin(); it != enemyProjs.end(); )
		{
			EnemyProjectile* proj = *it;

			AEVec2 projPos = proj->transform.GetPosition();
			AEVec2 projHalf = { 20.0f, 20.0f };

			AEVec2 playerPos = player->GetTransform().GetPosition();
			AEVec2 playerHalf = { 35.0f, 35.0f };

			if (AABBOverlap(projPos, projHalf, playerPos, playerHalf) && !abilityHandler->Reverse())
			{
				std::cout << "OUCH! Player Hit!" << std::endl;
				player->TakeDamage(10);

				delete proj;
				it = enemyProjs.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
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
	// SWORD PEEPEEPOOPOO
	std::vector<BaseProjectile*>& projs = player->projectileList;

	for (auto it = projs.begin(); it != projs.end(); )
	{
		BaseProjectile* pProj = *it;

		AEVec2 projPos = pProj->transform.GetPosition();
		AEVec2 projHalf = pProj->transform.GetScale();
		AEVec2Scale(&projHalf, &projHalf, 0.5f);

		bool destroyProjectile = false;
		for (int j = 0; j < enemyList.size(); j++)
		{
			BaseEnemy* enemy = enemyList[j];

			AEVec2 enemyPos = enemy->transform.GetPosition();
			AEVec2 enemyHalf = { fabs(enemy->transform.GetScale().x) / 2.0f, enemy->transform.GetScale().y / 2.0f };

			if (AABBOverlap(projPos, projHalf, enemyPos, enemyHalf))
			{
				if (!enemy->IsReadyToDelete())
				{
					enemy->TakeDamage();

					SwordProjectile* swordProj = dynamic_cast<SwordProjectile*>(pProj);
					if (!swordProj)
					{
						destroyProjectile = true;
						break;
					}
				}
			}
		}

		// erase the projectile (zav is gay)
		if (destroyProjectile || pProj->GetLifespan() <= 0.f)
		{
			delete pProj;
			it = projs.erase(it);
		}
		else
		{
			++it;
		}
	}

	// enemy die alr or nut punde
	for (auto it = enemyList.begin(); it != enemyList.end(); )
	{
		if ((*it)->IsReadyToDelete())
		{
			delete* it;
			it = enemyList.erase(it);
			std::cout << "Enemy Died and Removed!" << std::endl;
		}
		else
		{
			++it;
		}
	}
}

void TestScene::Draw() {
	//AEGfxSetBackgroundColor(0.4f, 0.4f, 0.4f);
	bg->Draw(meshManager->GetMesh(TEXTURE_SQUARE));
	map->RenderMap();

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);


	for (BaseEnemy* enemy : enemyList) {
		enemy->Draw();
	}
	if (abilityHandler) abilityHandler->Draw();
	player->Draw(meshManager->GetMesh(CORNER_SQUARE));
}

void TestScene::Free() {
	player->DestroyInstance();
	if (abilityHandler) delete abilityHandler;

	// CLEANUP KUKUBIRD CCB KNN OBAMA RASENGAN
	for (BaseEnemy* enemy : enemyList) {
		delete enemy;
	}
	enemyList.clear();
	delete bg;
}

void TestScene::Unload()
{
	player = nullptr;
	abilityHandler = nullptr;
	meshManager = nullptr;
	enemyList.clear();
}