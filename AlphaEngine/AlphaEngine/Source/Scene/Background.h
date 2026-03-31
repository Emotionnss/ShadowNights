#pragma once
#include "../GameObject/GameObject.h"
#include "../System/MeshManager.h"

class Background : public GameObject
{
public:
	Background(const char* texturePath = "Assets/Sprites/Background/orig_big.png");
	~Background();

	Transform2D transform;
	AEGfxTexture* projTex;

	void Draw(AEGfxVertexList* mesh);
};