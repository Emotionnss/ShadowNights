/*!
@file       BossExplosive.cpp
@author     Bay Yeok Hao (yeokhao.bay)
@brief      This file contains definitions of functions used by BossExplosive
			which is an object created by BossEnemy when attacking
*//*_______________________________________________________________________*/

#include "BossExplosive.h"
#include <iostream>

// Constructor
BossExplosive::BossExplosive()
	: player{ Player::GetInstance() },
	meshManager{ MeshManager::GetInstance() }
{
	numBullets = 16;
}

BossExplosive::BossExplosive(int numBullets)
	: player{ Player::GetInstance() },
	meshManager{ MeshManager::GetInstance() }
{
	this->numBullets = numBullets;
}

// Overload of constructor for custom parameters
BossExplosive::~BossExplosive()
{
	// Cleanup
	if (bigProjectile)
	{
		if (bigProjectile->projTex)
			AEGfxTextureUnload(bigProjectile->projTex);
		delete bigProjectile;
		bigProjectile = nullptr;
	}

	for (BaseProjectile* proj : smallProjectiles)
	{
		if (proj->projTex)
			AEGfxTextureUnload(proj->projTex);
		delete proj;
	}
	smallProjectiles.clear();
}

// Update
void BossExplosive::Update(float deltaTime)
{
	if (!active)
		return;

	// Move bigProjectile
	if (bigProjectile)
	{
		bigProjectile->Update(deltaTime);
		
		if (bigProjectile->GetLifespan() <= 0.f)
		{
			Explode(bigProjectile->transform.GetPosition());
			if (bigProjectile->projTex)
				AEGfxTextureUnload(bigProjectile->projTex);
			delete bigProjectile;
			bigProjectile = nullptr;
		}
	}
	
	// Move smaller projectiles
	for (BaseProjectile* proj : smallProjectiles)
	{
		proj->Update(deltaTime);

		AEVec2 pos = proj->transform.GetPosition();
		AEVec2 half = { proj->size * 0.5f, proj->size * 0.5f };

		AEVec2 playerPos = player->GetTransform().GetPosition();
		AEVec2 playerHalf = { player->GetTransform().GetScale().x * 0.5f, player->GetTransform().GetScale().y * 0.5f };

		if (AABBOverlap(pos, half, playerPos, playerHalf))
		{
			player->TakeDamage(10);
			proj->SetLifespan(0.f);
		}
	}

	// If smaller projectiles have expired, clean them up
	for (size_t i = 0; i < smallProjectiles.size();)
	{
		if (smallProjectiles[i]->GetLifespan() <= 0.f)
		{
			if (smallProjectiles[i]->projTex)
				AEGfxTextureUnload(smallProjectiles[i]->projTex);
			delete smallProjectiles[i];
			smallProjectiles.erase(smallProjectiles.begin() + i);
		}
		else
		{
			++i;
		}
	}
}

// Render
void BossExplosive::Draw()
{
	// Render bigProjectile
	if (bigProjectile)
		bigProjectile->Draw(meshManager->GetMesh(TEXTURE_SQUARE));

	// Render smaller projectiles
	for (BaseProjectile* proj : smallProjectiles)
	{
		proj->Draw(meshManager->GetMesh(TEXTURE_SQUARE));
	}
}

// Upon activation (spawning) of ability by BossEnemy
bool BossExplosive::Triggered(AEVec2 pos, AEVec2 dir)
{
	if (bigProjectile)
		delete bigProjectile;

	// Spawn dummy bigProjectile
	bigProjectile = new BaseProjectile(pos, dir, true);
	bigProjectile->transform.SetScale(100.f, 100.f);
	bigProjectile->SetLifespan(2.f);
	bigProjectile->projTex = AEGfxTextureLoad("Assets/Weapons/bossProjectile.png");
	bigProjectile->collidesWithTiles = false;
	active = true;

	return true;
}

// Once bigProjectile has expired, explode into smaller projectiles
void BossExplosive::Explode(AEVec2 origin)
{
	// bigProjectile explodes, spawn smaller projectiles where it exploded
	for (int i = 0; i < numBullets; i++)
	{
		float angle = (2 * PI / numBullets) * i;
		AEVec2 dir = { cosf(angle), sinf(angle) };

		BaseProjectile* proj = new BaseProjectile(origin, dir, true);
		proj->SetLifespan(5.f);
		proj->projTex = AEGfxTextureLoad("Assets/Weapons/bossProjectile.png");
		proj->collidesWithTiles = false;

		smallProjectiles.push_back(proj);
	}
}