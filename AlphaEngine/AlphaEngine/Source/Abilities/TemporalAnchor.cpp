#include "TemporalAnchor.h"
#include <iostream>
#include "../Utils/utils.h"
#include "../Sprite/Sprite.h"
TemporalAnchor::TemporalAnchor() :
	active{ false },
	activeTime{ 3.f },
	activeTimer{ activeTime },
	player{ Player::GetInstance() },
	zoneSprite{},
	anchorSprite{}
{
	zoneTfm.SetScale(1000, 1000);
	anchorTfm.SetScale(100, 100);
	anchorSprite = new Sprite(CSVManager::ReadInfo("anchor"));//create new sprite for the anchor
	anchorSprite->PlayAnimation("spawn", -1, 0.5f);

	ghostTfm.SetScale(player->GetTransform().GetScale().x, player->GetTransform().GetScale().y);
}

TemporalAnchor::~TemporalAnchor()
{
	player = nullptr;
	if (zoneSprite)
	{
		delete zoneSprite;
		zoneSprite = nullptr;
	}
	if (anchorSprite)
	{
		delete anchorSprite;
		anchorSprite = nullptr;
	}
	ghostVect.clear();
}

void TemporalAnchor::Update(f32 dt)
{
	if (active && !reversing)
	{
		activeTimer += -1 * dt;
		saveTimer += -1 * dt;
		if (saveTimer <= 0)
		{
			AEVec2 UV = player->GetSprite()->GetCurrentUVCoords();
			saveTimer = saveTime;

			SaveGhost(UV, player->GetTransform().GetPosition());

		}
		if (!AreCirclesIntersecting(player->GetTransform().GetPosition().x, player->GetTransform().GetPosition().y, player->GetTransform().GetScale().x,
			anchorTfm.GetPosition().x, anchorTfm.GetPosition().y, zoneTfm.GetScale().x))
		{
			active = false;
			ghostVect.clear();
		}
		else
		{
			if (activeTimer <= 0 && !reversing)
			{
				reversing = true;
				iter = ghostVect.size() - 1;
			}
		}
	}
	if (reversing)
	{
		if (!ghostVect.empty())
		{

			player->GetTransform().SetPosition(ghostVect[iter].position);
			ghostVect.erase(ghostVect.begin() + iter--);
		}
		else
		{
			reversing = false;
			active = false;
			//player->GetTransform().SetPosition(anchorTfm.GetPosition());
			player->SetHealth(savedHealth);
		}
	}
	else if (!active && !reversing)
	{
		activeTimer = activeTime;
		saveTimer = saveTime;
	}
	anchorSprite->Update();
}

bool TemporalAnchor::Anchor(AEVec2 anchPos)
{
	if (active)
	{
		active = false;
		reversing = true;
		iter = ghostVect.size() - 1;
		//player->GetTransform().SetPosition(anchorTfm.GetPosition());

	}
	else
	{
		active = true;
		savedHealth = player->GetHealth();
		anchorTfm.SetPosition(anchPos);
		zoneTfm.SetPosition(anchPos);
		anchorSprite->PlayAnimation("spawn", -1, 0.5f);
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