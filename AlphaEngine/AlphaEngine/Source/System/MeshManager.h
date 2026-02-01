/*!
@file       MeshManager.h
@author     Yap Yu Xuan Daevon (yuxuandaevon.yap)
@brief      This file contains declarations of functions and variables
			used for the MeshManager class, which holds all primitive meshes
			that are reused many times across the whole project
*//*_______________________________________________________________________*/

#pragma once

#include "AEEngine.h"
#include "SingletonTemplate.h"
#include <unordered_map>

enum MESH_TYPE
{
	SQUARE = 0,
	TEXTURE_SQUARE,
	TRIANGLE,
	CIRCLE,
	LINE,
	TOTAL_MESH
};

class MeshManager : public SingletonTemplate<MeshManager>
{
	friend class SingletonTemplate<MeshManager>;

private:
	//Map of meshes
	std::unordered_map<MESH_TYPE, AEGfxVertexList*> meshList;

public:
	/*!
	@brief  Constructor which will create primitive meshes
	*/
	MeshManager();

	/*!
	@brief	Destructor which will deallocate all meshes in meshList
	*/
	~MeshManager();

	/*!
	@brief	Get mesh based on MESH_TYPE given
	@param	MESH_TYPE type - type of mesh to get
	@return AEGfxVertexList* - vertex list of the mesh that has been gotten
	*/
	AEGfxVertexList* GetMesh(MESH_TYPE type) const;
};
