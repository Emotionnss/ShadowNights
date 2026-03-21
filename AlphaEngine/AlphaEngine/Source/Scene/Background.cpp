#include "Background.h"
#include <iostream>

Background::Background()
	: projTex{ }
{
	transform.SetScale(1600, 900);
	transform.SetPosition(0, 0);

	projTex = AEGfxTextureLoad("Assets/Sprites/Background/orig_big.png");

	if (projTex)
	{
		std::cout << "projTex" << std::endl;
	}
}

Background::~Background()
{
	if (projTex)
	{
		AEGfxTextureUnload(projTex);
	}
}

void Background::Draw(AEGfxVertexList* mesh)
{
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);

	AEGfxTextureSet(projTex, 0, 0);

	AEVec2 pos = transform.GetPosition();
	AEGfxGetCamPosition(&pos.x, &pos.y);
	transform.SetPosition(pos);
	AEGfxSetTransform(transform.GetMatrix().m);
	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxTextureSet(nullptr, 0, 0);
}