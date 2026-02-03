#include "BaseProjectile.h"
#include <iostream>

BaseProjectile::BaseProjectile()
{
	//speed = 500.f;
	//size = 25.f;
}

BaseProjectile::BaseProjectile(AEVec2 pos, AEVec2 dir)
{
	direction = dir;

	speed = 500.f;
	size = 25.f;

	transform.SetScale(size, size);
	transform.SetPosition(pos.x, pos.y);

	projTex = AEGfxTextureLoad("Assets/Weapons/knife.png");
	if (projTex)
	{
		std::cout << "loaded successfully" << std::endl;
	}

	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh
	Texmesh = AEGfxMeshEnd();
}

BaseProjectile::~BaseProjectile()
{
	Exit();
	if (Texmesh)
	{
		AEGfxMeshFree(Texmesh);
		Texmesh = nullptr;
	}
}

void BaseProjectile::Init()
{

}
void BaseProjectile::Update(float deltaTime)
{
	if (currLifespan > 0.f && !stopped)
	{
		currLifespan -= 1.f * deltaTime;
	}

	if (!stopped)
	{
		AEVec2 position = transform.GetPosition();
		position.x += speed * direction.x * deltaTime;
		position.y += speed * direction.y * deltaTime;

		float angle = transform.GetRotation();
		transform.SetRotation(atan2f(direction.y, direction.x) + (PI * 1.25f));
		transform.SetPosition(position.x, position.y);
	}
}

void BaseProjectile::Draw(AEGfxVertexList* mesh)
{
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	
	AEGfxTextureSet(projTex, 0, 0);
	
	AEGfxSetTransform(transform.GetMatrix().m);
	AEGfxMeshDraw(Texmesh, AE_GFX_MDM_TRIANGLES);
}

void BaseProjectile::Exit()
{
	AEGfxTextureUnload(projTex);
}

float BaseProjectile::GetLifespan()
{
	return currLifespan;
}