#include "BaseScene.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include "../System/Scenemanager.h"
#include "BossScene.h"
#include "MainMenu.h"
#include "LoseScene.h"
#include "TutorialScene.h"
#include "../Utils/utils.h"
#include "../Enemy/ShadowEnemy.h"
#include "../Enemy/BatEnemy.h"
#include "../Enemy/ArcherEnemy.h"
#include "../particle/ParticleManager.h"
#include "../System/SettingsManager.h"

/*!
@file       BaseScene.cpp
@author     Lim Chee Kang (l.cheekang)
@brief      This file contains definitions of functions and variables
			used for the BaseScene class, which is the parent class
			of all scenes in the game, and contains common variables and
			functions used across all scenes
*//*_______________________________________________________________________*/

extern void ShutdownSwordResources(); //Declare external functions to free projectile resources
extern void ShutdownKnifeResources(); // (These are defined in their respective .cpp files, but are called in BaseScene's Free function to ensure they are cleaned up regardless of the current scene)
static int activeSaveSlot = 0;

BaseScene::BaseScene(int slot) : loadFromSlot(slot) {} //Constructor that sets the loadFromSlot variable, which is used to determine which save file to load from when the scene starts

//BaseScene destructor, used to clean up the nextScene pointer if it has been set by a derived scene
BaseScene::~BaseScene() {
	if (nextScene != nullptr) {
		delete nextScene;
		nextScene = nullptr;
	}
}

//BaseScene Load function, used to load assets and initialize variables that are common across all scenes, such as the player, map, mesh manager, audio manager, and background. Also loads the map based on the levelMapName variable, which is set by derived scenes in their constructors
void BaseScene::Load() {
	AEGfxReset();
	meshManager = MeshManager::GetInstance();
	map = Map::GetInstance();
	player = Player::GetInstance();
	audio = AudioManager::GetInstance();

	bg = new Background;
	map->LoadMap(levelMapName);

	for (s32 i = 0; i < map->GetNumRow(); ++i) {
		for (s32 j = 0; j < map->GetNumCol(); ++j) {
			if (map->GetMap()[i][j].tileType == TILE_TYPE::CLOSED_DOOR) {
				AEVec2Set(&DoorTilePosition, static_cast<f32>(i), static_cast<f32>(j));
				break;
			}
		}
	}

	//load all buttons
	Button::LoadResources();
	btnResume = new Button(0.0f, 0.0f, 200.0f, 50.0f, "Resume");
	btnSaveMenu = new Button(0.0f, 0.0f, 200.0f, 50.0f, "Save Game");
	btnLoadMenu = new Button(0.0f, 0.0f, 200.0f, 50.0f, "Load Game");
	btnExit = new Button(0.0f, 0.0f, 200.0f, 50.0f, "Quit to Menu");

	btnSlot1 = new Button(0.0f, 0.0f, 200.0f, 50.0f, "Slot 1");
	btnSlot2 = new Button(0.0f, 0.0f, 200.0f, 50.0f, "Slot 2");
	btnSlot3 = new Button(0.0f, 0.0f, 200.0f, 50.0f, "Slot 3");
	btnBack = new Button(0.0f, 0.0f, 200.0f, 50.0f, "Back");

	btnYes = new Button(0.0f, 0.0f, 150.0f, 50.0f, "Yes");
	btnNo = new Button(0.0f, 0.0f, 150.0f, 50.0f, "No");

	btnSettings = new Button(0.0f, 0.0f, 200.0f, 50.0f, "Settings");
	btnToggleMute = new Button(0.0f, 0.0f, 250.0f, 50.0f, "Mute: OFF");
	btnToggleFullscreen = new Button(0.0f, 0.0f, 250.0f, 50.0f, "Fullscreen: OFF");
	btnToggleCameraShake = new Button(0.0f, 0.0f, 250.0f, 50.0f, "Camera Shake: OFF");
	btnSettingsBack = new Button(0.0f, 0.0f, 200.0f, 50.0f, "Back");
}

