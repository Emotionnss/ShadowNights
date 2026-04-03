#include "MainMenu.h"
#include "../System/Scenemanager.h"
#include "TutorialScene.h"
#include "Level1Scene.h"
#include "Credits.h"
#include "SettingsScene.h" 
#include "AEEngine.h"
#include <iostream>
#include <fstream>
#include <string>

/*!
@file       MainMenu.cpp
@author     Lim Chee Kang (l.cheekang)
@brief      This file contains definitions of functions and variables
			used for the MainMenu class, which is the first scene of the game,
			and contains buttons to start a new game, load from a save, access settings,
			view credits, or quit the game.
*//*_______________________________________________________________________*/

extern bool gGameRunning;

void MainMenu::DeleteSave(int slot) {
	std::string path = "save_slot_" + std::to_string(slot) + ".txt";
	std::remove(path.c_str());
}

void MainMenu::Load() {
	AEGfxReset();

	meshManager = MeshManager::GetInstance();
	audio = AudioManager::GetInstance();

	if (audio->GetCurrentMusicClip().name != "menu_music")
	{
		audio->PlayMusic("menu_music");
	}

	bg = new Background("Assets/Sprites/Background/Dark Gothic Castle.png");
	if (Button::fontId == -1) Button::LoadResources();

	btnPlay = new Button(0.0f, 90.0f - 200.f, 200.0f, 50.0f, "New Game");//-200.f cause cheekang dk how to base y value off previous BUTTON LOOKING AT U CHEE KANG
	btnLoad = new Button(0.0f, 30.0f - 200.f, 200.0f, 50.0f, "Load Game");
	btnSettings = new Button(0.0f, -30.0f - 200.f, 200.0f, 50.0f, "Settings");
	btnCredits = new Button(0.0f, -90.0f - 200.f, 200.0f, 50.0f, "Credits");
	btnQuit = new Button(0.0f, -150.0f - 200.f, 200.0f, 50.0f, "Quit");

	btnSlot1 = new Button(0.0f, 90.0f - 200.f, 200.0f, 50.0f, "Slot 1");
	btnSlot2 = new Button(0.0f, 30.0f - 200.f, 200.0f, 50.0f, "Slot 2");
	btnSlot3 = new Button(0.0f, -30.0f - 200.f, 200.0f, 50.0f, "Slot 3");
	btnBack = new Button(0.0f, -90.0f - 200.f, 200.0f, 50.0f, "Back");

	// Confirmation Buttons
	btnYes = new Button(-100.0f, -50.0f - 200.f, 150.0f, 50.0f, "Yes");
	btnNo = new Button(100.0f, -50.0f - 200.f, 150.0f, 50.0f, "No");

	logo = AEGfxTextureLoad("Assets/Credits/logo.png");
	logoTfm.SetPosition(0.f, 200.f);
	logoTfm.SetScale(600.f, 400.f);
}

void MainMenu::Init() {
	menuPage = 0;
}

void MainMenu::Update() {
	if (menuPage == 0) {
		if (btnPlay->IsClicked()) {
			DeleteSave(1);
			DeleteSave(2);
			DeleteSave(3);
			SceneManager::ChangeScene(new TutorialScene());
		}
		if (btnLoad->IsClicked()) {
			menuPage = 2;
			btnSlot1->SetLabel(FileExists(1) ? "Load Slot 1" : "Empty");
			btnSlot2->SetLabel(FileExists(2) ? "Load Slot 2" : "Empty");
			btnSlot3->SetLabel(FileExists(3) ? "Load Slot 3" : "Empty");
		}
		if (btnSettings->IsClicked()) {
			SceneManager::ChangeScene(new SettingsScene());
		}
		if (btnCredits->IsClicked())
		{
			SceneManager::ChangeScene(new CreditsScene());
		}
		if (btnQuit->IsClicked()) {
			menuPage = 3;
		}
	}
	else if (menuPage == 1 || menuPage == 2) {
		if (btnBack->IsClicked()) menuPage = 0;

		if (btnSlot1->IsClicked() && FileExists(1)) SceneManager::ChangeScene(new TutorialScene(1));
		if (btnSlot2->IsClicked() && FileExists(2)) SceneManager::ChangeScene(new TutorialScene(2));
		if (btnSlot3->IsClicked() && FileExists(3)) SceneManager::ChangeScene(new TutorialScene(3));
	}
	else if (menuPage == 3) {
		if (btnYes->IsClicked()) gGameRunning = false;
		if (btnNo->IsClicked()) menuPage = 0;
	}
}

void MainMenu::Draw() {
	AEGfxSetCamPosition(0.0f, 0.0f);
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	if (bg && meshManager) {
		bg->Draw(meshManager->GetMesh(TEXTURE_SQUARE));
	}

	if (menuPage == 0) {
		btnPlay->Draw();
		btnLoad->Draw();
		btnSettings->Draw();
		btnCredits->Draw();
		btnQuit->Draw();
	}
	else if (menuPage == 1 || menuPage == 2) {
		btnSlot1->Draw();
		btnSlot2->Draw();
		btnSlot3->Draw();
		btnBack->Draw();
	}
	else if (menuPage == 3) {
		btnYes->Draw();
		btnNo->Draw();

		char line1[64];
		snprintf(line1, sizeof(line1), "Quit to Desktop?");
		float w1 = 0.0f, h1 = 0.0f;
		AEGfxGetPrintSize(Button::fontId, line1, 1.0f, &w1, &h1);
		AEGfxPrint(Button::fontId, line1, -(w1 / 2.0f), -0.25f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	}

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.f, 0.f, 0.f, 0.0f);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1);

	AEGfxTextureSet(logo, 0, 0);

	AEGfxSetTransform(logoTfm.GetMatrix().m);
	AEGfxMeshDraw(MeshManager::GetInstance()->GetMesh(TEXTURE_SQUARE), AE_GFX_MDM_TRIANGLES);
}

void MainMenu::Free() {
	delete btnPlay; delete btnLoad; delete btnSettings; delete btnCredits; delete btnQuit;
	delete btnSlot1; delete btnSlot2; delete btnSlot3; delete btnBack;
	delete btnYes; delete btnNo;

	if (bg) {
		delete bg;
		bg = nullptr;
	}
	if (logo)
	{
		AEGfxTextureUnload(logo);
	}
	Button::FreeResources();
}

void MainMenu::Unload() {}