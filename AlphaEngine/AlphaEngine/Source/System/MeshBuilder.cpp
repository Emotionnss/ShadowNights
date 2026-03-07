/*!
@file       MeshBuilder.cpp
@author     Yap Yu Xuan Daevon (yuxuandaevon.yap)
@brief      This file contains definitions of static functions
            used for the MeshBuilder class, which are used to create
            meshes of different types
*//*_______________________________________________________________________*/

#include "MeshBuilder.h"
#include <iostream>
#include <array>

AEGfxVertexList* MeshBuilder::CreateSquareMesh(bool texture)
{
    AEGfxVertexList* mesh;

    std::array<f32, 2> uvCoords{};

    if (texture)
    {
        //This is square with UV coords
        uvCoords[1] = { 1.0f };
    }

    AEGfxMeshStart();

    // Now it's a square lol
    AEGfxTriAdd(
        -0.5f, -0.5f, 0xFFFFFFFF, uvCoords[0], uvCoords[1],
        0.5f, -0.5f, 0xFFFFFFFF, uvCoords[1], uvCoords[1],
        -0.5f, 0.5f, 0xFFFFFFFF, uvCoords[0], uvCoords[0]);

    AEGfxTriAdd(
        0.5f, -0.5f, 0xFFFFFFFF, uvCoords[1], uvCoords[1],
        0.5f, 0.5f, 0xFFFFFFFF, uvCoords[1], uvCoords[0],
        -0.5f, 0.5f, 0xFFFFFFFF, uvCoords[0], uvCoords[0]);

    mesh = AEGfxMeshEnd();

    if (mesh == nullptr) {
        std::cout << "ERROR: Square Mesh Creation Failed!" << std::endl;
    }
    else {
        std::cout << "SUCCESS: Square Mesh Created." << std::endl;
    }

    return mesh;
}

AEGfxVertexList* MeshBuilder::CreateCornerSquareMesh(bool texture)
{
    AEGfxVertexList* mesh;

    std::array<f32, 2> uvCoords{};

    if (texture)
    {
        //This is square with UV coords
        uvCoords[1] = { 1.0f };
    }

    AEGfxMeshStart();

    // Now it's a square lol
    AEGfxTriAdd(
        0.0, 1.0f, 0xFFFFFFFF, uvCoords[0], uvCoords[1],
        1.0f, 1.0f, 0xFFFFFFFF, uvCoords[1], uvCoords[1],
        0.0f, 0.0f, 0xFFFFFFFF, uvCoords[0], uvCoords[0]);

    AEGfxTriAdd(
        0.0f, 0.0f, 0xFFFFFFFF, uvCoords[0], uvCoords[0],
        1.0f, 0.0f, 0xFFFFFFFF, uvCoords[1], uvCoords[0],
        1.0f, 1.0f, 0xFFFFFFFF, uvCoords[1], uvCoords[1]);

    mesh = AEGfxMeshEnd();

    if (mesh == nullptr) {
        std::cout << "ERROR: Square Mesh Creation Failed!" << std::endl;
    }
    else {
        std::cout << "SUCCESS: Square Mesh Created." << std::endl;
    }

    return mesh;
}

AEGfxVertexList* MeshBuilder::CreateTriangle()
{
    AEGfxVertexList* mesh;

    AEGfxMeshStart();

    AEGfxTriAdd(
        -0.5f, -0.5f, 0xFFFFFF00, 0.0f, 0.0f,
        0.5f, -0.5f, 0xFFFFFF00, 0.0f, 0.0f,
        0.0f, 0.5f, 0xFFFFFF00, 0.0f, 0.0f
    );

    mesh = AEGfxMeshEnd();

    if (mesh == nullptr) {
        std::cout << "ERROR: Triangle Mesh Creation Failed!" << std::endl;
    }
    else {
        std::cout << "SUCCESS: Triangle Mesh Created." << std::endl;
    }

    return mesh;
}

AEGfxVertexList* MeshBuilder::CreateCircle()
{
    AEGfxVertexList* mesh;

    AEGfxMeshStart();

    for (int j = 0; j < 36; j++)
    {
        AEGfxTriAdd(
            AECos((PI / 180) * j * 10.f), AESin((PI / 180) * j * 10.f), 0xFFFFFFFF, 0.0f, 0.0f,
            AECos((PI / 180) * (j + 1) * 10.f), AESin((PI / 180) * (j + 1) * 10.f), 0xFFFFFFFF, 0.0f, 0.0f,
            0.f, 0.f, 0xFFFFFFFF, 0.0f, 0.0f);
    }

    mesh = AEGfxMeshEnd();

    if (mesh == nullptr) {
        std::cout << "ERROR: Circle Mesh Creation Failed!" << std::endl;
    }
    else {
        std::cout << "SUCCESS: Circle Mesh Created." << std::endl;
    }

    return mesh;
}

AEGfxVertexList* MeshBuilder::CreateLine()
{
    AEGfxVertexList* mesh;

    AEGfxMeshStart();

    AEGfxVertexAdd(
        -0.5f, 0.0f, 0xFFFF0000, 0.0f, 0.0f
    );

    AEGfxVertexAdd(
        0.5f, 0.0f, 0xFFFF0000, 0.0f, 0.0f
    );

    mesh = AEGfxMeshEnd();

    if (mesh == nullptr) {
        std::cout << "ERROR: Line Mesh Creation Failed!" << std::endl;
    }
    else {
        std::cout << "SUCCESS: Line Mesh Created." << std::endl;
    }

    return mesh;
}

AEGfxVertexList* MeshBuilder::CreateSpriteMesh(f32 spriteUVWidth, f32 spriteUVHeight)
{
    AEGfxVertexList* mesh;

    AEGfxMeshStart();

    // Now it's a square lol
    AEGfxTriAdd(
        -0.5f, -0.5f, 0xFFFFFFFF, 0.0f, spriteUVHeight,
        0.5f, -0.5f, 0xFFFFFFFF, spriteUVWidth, spriteUVHeight,
        -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

    AEGfxTriAdd(
        0.5f, -0.5f, 0xFFFFFFFF, spriteUVWidth, spriteUVHeight,
        0.5f, 0.5f, 0xFFFFFFFF, spriteUVWidth, 0.0f,
        -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

    mesh = AEGfxMeshEnd();

    if (mesh == nullptr) {
        std::cout << "ERROR: Sprite Mesh Creation Failed!" << std::endl;
    }
    else {
        std::cout << "SUCCESS: Sprite Mesh Created." << std::endl;
    }

    return mesh;
}
