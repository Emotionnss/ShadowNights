#include "BaseProjectile.h"
#include <iostream>
#include "../System/MeshManager.h"

BaseProjectile::BaseProjectile(AEVec2 pos, AEVec2 dir, bool skipTextureLoad)
	: direction{ dir }
	, oriSpeed{500.f}
	, speed{ oriSpeed}
	, size{ 25.f }
	, projTex{ nullptr }
{
	transform.SetScale(size, size);
	transform.SetPosition(pos.x, pos.y);

	if (!skipTextureLoad)
		projTex = AEGfxTextureLoad("Assets/Weapons/knife.png");
}


BaseProjectile::~BaseProjectile()
{
	Exit();
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
	AEGfxMeshDraw(MeshManager::GetInstance()->GetMesh(TEXTURE_SQUARE), AE_GFX_MDM_TRIANGLES);
}

void BaseProjectile::Exit()
{
	if (projTex)
	{
		AEGfxTextureUnload(projTex);
	}
}

float BaseProjectile::GetLifespan()
{
	return currLifespan;
}

void BaseProjectile::SetLifespan(float time)
{
	currLifespan = time;
}

void BaseProjectile::SetSpeed(float speed)
{
		this->speed = speed;
}

AEVec2 BaseProjectile::GetDirection()
{
	return direction;
}