#pragma once
#include "../GameObject/GameObject.h"
#include "../System/MeshManager.h"

class Background : public GameObject
{
public:
	Background();
	~Background();

	Transform2D transform;
	AEGfxTexture* projTex;

	void Draw(AEGfxVertexList* mesh);
};