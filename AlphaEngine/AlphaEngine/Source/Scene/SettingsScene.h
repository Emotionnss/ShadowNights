#pragma once
#include "BaseScene.h"
#include "../System/Button.h"

// Enum to track which settings page the player is currently viewing
enum class SETTINGS_TAB {
    AUDIO,
    GRAPHICS,
    GAMEPLAY
};

class SettingsScene : public BaseScene {
public:
    SettingsScene(int slot = 0);
    ~SettingsScene() override;

    void Load() override;
    void Init() override;
    void Update() override;
    void Draw() override;
    void Free() override;
    void Unload() override;

private:
    SETTINGS_TAB currentTab;

	// Navigation Buttons (can change positions/sizes as needed)
    Button* btnAudioTab = nullptr;
    Button* btnGraphicsTab = nullptr;
    Button* btnGameplayTab = nullptr;
    Button* btnBack = nullptr;

    // Example Content Buttons
    Button* btnToggleMute = nullptr;
    Button* btnToggleFullscreen = nullptr;

    // Scene Specific Parameters
};