void BaseScene::Init() {
	player->GetTransform().SetPosition(300.0f, 900.0f); //Set the player's initial position in the scene
	player->TriggerCameraShake(0.f, 0.f); //Set the player's camera shake to 0 at the start of the scene, to prevent any shake from carrying over from previous scenes

	audio->PlayMusic(levelBGM, 1.f); //Play the level's background music, with a fade-in time of 1 second

	abilityHandler = new AbilityHandler(); //Initialize the ability handler, which is used to manage the player's abilities and their interactions with enemies

	isPaused = false; //Initialize the paused state to false, since the game starts unpaused
	menuPage = 0; //Initialize the menu page to 0, which corresponds to the main pause menu (other pages include save menu, load menu, settings menu, and confirmation prompt)
	popupTimer = 0.0f; //Initialize the popup timer to 0, which is used to control how long the "Game Saved!" message is displayed on screen after saving the game

	// Iterate through the map and spawn enemies based on the tile types, then set those tiles to NONE to prevent respawning
	s32 maxRows = map->GetNumRow();
	for (s32 i = 0; i < maxRows; ++i) {

		s32 maxCols = map->GetNumCol();
		for (s32 j = 0; j < maxCols; ++j) {

			//Get the world position of the current tile, which is used as the spawn position for any enemies that need to be spawned on that tile
			AEVec2 pos = map->GetTileWorldPos(map->GetTilePos(map->GetMap()[i][j].transform.GetPosition().x, map->GetMap()[i][j].transform.GetPosition().y));

			if (map->GetMap()[i][j].tileType == TILE_TYPE::BAT_ENEMY) { //If the tile type is a bat enemy, spawn a new BatEnemy at that position, initialize it, and add it to the enemy list, then set the tile to NONE to prevent respawning
				BaseEnemy* batEnemy = new BatEnemy();
				batEnemy->Init(pos.x, pos.y, 85.f, 60.f, "batenemy");
				batEnemy->SetVelocity(0, 0);
				enemyList.push_back(batEnemy);
				map->GetMap()[i][j].tileType = TILE_TYPE::NONE;
			}
			else if (map->GetMap()[i][j].tileType == TILE_TYPE::ARCHER_ENEMY) { //If the tile type is an archer enemy, spawn a new ArcherEnemy at that position, initialize it, and add it to the enemy list, then set the tile to NONE to prevent respawning
				BaseEnemy* archerEnemy = new ArcherEnemy();
				archerEnemy->Init(pos.x, pos.y, 85.f, 60.f, "sniperenemy");
				archerEnemy->SetVelocity(0, 0);
				enemyList.push_back(archerEnemy);
				map->GetMap()[i][j].tileType = TILE_TYPE::NONE;
			}
			else if (map->GetMap()[i][j].tileType == TILE_TYPE::SHADOW_ENEMY) { //If the tile type is a shadow enemy, spawn a new ShadowEnemy at that position, initialize it, and add it to the enemy list, then set the tile to NONE to prevent respawning
				BaseEnemy* shadowEnemy = new ShadowEnemy();
				shadowEnemy->Init(pos.x, pos.y, 85.f, 60.f, "shadow1");
				shadowEnemy->SetVelocity(0, 0);
				enemyList.push_back(shadowEnemy);
				map->GetMap()[i][j].tileType = TILE_TYPE::NONE;
			}
		}
	}

	if (loadFromSlot > 0) { //If the loadFromSlot variable is greater than 0, which means we need to load from a save file, call the LoadGame function with the specified slot, then reset loadFromSlot to 0 to prevent accidentally loading again if the scene is reloaded for any reason
		LoadGame(loadFromSlot);
		loadFromSlot = 0;
	}
}

