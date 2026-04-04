/*!
@file       SettingsScene.h
@author     Yap Yu Xuan, Daevon (yuxuandaevon.yap), Chng Wei Sheng Bennett (chng.w)
@brief      This file contains the declaration of the SettingsScene class, which
            provides UI for audio, graphics, and gameplay settings and allows
            the player to adjust options and return to previous menus.
*//*_______________________________________________________________________*/

#pragma once
#include "BaseScene.h"
#include "../System/Button.h"
#include "../Utils/Slider.h"

// Enum to track which settings page the player is currently viewing
enum class SETTINGS_TAB {
    NONE,
    AUDIO,
    GRAPHICS,
    GAMEPLAY
};

class SettingsScene : public BaseScene {
public:
    /*!
    @brief    Constructs a SettingsScene for the given save slot.
    @param    slot The save slot to load from (defaults to 0).
    *//*______________________________________________________________*/
    SettingsScene(int slot = 0);

    /*!
    @brief    Destructor; frees scene-specific resources.
    @param    void
    *//*______________________________________________________________*/
    ~SettingsScene() override;

    /*!
    @brief    Loads assets, UI elements, and initializes the background for the settings scene.
    @param    void
    @return   void
    *//*______________________________________________________________*/
    void Load() override;

    /*!
    @brief    Initializes runtime state for the settings scene (positions, slider values, etc.).
    @param    void
    @return   void
    *//*______________________________________________________________*/
    void Init() override;

    /*!
    @brief    Handles user input, tab switching, and interactions with buttons and sliders.
    @param    void
    @return   void
    *//*______________________________________________________________*/
    void Update() override;

    /*!
    @brief    Renders the settings UI, currently active tab contents, and any helper text.
    @param    void
    @return   void
    *//*______________________________________________________________*/
    void Draw() override;

    /*!
    @brief    Frees dynamically allocated UI elements and scene-specific resources.
    @param    void
    @return   void
    *//*______________________________________________________________*/
    void Free() override;

    /*!
    @brief    Unloads textures and other graphics resources used by the settings scene.
    @param    void
    @return   void
    *//*______________________________________________________________*/
    void Unload() override;

private:
    SETTINGS_TAB currentTab = SETTINGS_TAB::NONE;

	// Navigation Buttons (can change positions/sizes as needed)
    Button* btnAudioTab = nullptr;
    Button* btnGraphicsTab = nullptr;
    Button* btnGameplayTab = nullptr;
    Button* btnBack = nullptr;

    // Example Content Buttons
    Button* btnToggleMute = nullptr;
    Button* btnToggleFullscreen = nullptr;
    Button* btnToggleCameraShake = nullptr;

    // Sliders
    Slider* sliderBGM = nullptr;
    Slider* sliderSFX = nullptr;
    int activeSlider = 0;

    // Scene Specific Parameterss
};

