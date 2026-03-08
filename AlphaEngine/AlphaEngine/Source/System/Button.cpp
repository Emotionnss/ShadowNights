#include "Button.h"
#include <iostream>
#include <cstdio>
#include "MeshManager.h"

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

void Button::Draw()
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

        AEGfxPrint(fontId, label, textX, textY, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    }
}

bool Button::IsHovered()
{
    POINT mousePos;
    GetCursorPos(&mousePos);
    ScreenToClient(AESysGetWindowHandle(), &mousePos);

    float camX, camY;
    AEGfxGetCamPosition(&camX, &camY);

    float worldMouseX = (mousePos.x - 800.0f) + camX;
    float worldMouseY = (450.0f - mousePos.y) + camY;

    return (worldMouseX > x - w / 2.0f && worldMouseX < x + w / 2.0f &&
        worldMouseY > y - h / 2.0f && worldMouseY < y + h / 2.0f);
}

bool Button::IsClicked()
{
    return IsHovered() && AEInputCheckTriggered(AEVK_LBUTTON);
}

void Button::LoadResources() {
    sprite = AEGfxTextureLoad("Assets/Button/Default/TransParent center/panel-transparent-center-023.png");
    if (!sprite) std::cout << "CRITICAL ERROR: Texture Not Found! Check path." << std::endl;

    fontId = AEGfxCreateFont("Assets/Fonts/ttf/monogram.ttf", 25);
    if (fontId == -1) std::cout << "CRITICAL ERROR: Font Not Found! Check path." << std::endl;

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
    fontId = -1; // Reset to prevent dangling pointers
}

void Button::SetPosition(float newX, float newY) {
    x = newX;
    y = newY;
}