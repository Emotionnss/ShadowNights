/*!
@file       Credits.h
@author     Chew Zavier (c.zavier)
@brief      This file contains declarations of functions used by the Credits scene
*//*_______________________________________________________________________*/
#pragma once
#include "BaseScene.h"
#include "../System/Button.h"

class CreditsScene : public BaseScene {
public:
    void Load() override;
    void Init() override;
    void Update() override;
    void Draw() override;
    void Free() override;
    void Unload() override;

private:
    Button* backBtn = nullptr;//back to main menu button
    float scrollSpeed = 100.f;//scroll speed
    AEGfxTexture* creditsImage;//credits image
    Transform2D creditsTfm;//transform for the image scroll

    //various sprites used as background VFXs
    Sprite* playerSprite{};
    Transform2D pSTfm{};

    Sprite* archerSprite{};
    Transform2D archerTfm{};

    Sprite* batSprite{};
    Transform2D batTfm{};

    Sprite* shadowSprite{};
    Transform2D shadowTfm{};

    Sprite* bossSprite{};
    Transform2D bossTfm{};
};