void BaseScene::Update() {
	if (canPause && AEInputCheckTriggered(AEVK_ESCAPE)) {
		isPaused = !isPaused;
		menuPage = 0;
	}

	f32 dt = (f32)AEFrameRateControllerGetFrameTime();

	if (dt > 0.033f) {
		dt = 0.01667f;
	}

	if (!isPaused) {
		audio->ResumeMusic();
		player->Update(dt);
		ParticleManager::GetInstance()->Update(dt);
		if (abilityHandler) abilityHandler->AbilityUpdate(enemyList, dt);

		for (BaseEnemy* enemy : enemyList) {
			enemy->Update(dt);
		}

		if (enemyList.size() == 0 && map->GetMap()[(int)DoorTilePosition.x][(int)DoorTilePosition.y].tileType == TILE_TYPE::CLOSED_DOOR) {
			AudioManager::GetInstance()->PlaySFXOneShot("door_open");
			map->SetTile(TILE_TYPE::OPEN_DOOR, DoorTilePosition.y, DoorTilePosition.x);
		}

		// door transition logic - only allow transitioning to the next scene if the player is colliding with the door tile and is physically close enough to it, to prevent accidental transitions from far away
		if (player->GetCollidedXTile().tileType == TILE_TYPE::OPEN_DOOR) {

			// Get the world position of the door and the player
			AEVec2 doorPos = map->GetMap()[(int)DoorTilePosition.x][(int)DoorTilePosition.y].transform.GetPosition();
			AEVec2 playerPos = player->GetTransform().GetPosition();

			// get the distance squared between the player and the door (using distance squared to avoid the costly square root operation, since we only need to compare it against a threshold distance squared)
			float distSq = (playerPos.x - doorPos.x) * (playerPos.x - doorPos.x) +
				(playerPos.y - doorPos.y) * (playerPos.y - doorPos.y);

			// only allow scene transition if the player is within a certain distance of the door (in this case, 100 units, which corresponds to a distance squared of 10000), to prevent accidental transitions when the player is far away but still colliding with the door tile
			if (distSq < 10000.0f) {
				if (nextScene != nullptr) {
					BaseScene* pNext = nextScene;
					nextScene = nullptr;
					SceneManager::ChangeScene(pNext);
				}
			}
		}

		if (AEInputCheckTriggered(AEVK_0)) { //debugging shortcut to skip directly to the boss scene
			SceneManager::ChangeScene(new BossScene());
		}
		if (AEInputCheckTriggered(AEVK_9)) //debugging shortcut to skip directly to the first level scene
		{
			SceneManager::ChangeScene(new Level1Scene());
		}
		for (BaseEnemy* enemy : enemyList) { //Iterate through all enemies and check for collisions between their projectiles and the player, applying damage and destroying projectiles as necessary. Also checks if the player is currently using the Reverse ability, in which case they are invulnerable to enemy projectiles and will not take damage or have projectiles destroyed on collision
			std::vector<EnemyProjectile*>& enemyProjs = enemy->GetProjectiles();
			for (auto it = enemyProjs.begin(); it != enemyProjs.end(); ) {
				EnemyProjectile* proj = *it;

				AEVec2 projPos = proj->transform.GetPosition();
				AEVec2 projHalf = { 20.0f, 20.0f };
				AEVec2 playerPos = player->GetTransform().GetPosition();
				AEVec2 playerHalf = { 35.0f, 35.0f };

				if (AABBOverlap(projPos, projHalf, playerPos, playerHalf) && !abilityHandler->Reverse()) {
					player->TakeDamage(3);
					delete proj;
					it = enemyProjs.erase(it);
				}
				else {
					++it;
				}
			}
		}

		//Check if the player's health has dropped to 0 or below, and if so, check if they are currently anchored. If they are anchored, trigger the anchor to teleport them back to safety. If they are not anchored, transition to the lose scene for the current level
		if (player->GetHealth() <= 0) {
			if (abilityHandler->Anchored()) {
				abilityHandler->TriggerAnchor();
			}
			else {
				SceneManager::ChangeScene(new LoseScene(levelID));
			}
		}

		//Iterate through all of the player's projectiles and check for collisions with enemies, applying damage and destroying projectiles as necessary. Also checks for collisions with boss weakpoints if the enemy is a boss, and applies different damage values based on whether the projectile is a sword or not
		std::vector<BaseProjectile*>& projs = player->projectileList;
		for (auto it = projs.begin(); it != projs.end(); ) {
			BaseProjectile* pProj = *it;
			AEVec2 projPos = pProj->transform.GetPosition();
			AEVec2 projHalf = pProj->transform.GetScale();
			AEVec2Scale(&projHalf, &projHalf, 0.5f);

			bool destroyProjectile = false;
			for (int j = 0; j < enemyList.size(); j++) {
				BaseEnemy* enemy = enemyList[j];
				if (enemy->IsDying()) continue;

				//Boss weakpoint collision behaviour
				BossEnemy* boss = dynamic_cast<BossEnemy*>(enemy);
				if (boss != nullptr)
				{
					if (boss->GetWeakpoint().active) //Only check for weakpoint collision if the weakpoint is currently active
					{
						Weakpoint const& weakpoint{ boss->GetWeakpoint() };
						AEVec2 bossWeakpointPos = weakpoint.transform.GetPosition();
						AEVec2 bossWeakpointHalf = { fabs(weakpoint.transform.GetScale().x) / 2.0f, weakpoint.transform.GetScale().y / 2.0f };

						if (AABBOverlap(projPos, projHalf, bossWeakpointPos, bossWeakpointHalf)) {
							if (dynamic_cast<SwordProjectile*>(pProj) == nullptr) {
								boss->TakeDamage(3);
								destroyProjectile = true;
								break;
							}
							else {
								boss->TakeDamage(5);
							}
						}
					}
				}
				else
				{
					AEVec2 enemyPos = enemy->transform.GetPosition();
					AEVec2 enemyHalf = { fabs(enemy->transform.GetScale().x) / 2.0f, enemy->transform.GetScale().y / 2.0f };

					if (AABBOverlap(projPos, projHalf, enemyPos, enemyHalf)) {
						if (dynamic_cast<SwordProjectile*>(pProj) == nullptr) {
							enemy->TakeDamage(3);
							destroyProjectile = true;
							break;
						}
						else {
							enemy->TakeDamage(5);
						}
					}
				}
			}

			//If the projectile has collided with an enemy and needs to be destroyed, or if its lifespan has dropped to 0 or below, delete the projectile and remove it from the player's projectile list. Otherwise, move on to the next projectile in the list
			if (destroyProjectile || pProj->GetLifespan() <= 0.f) {
				delete pProj;
				it = projs.erase(it);
			}
			else {
				++it;
			}
		}

		//Iterate through the enemy list and check for any enemies that are ready to be deleted (i.e. their death animation has finished), and if so, delete them and remove them from the enemy list
		for (auto it = enemyList.begin(); it != enemyList.end(); ) {
			if ((*it)->IsReadyToDelete()) {
				delete* it;
				it = enemyList.erase(it);
			}
			else {
				++it;
			}
		}
	}
	else {
		audio->PauseMusic();
		AEVec2 CamPos = { 0.0f, 0.0f };
		AEGfxGetCamPosition(&CamPos.x, &CamPos.y);

		//Display a different set of buttons depending on which menu page the player is currently on, and check for clicks on those buttons to perform the appropriate actions (e.g. resuming the game, saving/loading, changing settings, confirming exit, etc.)
		if (menuPage == 0) {
			btnResume->SetPosition(CamPos.x, CamPos.y + 120.0f);
			btnSaveMenu->SetPosition(CamPos.x, CamPos.y + 50.0f);
			btnLoadMenu->SetPosition(CamPos.x, CamPos.y - 20.0f);
			btnSettings->SetPosition(CamPos.x, CamPos.y - 90.0f);
			btnExit->SetPosition(CamPos.x, CamPos.y - 160.0f);

			if (btnResume->IsClicked()) isPaused = false; //If the resume button is clicked, unpause the game

			if (btnSaveMenu->IsClicked()) {
				// 1. Always open the Save Menu list
				menuPage = 1;

				// 2. Label the slots clearly so you know what you are overwriting
				btnSlot1->SetLabel(activeSaveSlot == 1 ? "Slot 1 (Active)" : (FileExists(1) ? "Slot 1 (Used)" : "Slot 1 (Empty)"));
				btnSlot2->SetLabel(activeSaveSlot == 2 ? "Slot 2 (Active)" : (FileExists(2) ? "Slot 2 (Used)" : "Slot 2 (Empty)"));
				btnSlot3->SetLabel(activeSaveSlot == 3 ? "Slot 3 (Active)" : (FileExists(3) ? "Slot 3 (Used)" : "Slot 3 (Empty)"));
			}

			//if the load menu button is clicked, switch to the load menu page and update the slot button labels to indicate whether each slot is currently used or empty (with different text than the save menu to indicate that these slots will be loaded from rather than saved to)
			if (btnLoadMenu->IsClicked()) {
				menuPage = 2;
				btnSlot1->SetLabel(FileExists(1) ? "Load Slot 1" : "Empty");
				btnSlot2->SetLabel(FileExists(2) ? "Load Slot 2" : "Empty");
				btnSlot3->SetLabel(FileExists(3) ? "Load Slot 3" : "Empty");
			}
			if (btnSettings->IsClicked()) menuPage = 4; //If the settings button is clicked, switch to the settings menu page
			if (btnExit->IsClicked()) menuPage = 3; //If the exit button is clicked, switch to the confirmation prompt page to confirm whether the player really wants to quit to the main menu, since doing so will lose all unsaved progress
		}
		//If the player is currently on the save menu page or the load menu page, display the slot buttons and the back button, and check for clicks on those buttons to perform the appropriate actions (e.g. saving/loading to/from the selected slot, going back to the main pause menu, etc.)
		else if (menuPage == 1 || menuPage == 2) {
			btnSlot1->SetPosition(CamPos.x, CamPos.y + 100.0f);
			btnSlot2->SetPosition(CamPos.x, CamPos.y + 30.0f);
			btnSlot3->SetPosition(CamPos.x, CamPos.y - 40.0f);
			btnBack->SetPosition(CamPos.x, CamPos.y - 110.0f);

			if (btnBack->IsClicked()) menuPage = 0;
			if (btnSlot1->IsClicked()) { if (menuPage == 1) { SaveGame(1); activeSaveSlot = 1; } else LoadGame(1); menuPage = 0; }
			if (btnSlot2->IsClicked()) { if (menuPage == 1) { SaveGame(2); activeSaveSlot = 2; } else LoadGame(2); menuPage = 0; }
			if (btnSlot3->IsClicked()) { if (menuPage == 1) { SaveGame(3); activeSaveSlot = 3; } else LoadGame(3); menuPage = 0; }
		}
		//If the player is currently on the confirmation prompt page, display the yes and no buttons, and check for clicks on those buttons to perform the appropriate actions (i.e. if yes is clicked, change to the main menu scene; if no is clicked, go back to the main pause menu)
		else if (menuPage == 3) {
			btnYes->SetPosition(CamPos.x - 100.0f, CamPos.y - 50.0f);
			btnNo->SetPosition(CamPos.x + 100.0f, CamPos.y - 50.0f);

			if (btnYes->IsClicked()) {
				activeSaveSlot = 0; // Clear memory when quitting to main menu
				SceneManager::ChangeScene(new MainMenu());
			}
			if (btnNo->IsClicked()) menuPage = 0;
		}
		//If the player is currently on the settings menu page, display the toggle mute, toggle fullscreen, toggle camera shake, and back buttons, and check for clicks on those buttons to perform the appropriate actions (e.g. toggling mute, toggling fullscreen, toggling camera shake, going back to the main pause menu, etc.). Also update the labels of the toggle buttons to reflect the current state of each setting
		else if (menuPage == 4) {
			btnToggleMute->SetPosition(CamPos.x, CamPos.y + 50.0f);
			btnToggleFullscreen->SetPosition(CamPos.x, CamPos.y - 20.0f);
			btnToggleCameraShake->SetPosition(CamPos.x, CamPos.y - 90.0f);
			btnSettingsBack->SetPosition(CamPos.x, CamPos.y - 160.0f);

			// Mute logic
			if (btnToggleMute->IsClicked()) {
				audio->SetMute();
			}
			btnToggleMute->SetLabel(audio->GetMuteStatus() ? "Mute: ON" : "Mute: OFF");

			// Fullscreen logic
			if (btnToggleFullscreen->IsClicked()) {
				bool isFullScreen = AESysIsFullScreen();
				AESysSetFullScreen(!isFullScreen);
			}

			// Update the fullscreen button label based on the new fullscreen state after toggling
			btnToggleFullscreen->SetLabel(AESysIsFullScreen() ? "Fullscreen: ON" : "Fullscreen: OFF");

			if (btnToggleCameraShake->IsClicked()) {
				SettingsManager::GetInstance()->SetCameraShake();
			}

			btnToggleCameraShake->SetLabel(SettingsManager::GetInstance()->GetCameraShake() ? "Camera Shake: ON" : "Camera Shake: OFF");

			//If the back button is clicked, go back to the main pause menu
			if (btnSettingsBack->IsClicked()) menuPage = 0;
		}
	}
}

