/*!
@file       MeshManager.cpp
@author     Yap Yu Xuan Daevon (yuxuandaevon.yap)
@brief      This file contains definitions of functions and variables
            used for the MeshManager class, which holds all primitive meshes
            that are reused many times across the whole project
*//*_______________________________________________________________________*/

#include "MeshManager.h"
#include "MeshBuilder.h"

MeshManager::MeshManager()
{
    meshList.insert({ SQUARE, MeshBuilder::CreateSquareMesh() });
    meshList.insert({ TEXTURE_SQUARE, MeshBuilder::CreateSquareMesh(true) });
    meshList.insert({ TRIANGLE, MeshBuilder::CreateTriangle() });
    meshList.insert({ CIRCLE, MeshBuilder::CreateCircle() });
    meshList.insert({ LINE, MeshBuilder::CreateLine() });
}

MeshManager::~MeshManager()
{
    for (std::pair<MESH_TYPE, AEGfxVertexList*> mesh : meshList)
    {
        //Free meshes
        AEGfxMeshFree(mesh.second);
        mesh.second = nullptr;
    }
}

AEGfxVertexList* MeshManager::GetMesh(MESH_TYPE type) const
{
	return meshList.at(type);
}
