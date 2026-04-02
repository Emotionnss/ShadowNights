/*!
@file       Splashscreen.h
@author     Bay Yeok Hao (yeokhao.bay)
@brief      Splashscreen fade-in before Main Menu
*//*_______________________________________________________________________*/

#pragma once
#include "BaseScene.h"

class Splashscreen : public BaseScene {
public:
    void Load() override;
    void Init() override;
    void Update() override;
    void Draw() override;
    void Free() override;
    void Unload() override;

private:
    AEGfxTexture* splashTex{};
    Transform2D splashTfm;
    float splashCurr = 0.f;
    float splashDuration = 3.f;
};