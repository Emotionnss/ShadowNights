#pragma once

#include "AEEngine.h"
#include "SingletonTemplate.h"
#include <map>

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
	std::map<MESH_TYPE, AEGfxVertexList*> meshList;

public:
	MeshManager();
	~MeshManager();

	void CreateMeshes();

	void CreateSquareMesh(bool texture = false);
	void CreateTriangle();
	void CreateLine();

	void CreateCircle();
	//Get Meshes
	AEGfxVertexList* GetMesh(MESH_TYPE type) const;
};
