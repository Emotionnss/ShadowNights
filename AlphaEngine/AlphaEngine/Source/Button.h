#pragma once
#include "AEEngine.h"

class Button {
public:
    Button(float x, float y, float w, float h, const char* text);

    void Draw();        // Draw the button
    bool IsHovered();   // Check if mouse is over the button
    bool IsClicked();   // Check if button is clicked
    void SetPosition(float x, float y);

    static void LoadResources();   // Load texture/mesh once
    static void FreeResources();   // Free texture/mesh once

private:
    float x, y, w, h;       // Position and size
    const char* label;      // Button label text

    // Static shared resources
    static AEGfxTexture* sprite;
    static AEGfxVertexList* pMesh; 
    static s8 fontId;
};