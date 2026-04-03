#pragma once
#include "BaseScene.h"
#include "Level1Scene.h"

/*!
@file       TutorialScene.h
@author     Lim Chee Kang (l.cheekang), Yap Yu Xuan Daevon (yuxuandaevon.yap)
@brief      This file contains definitions of functions and variables
             used for the TutorialScene class, which is the first scene of the game,
             and contains the specific implementation of the scene, such as the
            tutorial text that is shown and the map used.
*//*_______________________________________________________________________*/

struct TutorialText
{
    AEGfxTexture* tex;
    Transform2D transform;

    /*!
    @brief    Constructor to initialize tutorial text image properties.
    @param    pos The world position of the text.
    @param    scale The scale of the text image.
    @param    name The filename of the tutorial image.
    *//*______________________________________________________________*/
    TutorialText(AEVec2 pos, AEVec2 scale, std::string name)
    {
        transform.SetPosition(pos);
        transform.SetScale(scale.x, scale.y);
        std::string filePath = "Assets/Tutorial/" + name + ".png";
        tex = AEGfxTextureLoad(filePath.c_str());
    }

    /*!
    @brief    Destructor that frees the loaded tutorial texture.
    *//*______________________________________________________________*/
    ~TutorialText()
    {
        AEGfxTextureUnload(tex);
    }
};

class TutorialScene : public BaseScene {
private:
    std::vector<TutorialText*> tutTexts;
    s8 fontId;

public:
    /*!
    @brief    Constructor for TutorialScene, sets up map name and parsed tutorial text.
    @param    slot The save slot to load from, defaults to 0.
    *//*______________________________________________________________*/
    TutorialScene(int slot = 0) : BaseScene(slot) {
        levelMapName = "TutorialLevel";
        levelBGM = "bgm_level1";
        nextScene = new Level1Scene();

        levelID = 0;

        //Doing this just so i can set scale
        fontId = AEGfxCreateFont("Assets/Fonts/ttf/monogram.ttf", 36);

        std::vector<std::vector<std::string>> tutorialData = CSVManager::ReadTutorialInfo();

        f32 tileSize = SettingsManager::GetInstance()->TILE_HEIGHT;

        for (std::vector<std::string> data : tutorialData)
        {
            AEVec2 scale{};

            s32 count{ max(1, static_cast<int>(data.size()) - 3) };

            //Determine scale
            for (std::vector<std::string>::iterator it = data.begin() + 3; it != data.end(); ++it)
            {
                AEVec2 tmpScale{};
                AEGfxGetPrintSize(fontId, it->c_str(), 36, &tmpScale.x, &tmpScale.y);

                scale.x = tmpScale.x > scale.x ? tmpScale.x : scale.x;
                if (data.size() <= 5)
                {
                    scale.y += tmpScale.y / 0.95f;
                }
                else
                {
                    scale.y += tmpScale.y * (1.0f + count++ * 0.085f); 
                }
            }

            //AEVec2Scale(&scale, &scale, tileSize / 4.f);
            scale.x *= tileSize / 3.f;
            scale.y *= tileSize / 4.f;

            TutorialText* text = new TutorialText(
                AEVec2{ std::stof(data[1]) * tileSize, std::stof(data[2]) * tileSize }, scale, data[0]);
            tutTexts.push_back(text);
        }

        AEGfxDestroyFont(fontId);
    }

    /*!
    @brief    Loads the tutorial assets and restores the player's health.
    *//*______________________________________________________________*/
    void Load()
    {
        BaseScene::Load();

        if (player != nullptr)
        {
            player->SetHealth(100);
        }
    }

    /*!
    @brief    Calls the base draw function to render the tutorial map and entities.
    *//*______________________________________________________________*/
    void Draw()
    {
        BaseScene::Draw();
    }

    /*!
    @brief    Cleans up tutorial specific text structures and unloads the scene.
    *//*______________________________________________________________*/
    void Unload()
    {
        BaseScene::Unload();

        for (TutorialText* tut : tutTexts)
        {
            delete tut;
        }

        tutTexts.clear();
    }

    /*!
    @brief    Iterates through and renders the loaded tutorial text textures onto the map.
    *//*______________________________________________________________*/
    void RenderTutorialText()
    {
        AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
        AEGfxSetBlendMode(AE_GFX_BM_BLEND);
        AEGfxSetTransparency(1.0f);

        for (TutorialText* tut : tutTexts)
        {
            AEGfxTextureSet(tut->tex, 0, 0);
            AEGfxSetTransform(tut->transform.GetMatrix().m);
            AEGfxMeshDraw(meshManager->GetMesh(TEXTURE_SQUARE), AE_GFX_MDM_TRIANGLES);
        }
    }
};