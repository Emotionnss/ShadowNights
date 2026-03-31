#include "Button.h"
#include <iostream>
#include <cstdio>
#include "MeshManager.h"

/*!
@file       Button.cpp
@author     Lim Chee Kang (l.cheekang)
@brief      This file contains definitions of functions and variables
             used for the Button class, which is a UI element used for
             clickable buttons in the game, and contains functions to draw
             the button, check if it's hovered or clicked, and manage its
			 resources.
*//*_______________________________________________________________________*/

AEGfxTexture* Button::sprite = nullptr;
AEGfxVertexList* Button::pMesh = nullptr;
s8 Button::fontId = -1;

Button::Button(float x, float y, float width, float height, const char* text)
    : x(x), y(y), w(width), h(height)
{
    SetLabel(text);

    if (!sprite || !pMesh || fontId == -1) {
        LoadResources();
    }
}

void Button::SetLabel(const char* text) {
    if (text) {
        snprintf(label, sizeof(label), "%s", text);
    }
}

void Button::Draw() const
{
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetTransparency(1.0f);

    if (sprite == nullptr) {
        AEGfxSetRenderMode(AE_GFX_RM_COLOR);
        AEGfxSetColorToMultiply(1.0f, 0.0f, 0.0f, 1.0f);
    }
    else {
        AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
        AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
        AEGfxTextureSet(sprite, 0, 0);
    }

    AEMtx33 scale, trans, final;
    AEMtx33Scale(&scale, w, h);
    AEMtx33Trans(&trans, x, y);
    AEMtx33Concat(&final, &trans, &scale);
    AEGfxSetTransform(final.m);

    if (pMesh) {
        AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
    }

    AEMtx33 identity;
    AEMtx33Identity(&identity);
    AEGfxSetTransform(identity.m);

    if (fontId != -1) {
        float camX, camY;
        AEGfxGetCamPosition(&camX, &camY);

        float relX = x - camX;
        float relY = y - camY;

        float textX = relX / 800.0f; 
        float textY = relY / 450.0f; 

        float textW, textH;
        AEGfxGetPrintSize(fontId, label, 1.0f, &textW, &textH);

        textX -= (textW / 2.0f);
        textY -= (textH / 2.0f);

        AEGfxPrint(fontId, label, textX, textY, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    }
}

bool Button::IsHovered() const 
{
	HWND hwnd = AESysGetWindowHandle(); // Get the window handle from the engine
	POINT mousePos; // Structure to hold mouse position
	GetCursorPos(&mousePos); // Get mouse position in screen coordinates
	ScreenToClient(hwnd, &mousePos); // Convert to client area coordinates

	// Get current window size to convert mouse position to game world coordinates
	RECT rect; // Structure to hold window dimensions
    GetClientRect(hwnd, &rect);
	float currentWinWidth = static_cast<float>(rect.right - rect.left); // Calculate current window width
	float currentWinHeight = static_cast<float>(rect.bottom - rect.top); // Calculate current window height

    // Safety check to prevent dividing by zero when minimizing
    if (currentWinWidth <= 0.0f || currentWinHeight <= 0.0f) return false;

    float camX, camY;
    AEGfxGetCamPosition(&camX, &camY);

	// convert mouse position to game world coordinates:
    float percentX = mousePos.x / currentWinWidth;
    float percentY = mousePos.y / currentWinHeight;

	// map to 1600x900 virtual resolution
    float scaledMouseX = percentX * 1600.0f;
    float scaledMouseY = percentY * 900.0f;

    // shift origin to center and adjust for camera position
    float worldMouseX = (scaledMouseX - 800.0f) + camX;
    float worldMouseY = (450.0f - scaledMouseY) + camY;

    return (worldMouseX > x - w / 2.0f && worldMouseX < x + w / 2.0f &&
        worldMouseY > y - h / 2.0f && worldMouseY < y + h / 2.0f);
}

bool Button::IsClicked() const 
{
    return IsHovered() && AEInputCheckTriggered(AEVK_LBUTTON);
}

void Button::LoadResources() {
    if (sprite == nullptr) {
        sprite = AEGfxTextureLoad("Assets/Button/Default/Panel/panel-011.png");
        if (!sprite) std::cout << "CRITICAL ERROR: Texture Not Found! Check path." << std::endl;
    }

    if (fontId == -1) {
        fontId = AEGfxCreateFont("Assets/Fonts/ttf/monogram.ttf", 25);
        if (fontId == -1) std::cout << "CRITICAL ERROR: Font Not Found! Check path." << std::endl;
    }

    if (!pMesh) {
        pMesh = MeshManager::GetInstance()->GetMesh(TEXTURE_SQUARE);
    }
}

void Button::FreeResources() {
    if (sprite) {
        AEGfxTextureUnload(sprite);
        sprite = nullptr;
    }

    if (fontId != -1) {
        AEGfxDestroyFont(fontId);
        fontId = -1;
    }

    pMesh = nullptr;
}

void Button::SetPosition(float newX, float newY) {
    x = newX;
    y = newY;
}