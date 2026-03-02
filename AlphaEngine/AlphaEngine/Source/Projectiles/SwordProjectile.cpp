#include "SwordProjectile.h"

SwordProjectile::SwordProjectile(AEVec2 pos, AEVec2 dir)
	: BaseProjectile(pos, dir, true)
{
	speed = 1000.f;

	transform.SetScale(size * 5.f, size * 10.f);
	transform.SetPosition(pos.x, pos.y);

	projTex = AEGfxTextureLoad("Assets/Weapons/swordSlash.png");
}

SwordProjectile::~SwordProjectile()
{
	if (projTex)
	{
		AEGfxTextureUnload(projTex);
	}
	Exit();
}

void SwordProjectile::Update(float deltaTime)
{
	if (GetLifespan() > 0.f)
	{
		speed = slowed ? speed * 2.f : speed;
		if (!stopped)
		{
			SetLifespan(GetLifespan() - 1.f * deltaTime);
			speed *= 0.95f;
			AEVec2 position = transform.GetPosition();
			position.x += speed * GetDirection().x * deltaTime;
			position.y += speed * GetDirection().y * deltaTime;

			transform.SetRotation(atan2f(GetDirection().y, GetDirection().x));
			transform.SetPosition(position.x, position.y);
		}
	}
}