#include "SettingsScene.h"
#include "../System/Scenemanager.h"
#include "MainMenu.h"
#include "../System/AudioManager.h"
#include "../System/SettingsManager.h"

SettingsScene::SettingsScene(int slot) : BaseScene(slot) {
	// not needed for this scene, but we can set it to an empty string to avoid confusion
	levelMapName = "";
}

SettingsScene::~SettingsScene() {
}

void SettingsScene::Load() {
	//load any resources you need here (fonts, meshes, audio, etc)
	meshManager = MeshManager::GetInstance();
	audio = AudioManager::GetInstance();
	Button::LoadResources();

	// Create Navigation Buttons (Left side of screen) (can change positions/sizes as needed)
	btnAudioTab = new Button(-300.0f, 100.0f, 200.0f, 50.0f, "Audio");
	btnGraphicsTab = new Button(-300.0f, 30.0f, 200.0f, 50.0f, "Graphics");
	btnGameplayTab = new Button(-300.0f, -40.0f, 200.0f, 50.0f, "Gameplay");
	btnBack = new Button(-300.0f, -200.0f, 200.0f, 50.0f, "Back to Menu");

	// Create Example Content Buttons (Right side of screen) (can change positions/sizes as needed)
	btnToggleMute = new Button(100.0f, 100.0f, 250.0f, 50.0f, "Mute: OFF");
	btnToggleFullscreen = new Button(100.0f, 100.0f, 250.0f, 50.0f, "Fullscreen: OFF");
	btnToggleCameraShake = new Button(100.0f, 100.0f, 250.0f, 50.0f, "Camera Shake: OFF");
}

void SettingsScene::Init() {
	// audio and graphics settings are usually the most commonly accessed, so let's default to the audio tab when we open the settings menu
	currentTab = SETTINGS_TAB::AUDIO;

	// Reset camera to center just in case the last scene moved it
	AEGfxSetCamPosition(0.0f, 0.0f);
}

void SettingsScene::Update() {
	//f32 dt = (f32)AEFrameRateControllerGetFrameTime();

	// Handle Navigation Clicks
	if (btnAudioTab->IsClicked())    currentTab = SETTINGS_TAB::AUDIO;
	if (btnGraphicsTab->IsClicked()) currentTab = SETTINGS_TAB::GRAPHICS;
	if (btnGameplayTab->IsClicked()) currentTab = SETTINGS_TAB::GAMEPLAY;

	if (btnBack->IsClicked() || AEInputCheckTriggered(AEVK_ESCAPE)) {
		SceneManager::ChangeScene(new MainMenu());
		return;
	}

	// Handle Tab-Specific Logic
	switch (currentTab) {
	case SETTINGS_TAB::AUDIO:
		// TODO: Link this to the actual AudioManager volume
		if (btnToggleMute->IsClicked()) {
			// Example toggle mute logic (replace with actual AudioManager calls)
			// bool isMuted = audio->ToggleMute();
			// btnToggleMute->SetLabel(isMuted ? "Mute: ON" : "Mute: OFF");
			audio->SetMute();
		}

		btnToggleMute->SetLabel(audio->GetMuteStatus() ? "Mute: ON" : "Mute: OFF");

		break;

	case SETTINGS_TAB::GRAPHICS:
		// TODO: Link to window resizing logic
		if (btnToggleFullscreen->IsClicked()) {
			// Toggle fullscreen logic here
			bool isFullScreen = AESysIsFullScreen();
			AESysSetFullScreen(!isFullScreen);
		}

		btnToggleFullscreen->SetLabel(AESysIsFullScreen() ? "Fullscreen: ON" : "Fullscreen: OFF");

		break;

	case SETTINGS_TAB::GAMEPLAY:
		// TODO: Add keybinding buttons or difficulty toggles here
		// Ain't no way I doing keybindings yet LMAO
		if (btnToggleCameraShake->IsClicked()) {
			SettingsManager::GetInstance()->SetCameraShake();
		}

		btnToggleCameraShake->SetLabel(SettingsManager::GetInstance()->GetCameraShake() ? "Camera Shake: ON" : "Camera Shake: OFF");

		break;
	}
}

void SettingsScene::Draw() {
	// Draw a nice dark background
	AEGfxSetBackgroundColor(0.1f, 0.1f, 0.15f);

	// AEGfxSetRenderMode(AE_GFX_RM_COLOR); // Uncomment if your buttons need it!

	// Draw Navigation Buttons
	btnAudioTab->Draw();
	btnGraphicsTab->Draw();
	btnGameplayTab->Draw();
	btnBack->Draw();

	// 3. Draw Tab-Specific Content & Titles
	switch (currentTab) {
	case SETTINGS_TAB::AUDIO:
		AEGfxPrint(Button::fontId, "Audio Settings", 0.0f, 0.6f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		btnToggleMute->Draw();
		// TODO: Draw volume sliders here
		break;

	case SETTINGS_TAB::GRAPHICS:
		AEGfxPrint(Button::fontId, "Graphics Settings", 0.0f, 0.6f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		btnToggleFullscreen->Draw();
		// TODO: Draw resolution dropdowns here
		break;

	case SETTINGS_TAB::GAMEPLAY:
		AEGfxPrint(Button::fontId, "Gameplay Settings", 0.0f, 0.6f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		btnToggleCameraShake->Draw();
		// TODO: Draw keybind configs here
		break;
	}
}

void SettingsScene::Free() {
	// Clean up any dynamic lists if needed (not needed in this example since we don't have any, but good practice to include for future expansion)
}

void SettingsScene::Unload() {
	// Delete all buttons
	delete btnAudioTab;
	delete btnGraphicsTab;
	delete btnGameplayTab;
	delete btnBack;

	delete btnToggleMute;
	delete btnToggleFullscreen;
	delete btnToggleCameraShake;
}