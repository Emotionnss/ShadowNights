#include "BaseProjectile.h"

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
}

BaseProjectile::~BaseProjectile()
{

}

void BaseProjectile::Init()
{

}
void BaseProjectile::Update(float deltaTime)
{
	if (currLifespan > 0.f)
	{
		currLifespan -= 1.f * deltaTime;
	}

	AEVec2 position = transform.GetPosition();
	position.x += speed * direction.x * deltaTime;
	position.y += speed * direction.y * deltaTime;

	transform.SetPosition(position.x, position.y);
}

void BaseProjectile::Draw(AEGfxVertexList* mesh)
{
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetColorToAdd(-1.f, -1.f, 0.7f, 0.0f);
	AEGfxSetTransform(transform.GetMatrix().m);
	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
}

void BaseProjectile::Exit()
{
}

float BaseProjectile::GetLifespan()
{
	return currLifespan;
}