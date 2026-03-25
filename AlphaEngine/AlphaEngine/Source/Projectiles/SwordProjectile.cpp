#include "SwordProjectile.h"
#include "../particle/ParticleManager.h"

AEGfxTexture* sharedSwordTex = nullptr;

SwordProjectile::SwordProjectile(AEVec2 pos, AEVec2 dir)
	: BaseProjectile(pos, dir, true)
{
	speed = 1200.f; // Slightly faster for anime-wave feel
	transform.SetScale(size * 5.f, size * 10.f);
	transform.SetPosition(pos.x, pos.y);

	collidesWithTiles = false;

	if (sharedSwordTex == nullptr) {
		sharedSwordTex = AEGfxTextureLoad("Assets/Weapons/swordSlash.png");
	}
	projTex = sharedSwordTex;
}

SwordProjectile::~SwordProjectile()
{
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
			speed *= 0.98f;

			AEVec2 position = transform.GetPosition();
			position.x += speed * GetDirection().x * deltaTime;
			position.y += speed * GetDirection().y * deltaTime;

			transform.SetRotation(atan2f(GetDirection().y, GetDirection().x));
			transform.SetPosition(position.x, position.y);

			AEVec2 trailScale = { transform.GetScale().x * 0.8f, transform.GetScale().y * 0.8f };

			//Glowing Tapered Sword Wave Trail
			ParticleManager::GetInstance()->SpawnTrail(
				transform.GetPosition(),
				trailScale,
				0.2f,
				1.0f, 0.0f, 0.0f,
				transform.GetRotation(),
				projTex
			);
		}
	}
}

void ShutdownSwordResources() {
	if (sharedSwordTex != nullptr) {
		AEGfxTextureUnload(sharedSwordTex);
		sharedSwordTex = nullptr;
	}
}