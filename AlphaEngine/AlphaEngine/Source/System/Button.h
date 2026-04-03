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
    /*!
    @brief    Constructor to initialize a button's dimensions and text.
    @param    x Center x-coordinate.
    @param    y Center y-coordinate.
    @param    w Total width of the button.
    @param    h Total height of the button.
    @param    text The text to display inside the button.
    *//*______________________________________________________________*/
    Button(float x, float y, float w, float h, const char* text);

    /*!
    @brief    Renders the button graphics and centered text.
    *//*______________________________________________________________*/
    void Draw() const;

    /*!
    @brief    Checks if the user's mouse cursor is currently over the button.
    @return   True if hovered, false otherwise.
    *//*______________________________________________________________*/
    bool IsHovered() const;

    /*!
    @brief    Checks if the user left-clicked on the button this frame.
    @return   True if clicked, false otherwise.
    *//*______________________________________________________________*/
    bool IsClicked() const;

    /*!
    @brief    Updates the physical position of the button on the screen.
    @param    x New center x-coordinate.
    @param    y New center y-coordinate.
    *//*______________________________________________________________*/
    void SetPosition(float x, float y);

    /*!
    @brief    Updates the text label displayed inside the button.
    @param    text The new text string.
    *//*______________________________________________________________*/
    void SetLabel(const char* text);

    /*!
    @brief    Loads standard button textures and meshes into graphics memory.
    *//*______________________________________________________________*/
    static void LoadResources();

    /*!
    @brief    Frees standard button textures and meshes from graphics memory.
    *//*______________________________________________________________*/
    static void FreeResources();

    static s8 fontId;

private:
    float x, y, w, h;
    char label[64];

    static AEGfxTexture* sprite;
    static AEGfxVertexList* pMesh;
};