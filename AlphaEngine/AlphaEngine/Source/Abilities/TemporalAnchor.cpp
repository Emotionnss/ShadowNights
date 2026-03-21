#include "TemporalAnchor.h"
#include <iostream>
#include "../Utils/utils.h"
#include "../Sprite/Sprite.h"

TemporalAnchor::TemporalAnchor() ://default constructor
	active{ false },
	activeTime{ 3.f },
	activeTimer{ activeTime },
	player{ Player::GetInstance() },
	zoneSprite{},
	anchorSprite{},
	audioManager{AudioManager::GetInstance()}
{
	zoneTfm.SetScale(1000, 1000);
	anchorTfm.SetScale(100, 100);
	anchorSpawnTfm.SetScale(100, 100);
	anchorSprite = new Sprite(CSVManager::ReadInfo("anchor"));//create new sprite for the anchor
	anchorSprite->PlayAnimation("spawn", -1, 0.5f);//make it play an animation on enter

	anchorSpawnSprite = new Sprite(CSVManager::ReadInfo("anchorSpawn"));
	anchorSpawnSprite->PlayAnimation("spawn", 1, spawnTime);

	ghostTfm.SetScale(player->GetTransform().GetScale().x, player->GetTransform().GetScale().y);//initialize the ghost's scale to match the player
}

TemporalAnchor::~TemporalAnchor()//default destructor
{
	player = nullptr;//make the pointer point to nothing so that player can handle its own destruction
	if (zoneSprite)//if there IS  a zone sprite
	{
		delete zoneSprite;//delete it
		zoneSprite = nullptr;//point to nothing
	}
	if (anchorSprite)//same for anchor
	{
		delete anchorSprite;
		anchorSprite = nullptr;
	}
	if (anchorSpawnSprite)
	{
		delete anchorSpawnSprite;
		anchorSpawnSprite = nullptr;
	}
	ghostVect.clear();//clear the vector
}

void TemporalAnchor::Update(f32 dt)//update function based on dt
{
	if (spawning)
	{
		std::cout << "in2\n";
		anchorSpawnTfm.SetPosition(player->GetTransform().GetPosition());
		spawnTimer += -1 * dt;
		if (spawnTimer <= 0)
		{
			std::cout << "in2\n";
			spawnTimer = spawnTime;
			spawning = false;
			active = true;

			anchorTfm.SetPosition(anchorSpawnTfm.GetPosition());
			zoneTfm.SetPosition(anchorSpawnTfm.GetPosition());
		}
	}
	if (active && !reversing && !spawning)//if the abilitiy is active AND not reversing the player back to the anchor position
	{
		activeTimer += -1 * dt;//countdown for how long the ability can be active for ;; default is 3
		saveTimer += -1 * dt;//a countdown for how many times per second the position should save into the vector;; default counter is 0.1
		if (saveTimer <= 0)
		{
			AEVec2 UV = player->GetSprite()->GetCurrentUVCoords();//get the UV coords of the current sprite
			SaveGhost(UV, player->GetTransform().GetPosition());//save the position and UV
			saveTimer = saveTime;//reset timer


		}
		if (!AreCirclesIntersecting(player->GetTransform().GetPosition().x, player->GetTransform().GetPosition().y, 0.1f,
			anchorTfm.GetPosition().x, anchorTfm.GetPosition().y, zoneTfm.GetScale().x))//if player goes out of zone
		{
			active = false;//deactivate
			ghostVect.clear();//clear the vector for next use
		}
		else
		{
			if (activeTimer <= 0 && !reversing)//if active timer is up and not yet reversing
			{
				audioManager->PlaySFXOneShot("time_reverse");
				reversing = true;//start reversing
				iter = ghostVect.size() - 1;//start from the BACK of the vector to simulate reversing time
			}
		}
	}
	if (reversing)//if reversing
	{
		if (!ghostVect.empty())//and the ghost vector is NOT empty
		{
			player->GetTransform().SetPosition(ghostVect[iter].position);//set player position to the ghost's position
			ghostVect.erase(ghostVect.begin() + iter--);//erase that particular iterators' data and decrement the iterator
		}
		else//once its empty
		{
			reversing = false;//stop reversing
			active = false;//stop activity
			player->SetHealth(savedHealth);//reset the health to what it had before
			player->GetTransform().SetPosition(anchorTfm.GetPosition());
		}
	}
	else if (!active && !reversing && !spawning)//once its not active nor reversing
	{
		activeTimer = activeTime;//reset timer
		saveTimer = saveTime;//reset save timer
		spawnTimer = spawnTime;
	}
	anchorSprite->Update();//update the sprite for the anchor
	anchorSpawnSprite->Update();//update sprite for the anchor spawning
}

bool TemporalAnchor::Anchor(AEVec2 anchPos)
{
	if (active)
	{
		active = false;
		spawning = false;
		reversing = true;
		iter = ghostVect.size() - 1;
		//player->GetTransform().SetPosition(anchorTfm.GetPosition());

	}
	else
	{
		//active = true;
		spawning = true;
		savedHealth = player->GetHealth();

		anchorSpawnSprite->PlayAnimation("spawn", 1, spawnTime);
	}
	return active;
}

bool TemporalAnchor::Anchor() const
{
	return active;
}

void TemporalAnchor::Draw()
{
	//zone for timestop
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	if(spawning)
		anchorSpawnSprite->Render(anchorSpawnTfm);
	if (active)
	{
		AEGfxSetTransparency(0.25f);
		AEGfxSetColorToMultiply(0.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetColorToAdd(0.f, 0.f, 255.f, 0.f);
		AEGfxSetTransform(zoneTfm.GetMatrix().m);
		AEGfxMeshDraw(MeshManager::GetInstance()->GetMesh(CIRCLE), AE_GFX_MDM_TRIANGLES);

		anchorSprite->Render(anchorTfm);
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetTransparency(0.25f);
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

		Sprite* ghostSprite = player->GetSprite();

		for (Ghosts ghost : ghostVect)
		{

			ghostTfm.SetPosition(ghost.position);
			AEGfxTextureSet(ghostSprite->GetSpriteTexture(), ghost.spriteUV.x, ghost.spriteUV.y);
			AEGfxSetTransform(ghostTfm.GetMatrix().m);
			AEGfxMeshDraw(ghostSprite->GetSpriteMesh(), AE_GFX_MDM_TRIANGLES);
		}

	}
}

void TemporalAnchor::SaveGhost(AEVec2 UVs, AEVec2 playerPos)
{
	Ghosts ghost{ UVs,playerPos };
	ghostVect.push_back(ghost);
	/*std::cout << "saving UV" << UVs.x << " " << UVs.y << " vector size" << ghostVect.size() << '\n';*/
}

bool TemporalAnchor::Reversing() const
{
	return reversing;
}