//BaseScene Draw function, used to render the background, map, enemies, player, ability effects, and pause menu (if the game is currently paused) to the screen. Also renders the "Game Saved!" popup message for a short time after saving the game
void BaseScene::Draw() {

	if (!meshManager) return;

	if (bg && meshManager) {
		bg->Draw(meshManager->GetMesh(TEXTURE_SQUARE));
	}
	map->RenderMap();

	TutorialScene* tutScene = dynamic_cast<TutorialScene*>(this);

	if (tutScene != nullptr)
	{
		tutScene->RenderTutorialText();
	}

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);

	ParticleManager::GetInstance()->Draw(meshManager->GetMesh(SQUARE));
	for (BaseEnemy* enemy : enemyList) {
		enemy->Draw();
	}

	if (abilityHandler && !isPaused) abilityHandler->Draw();
	player->Draw(meshManager->GetMesh(CORNER_SQUARE));

	if (isPaused) {
		AEVec2 CamPos = { 0.0f, 0.0f };
		AEGfxGetCamPosition(&CamPos.x, &CamPos.y);

		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(0.6f);
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
		AEGfxSetColorToMultiply(0.0f, 0.0f, 0.0f, 1.0f);

		AEMtx33 scale, trans, final;
		AEMtx33Scale(&scale, 3000.0f, 3000.0f);
		AEMtx33Trans(&trans, CamPos.x, CamPos.y);
		AEMtx33Concat(&final, &trans, &scale);
		AEGfxSetTransform(final.m);

		AEGfxMeshDraw(meshManager->GetMesh(SQUARE), AE_GFX_MDM_TRIANGLES);

		AEGfxSetTransparency(1.0f);
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

		if (menuPage == 0) {
			btnResume->Draw(); btnSaveMenu->Draw(); btnLoadMenu->Draw(); btnSettings->Draw(); btnExit->Draw();
		}
		else if (menuPage == 1 || menuPage == 2) {
			btnSlot1->Draw(); btnSlot2->Draw(); btnSlot3->Draw(); btnBack->Draw();
		}
		else if (menuPage == 3) {
			btnYes->Draw();
			btnNo->Draw();

			char line1[64];
			snprintf(line1, sizeof(line1), "Are you sure you want to quit?");
			float w1 = 0.0f, h1 = 0.0f;
			AEGfxGetPrintSize(Button::fontId, line1, 1.0f, &w1, &h1);
			AEGfxPrint(Button::fontId, line1, -(w1 / 2.0f), 0.15f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

			char line2[64];
			snprintf(line2, sizeof(line2), "Unsaved progress will be lost.");
			float w2 = 0.0f, h2 = 0.0f;
			AEGfxGetPrintSize(Button::fontId, line2, 1.0f, &w2, &h2);
			AEGfxPrint(Button::fontId, line2, -(w2 / 2.0f), 0.05f, 1.0f, 1.0f, 0.5f, 0.5f, 1.0f);
		}
		else if (menuPage == 4) {
			btnToggleMute->Draw();
			btnToggleFullscreen->Draw();
			btnToggleCameraShake->Draw();
			btnSettingsBack->Draw();

			char title[64];
			snprintf(title, sizeof(title), "Settings");
			float wt = 0.0f, ht = 0.0f;
			AEGfxGetPrintSize(Button::fontId, title, 1.0f, &wt, &ht);
			AEGfxPrint(Button::fontId, title, -(wt / 2.0f), 0.25f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		}

		if (popupTimer > 0.0f) {
			char msg[64];
			snprintf(msg, sizeof(msg), "Game Saved!");
			float wm = 0.0f, hm = 0.0f;
			AEGfxGetPrintSize(Button::fontId, msg, 1.0f, &wm, &hm);
			AEGfxPrint(Button::fontId, msg, -(wm / 2.0f), 0.35f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f);
			popupTimer -= (f32)AEFrameRateControllerGetFrameTime();
		}
	}
}

//BaseScene SaveGame function, used to save the current game state to a text file corresponding to the specified save slot. The saved data includes the current level ID, the player's position, and the player's health. After saving, sets the popup timer to display the "Game Saved!" message for a short time
void BaseScene::SaveGame(int slot) {
	std::string path = "save_slot_" + std::to_string(slot) + ".txt";
	std::ofstream f(path);
	if (f.is_open()) {
		AEVec2 pos = player->GetTransform().GetPosition();

		// Safely get mana (defaults to 100 if abilityHandler isn't loaded yet)
		float currentMana = abilityHandler ? abilityHandler->GetResource() : 100.0f;

		// Write Player Data + Mana
		f << levelID << " " << pos.x << " " << pos.y << " " << player->GetHealth() << " " << currentMana << "\n";

		// Only count enemies that are actually alive
		int aliveCount = 0;
		for (BaseEnemy* enemy : enemyList) {
			if (enemy != nullptr && !enemy->IsDying() && enemy->GetHealth() > 0) {
				aliveCount++;
			}
		}

		// Write the true amount of living enemies
		f << aliveCount << "\n";

		for (BaseEnemy* enemy : enemyList) {
			// Skip nulls, and skip enemies that are currently in their death animation
			if (enemy == nullptr || enemy->IsDying() || enemy->GetHealth() <= 0) continue;

			int typeID = 0;

			// Figure out what type of enemy this is
			if (dynamic_cast<BatEnemy*>(enemy)) typeID = 1;
			else if (dynamic_cast<ArcherEnemy*>(enemy)) typeID = 2;
			else if (dynamic_cast<ShadowEnemy*>(enemy)) typeID = 3;
			else if (dynamic_cast<BossEnemy*>(enemy)) typeID = 4;

			AEVec2 ePos = enemy->transform.GetPosition();
			f << typeID << " " << ePos.x << " " << ePos.y << " " << enemy->GetHealth() << "\n";
		}

		f.close();
		popupTimer = 2.0f;
	}
}

void BaseScene::LoadGame(int slot) {
	std::string path = "save_slot_" + std::to_string(slot) + ".txt";
	std::ifstream f(path);
	activeSaveSlot = slot;
	if (f.is_open()) {

		int savedLevelID;
		float x, y, hp, mana;

		// Read Player Data + Mana
		if (f >> savedLevelID >> x >> y >> hp >> mana) {

			player->GetTransform().SetPosition(x, y);
			player->SetHealth(hp);
			isPaused = false;

			if (savedLevelID != this->levelID) {
				// Change to the correct scene and pass the slot so it loads when it initializes
				if (savedLevelID == 1) SceneManager::ChangeScene(new Level1Scene(slot));
				else if (savedLevelID == 0) SceneManager::ChangeScene(new TutorialScene(slot));
				else if (savedLevelID == 2) SceneManager::ChangeScene(new BossScene(slot));
			}
			else {
				// 1. Delete the default enemies spawned by the map
				for (BaseEnemy* enemy : enemyList) {
					delete enemy;
				}
				enemyList.clear();

				// 2. Read the saved enemies and spawn them exactly where they were
				int numEnemies = 0;
				if (f >> numEnemies) {
					for (int i = 0; i < numEnemies; ++i) {
						int typeID;
						float eX, eY;
						int eHP;
						f >> typeID >> eX >> eY >> eHP;

						BaseEnemy* newEnemy = nullptr;
						if (typeID == 1) { newEnemy = new BatEnemy(); newEnemy->Init(eX, eY, 85.f, 60.f, "batenemy"); }
						else if (typeID == 2) { newEnemy = new ArcherEnemy(); newEnemy->Init(eX, eY, 85.f, 60.f, "sniperenemy"); }
						else if (typeID == 3) { newEnemy = new ShadowEnemy(); newEnemy->Init(eX, eY, 85.f, 60.f, "shadow1"); }
						else if (typeID == 4) { newEnemy = new BossEnemy(); newEnemy->Init(eX, eY, 85.f, 60.f, "boss"); }

						if (newEnemy) {
							newEnemy->SetHealth(eHP);
							enemyList.push_back(newEnemy);
						}
					}
				}

				// cancel any active time-freezes and set the loaded mana
				if (abilityHandler) {
					delete abilityHandler;
					abilityHandler = new AbilityHandler();

					//Give the fresh handler your saved mana
					abilityHandler->SetResource(mana);
				}

				// Wipe active particles and thrown projectiles from the screen
				ParticleManager::GetInstance()->ClearAll();
				player->ClearProjectileList();
			}
		}
		f.close();
	}
}

//BaseScene FileExists function, used to check if a save file corresponding to the specified slot exists and can be opened successfully. Returns true if the file exists and can be opened, false otherwise
bool BaseScene::FileExists(int slot) const {
	std::ifstream f("save_slot_" + std::to_string(slot) + ".txt");
	return f.good();
}

//BaseScene Free function, used to clean up dynamically allocated memory for the ability handler, player projectiles, enemies, background, and particle manager when the scene is being unloaded. Also calls external functions to free resources for sword and knife projectiles, since those resources are shared across all scenes and need to be freed regardless of which scene is currently active
void BaseScene::Free() {
	if (abilityHandler)
	{
		delete abilityHandler;
	}

	if (player != nullptr)
	{
		player->ClearProjectileList();
	}

	for (BaseEnemy* enemy : enemyList) {
		delete enemy;
	}
	enemyList.clear();

	if (bg) {
		delete bg;
		bg = nullptr;
	}

	ParticleManager::GetInstance()->ClearAll();
}

//BaseScene Unload function, used to clean up dynamically allocated memory for buttons and other resources that are used in the pause menu, as well as resetting pointers to the player, ability handler, and mesh manager to nullptr, and clearing the enemy list. Also calls external functions to free resources for sword and knife projectiles, since those resources are shared across all scenes and need to be freed regardless of which scene is currently active
void BaseScene::Unload() {

	delete btnResume; delete btnSaveMenu; delete btnLoadMenu; delete btnExit;
	delete btnSlot1; delete btnSlot2; delete btnSlot3; delete btnBack;
	delete btnYes; delete btnNo;

	delete btnSettings;
	delete btnToggleMute;
	delete btnToggleFullscreen;
	delete btnToggleCameraShake;
	delete btnSettingsBack;

	player = nullptr;
	abilityHandler = nullptr;
	meshManager = nullptr;
	enemyList.clear();

	ShutdownSwordResources();
	ShutdownKnifeResources();
}