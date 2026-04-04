/*!
@file       Splashscreen.h
@author     Bay Yeok Hao (yeokhao.bay)
@brief      Splashscreen fade-in before Main Menu
*//*_______________________________________________________________________*/

#pragma once
#include "BaseScene.h"

class Splashscreen : public BaseScene {
public:
    /*!
    @brief    Loads the splash texture and prepares any required assets.
    @param    void
    @return   void
    *//*______________________________________________________________*/
    void Load() override;

    /*!
    @brief    Initializes splashscreen state (timers, transforms).
    @param    void
    @return   void
    *//*______________________________________________________________*/
    void Init() override;

    /*!
    @brief    Updates splashscreen fade progress and transitions to the next scene when complete.
    @param    void
    @return   void
    *//*______________________________________________________________*/
    void Update() override;

    /*!
    @brief    Renders the splash texture with the current fade/transform.
    @param    void
    @return   void
    *//*______________________________________________________________*/
    void Draw() override;

    /*!
    @brief    Frees dynamically allocated resources used by the splashscreen.
    @param    void
    @return   void
    *//*______________________________________________________________*/
    void Free() override;

    /*!
    @brief    Unloads textures and other graphics resources from memory.
    @param    void
    @return   void
    *//*______________________________________________________________*/
    void Unload() override;

private:
    AEGfxTexture* splashTex{};
    Transform2D splashTfm;
    float splashCurr = 0.f;
    float splashDuration = 3.f;
};