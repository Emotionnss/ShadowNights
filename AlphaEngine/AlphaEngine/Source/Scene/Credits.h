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
    /*!
    @brief    Loads the Credits scene
    @param    void
    @return   void
    *//*______________________________________________________________*/
    void Load() override;
    /*!
    @brief    Initializes the Credits scene
    @param    void
    @return   void
    *//*______________________________________________________________*/
    void Init() override;
    /*!
    @brief    Updates the Credits scene
    @param    void
    @return   void
    *//*______________________________________________________________*/
    void Update() override;
    /*!
    @brief    Renders the Credits scene
    @param    void
    @return   void
    *//*______________________________________________________________*/
    void Draw() override;
    /*!
    @brief    Frees the variables of the Credits scene
    @param    void
    @return   void
    *//*______________________________________________________________*/
    void Free() override;

    /*!
    @brief    Unloads the Credits scene
    @param    void
    @return   void
    *//*______________________________________________________________*/
    void Unload() override;

private:
    Button* backBtn = nullptr;//back to main menu button
    float scrollSpeed = 100.f;//scroll speed
    AEGfxTexture* creditsImage;//credits image
    Transform2D creditsTfm;//transform for the image scroll

    //various sprites and their transforms used as background VFXs
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

    AEGfxTexture* logoTex{};
    Transform2D logoTfm{};

    AEGfxTexture* bgTex{};
    Transform2D bgTfm{};

    //alphas of the logo and background to allow for cool fade in effect hehehehehe
    f32 logoAlpha{0.1f};

    f32 bgAlpha{};
};

