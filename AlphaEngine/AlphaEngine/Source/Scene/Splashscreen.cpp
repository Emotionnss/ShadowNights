/*!
@file       Splashscreen.cpp
@author     Bay Yeok Hao (yeokhao.bay)
@brief      Splashscreen fade-in before Main Menu
*//*_______________________________________________________________________*/

#include "Splashscreen.h"
#include "../System/SceneManager.h"
#include "./MainMenu.h"
#include "AEEngine.h"

// Load assets
void Splashscreen::Load() 
{
    AEGfxReset();
}

// Init textures and position/scale of texture
void Splashscreen::Init() 
{
    splashTex = AEGfxTextureLoad("Assets/Credits/DigiPen_Singapore_WEB_RED.png");

    splashTfm.SetPosition(0, 0);
    splashTfm.SetScale(1525, 445);
}

// Update, advance timer
void Splashscreen::Update() 
{
    float dt = (float)AEFrameRateControllerGetFrameTime();

    splashCurr += 1.f * dt;
    AEClamp(splashCurr, 0.f, splashDuration);

    // once duration is over, change to main menu scene
    if (splashCurr >= splashDuration)
    {
        SceneManager::ChangeScene(new MainMenu());
    }
}

// Render, opacity of splashscreen texture is tied to time
void Splashscreen::Draw() 
{
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
    AEGfxSetColorToAdd(0.f, 0.f, 0.f, 0.0f);

    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetTransparency(splashCurr/splashDuration);
    
    AEGfxTextureSet(splashTex, 0, 0);
    
    AEGfxSetTransform(splashTfm.GetMatrix().m);
    AEGfxMeshDraw(MeshManager::GetInstance()->GetMesh(TEXTURE_SQUARE), AE_GFX_MDM_TRIANGLES);

}

// Cleanup, unloads textures
void Splashscreen::Free() 
{
    if (splashTex)
        AEGfxTextureUnload(splashTex);
}

void Splashscreen::Unload() {}