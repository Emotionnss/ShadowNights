#pragma once
#include "AEEngine.h"

/*!
@file       Button.h
@author     Lim Chee Kang (l.cheekang)
@brief      This file contains definitions of functions and variables
             used for the Button class, which is a UI element used across
			 multiple scenes in the game, such as the main menu and pause menu.
*//*_______________________________________________________________________*/

class Button {
public:
    Button(float x, float y, float w, float h, const char* text);

    void Draw();        
    bool IsHovered();   
    bool IsClicked();   
    void SetPosition(float x, float y);
    void SetLabel(const char* text);

    static void LoadResources();   
    static void FreeResources();  
    static s8 fontId;

private:
    float x, y, w, h;       
    char label[64];

    static AEGfxTexture* sprite;
    static AEGfxVertexList* pMesh;
};