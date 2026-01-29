#include "MeshManager.h"
#include <iostream>
#include <array>

MeshManager::MeshManager()
{
    CreateMeshes();
}

MeshManager::~MeshManager()
{
    for (std::pair<MESH_TYPE, AEGfxVertexList*> mesh : meshList)
    {
        //Free meshes
        AEGfxMeshFree(mesh.second);
        mesh.second = nullptr;
    }

    //for (std::map<MESH_TYPE, AEGfxVertexList*>::iterator it = meshList.begin(); it != meshList.end(); it++)
    //{
    //    //Free meshes
    //    AEGfxMeshFree(it->second);
    //    it->second = nullptr;
    //}
}

void MeshManager::CreateMeshes()
{
    CreateSquareMesh();
    CreateSquareMesh(true);
    CreateTriangle();
    CreateCircle();
}

void MeshManager::CreateSquareMesh(bool texture)
{
    AEGfxVertexList* mesh;

    std::array<f32, 2> uvCoords{};

    MESH_TYPE type = SQUARE;

    if (texture)
    {
        //This is square with UV coords
        uvCoords[1] = { 1.0f };
        type = TEXTURE_SQUARE;
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

        //Insert mesh into list if sucessful
        meshList.insert({ type, mesh });
    }
}

void MeshManager::CreateTriangle()
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

        //Insert mesh into list if sucessful
        meshList.insert({ TRIANGLE, mesh });
    }
}

void MeshManager::CreateCircle()
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

        //Insert mesh into list if sucessful
        meshList.insert({ CIRCLE, mesh });
    }
}

void MeshManager::CreateLine()
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

        //Insert mesh into list if sucessful
        meshList.insert({ LINE, mesh });
    }
}

AEGfxVertexList* MeshManager::GetMesh(MESH_TYPE type) const
{
	return meshList.at(type);
}
