#include "Button.h"
#include <iostream>
#include "MeshManager.h"

AEGfxTexture* Button::sprite = nullptr;
AEGfxVertexList* Button::pMesh = nullptr;
s8 Button::fontId = -1;;


Button::Button(float x, float y, float width, float height, const char* text)
    : x(x), y(y), w(width), h(height), label(text)
{
    if (!sprite || !pMesh) {
        LoadResources();
    }
}

void Button::Draw()
{
    //Draw the Button Background Image
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetTransparency(1.0f);

    // Set the button texture (safety check)
    if (sprite == nullptr) {
        AEGfxSetRenderMode(AE_GFX_RM_COLOR);
        AEGfxSetColorToMultiply(1.0f, 0.0f, 0.0f, 1.0f);
    }
    else {
        AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
        AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f); // Reset to White
        AEGfxTextureSet(sprite, 0, 0);
    }

    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetTransparency(1.0f);

    // Create Transform Matrix
    AEMtx33 scale, trans, final;
    AEMtx33Scale(&scale, w, h);
    AEMtx33Trans(&trans, x, y);
    AEMtx33Concat(&final, &trans, &scale);

    AEGfxSetTransform(final.m);

    if (pMesh) {
        AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
    }

    if (fontId != -1) {
        //Get Window Dimensions
        float winW = 800.0f; // Half of 1600
        float winH = 450.0f; // Half of 900

        //Convert Button World Position
        float textX = x / winW;
        float textY = y / winH;

        //Center the Text later pls
        float offsetX = -0.01f * strlen(label);
        float offsetY = -0.015f;

        AEGfxPrint(fontId, label, textX + offsetX, textY + offsetY, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    }
}

bool Button::IsHovered()
{
    //Get Mouse Position
    POINT mousePos;
    GetCursorPos(&mousePos);
    ScreenToClient(AESysGetWindowHandle(), &mousePos);

	//Window Dimensions
    float halfWidth = 800.0f;
    float halfHeight = 450.0f;

    //Convert to World Coordinates
    // World X = Pixel X - 800
    // World Y = 450 - Pixel Y
    float worldMouseX = mousePos.x - halfWidth;
    float worldMouseY = halfHeight - mousePos.y;

	// Debugging leave this here for now
    // char debug[100];
    // sprintf_s(debug, "Mouse: %f, %f | Button: %f, %f\n", worldMouseX, worldMouseY, x, y);
    // OutputDebugStringA(debug);

    // Check Collision
    return (worldMouseX > x - w / 2 && worldMouseX < x + w / 2 &&
        worldMouseY > y - h / 2 && worldMouseY < y + h / 2);
}


bool Button::IsClicked()
{
    //checks for left click
    return IsHovered() && AEInputCheckTriggered(AEVK_LBUTTON);
}


void Button::LoadResources() {
    // Load Texture
    if (!sprite) {
        sprite = AEGfxTextureLoad("Assets/Button/Default/TransParent center/panel-transparent-center-023.png");

		//for debugging
        if (!sprite) std::cout << "CRITICAL ERROR: Texture Not Found! Check path." << std::endl;
    }

    if (fontId == -1) {
        fontId = AEGfxCreateFont("Assets/Fonts/ttf/monogram.ttf", 25);

		//for debugging
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

    pMesh = nullptr;
}


void Button::SetPosition(float newX, float newY) {
    x = newX;
    y = newY;
}