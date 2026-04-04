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

extern bool gGameRunning; //External variable to control the main game loop, set to false when quitting the game

//Helper function to check if a save file exists for a given slot
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

	bg = new Background("Assets/Sprites/Background/Dark Gothic Castle.png"); //Load the menu background
	if (Button::fontId == -1) Button::LoadResources(); //Load button resources if they haven't been loaded yet

	float startY = -110.0f;
	float spacing = 60.0f;
	float width = 200.0f;
	float height = 50.0f;

	//all buttons initialized with the same spacing, width, and height, but different labels and starting positions (starting off-screen for animation)
	btnPlay = new Button(0.0f, startY, width, height, "New Game");
	btnLoad = new Button(0.0f, startY - spacing, width, height, "Load Game");
	btnSettings = new Button(0.0f, startY - (spacing * 2), width, height, "Settings");
	btnCredits = new Button(0.0f, startY - (spacing * 3), width, height, "Credits");
	btnQuit = new Button(0.0f, startY - (spacing * 4), width, height, "Quit");

	btnSlot1 = new Button(0.0f, startY, width, height, "Slot 1");
	btnSlot2 = new Button(0.0f, startY - spacing, width, height, "Slot 2");
	btnSlot3 = new Button(0.0f, startY - (spacing * 2), width, height, "Slot 3");
	btnBack = new Button(0.0f, startY - (spacing * 3), width, height, "Back");

	// Confirmation Buttons (Pushed slightly further down)
	btnYes = new Button(-100.0f, startY - (spacing * 2.5f), 150.0f, height, "Yes");
	btnNo = new Button(100.0f, startY - (spacing * 2.5f), 150.0f, height, "No");

	logo = AEGfxTextureLoad("Assets/Credits/logo.png");
	logoTfm.SetPosition(0.f, 200.f);
	logoTfm.SetScale(600.f, 400.f);
}

void MainMenu::Init() {
	menuPage = 0;
	animT = 0.0f;
}

void MainMenu::Update() {
	f32 dt = (f32)AEFrameRateControllerGetFrameTime(); //Get the delta time for the current frame, used for animations and timing
	if (dt > 0.033f) dt = 0.01667f; //Cap the delta time to prevent issues with very low frame rates (e.g. when debugging or on very slow machines), which could cause animations to jump or skip frames

	// Increment the animation timer if it's less than 1, to drive the slide-in animations for the menu buttons
	if (animT < 1.0f) {
		animT += dt * 1.5f;
		if (animT > 1.0f) animT = 1.0f;
	}

	float ease = 1.0f - pow(1.0f - animT, 3.0f);
	float fromRight = 800.0f * (1.0f - ease);
	float fromLeft = -800.0f * (1.0f - ease);

	//set button positions based on the current menu page and the animation timer, with buttons sliding in from the left or right depending on their position in the menu. Also check for button clicks and handle them accordingly (e.g. starting a new game, loading a save, opening settings, etc.)
	float startY = -110.0f;
	float spacing = 60.0f;

	// Main Menu
	if (menuPage == 0) {
		btnPlay->SetPosition(fromRight, startY);
		btnLoad->SetPosition(fromLeft, startY - spacing);
		btnSettings->SetPosition(fromRight, startY - (spacing * 2));
		btnCredits->SetPosition(fromLeft, startY - (spacing * 3));
		btnQuit->SetPosition(fromRight, startY - (spacing * 4));

		if (btnPlay->IsClicked()) {
			DeleteSave(1); DeleteSave(2); DeleteSave(3);
			SceneManager::ChangeScene(new TutorialScene());
		}
		if (btnLoad->IsClicked()) {
			menuPage = 2;
			animT = 0.0f;
			btnSlot1->SetLabel(FileExists(1) ? "Load Slot 1" : "Empty");
			btnSlot2->SetLabel(FileExists(2) ? "Load Slot 2" : "Empty");
			btnSlot3->SetLabel(FileExists(3) ? "Load Slot 3" : "Empty");
		}
		if (btnSettings->IsClicked()) {
			SceneManager::ChangeScene(new SettingsScene());
		}
		if (btnCredits->IsClicked()) {
			SceneManager::ChangeScene(new CreditsScene());
		}
		if (btnQuit->IsClicked()) {
			menuPage = 3;
			animT = 0.0f;
		}
	}
	// Load Menu
	else if (menuPage == 1 || menuPage == 2) {
		btnSlot1->SetPosition(fromRight, startY);
		btnSlot2->SetPosition(fromLeft, startY - spacing);
		btnSlot3->SetPosition(fromRight, startY - (spacing * 2));
		btnBack->SetPosition(fromLeft, startY - (spacing * 3));

		if (btnBack->IsClicked()) {
			menuPage = 0;
			animT = 0.0f;
		}

		if (btnSlot1->IsClicked() && FileExists(1)) SceneManager::ChangeScene(new TutorialScene(1));
		if (btnSlot2->IsClicked() && FileExists(2)) SceneManager::ChangeScene(new TutorialScene(2));
		if (btnSlot3->IsClicked() && FileExists(3)) SceneManager::ChangeScene(new TutorialScene(3));
	}
	// Quit Confirmation
	else if (menuPage == 3) {
		btnYes->SetPosition(-100.0f, startY - (spacing * 2.5f));
		btnNo->SetPosition(100.0f, startY - (spacing * 2.5f));

		if (btnYes->IsClicked()) gGameRunning = false;
		if (btnNo->IsClicked()) {
			menuPage = 0;
			animT = 0.0f;
		}
	}
}

void MainMenu::Draw() {
	AEGfxSetCamPosition(0.0f, 0.0f);
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	if (bg && meshManager) {
		bg->Draw(meshManager->GetMesh(TEXTURE_SQUARE));
	}

	// Draw buttons based on the current menu page
	if (menuPage == 0) {
		btnPlay->Draw();
		btnLoad->Draw();
		btnSettings->Draw();
		btnCredits->Draw();
		btnQuit->Draw();
	}
	// Draw save/load menu
	else if (menuPage == 1 || menuPage == 2) {
		btnSlot1->Draw();
		btnSlot2->Draw();
		btnSlot3->Draw();
		btnBack->Draw();
	}
	// Draw quit confirmation
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

// Frees dynamically allocated memory for the menu buttons and unloads the logo texture from graphics memory. Also calls Button::FreeResources() to free any static resources used by the Button class.
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