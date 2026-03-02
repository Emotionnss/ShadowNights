#include "TemporalAnchor.h"
#include <iostream>
#include "../Utils/utils.h"
TemporalAnchor::TemporalAnchor() :
	active{ false },
	activeTime{ 3.f },
	activeTimer{activeTime},
	player{Player::GetInstance()},
	zoneSprite{},
	anchorSprite{}
{
	zoneTfm.SetScale(1000, 1000);
	anchorTfm.SetScale(100, 100);
	anchorSprite = new Sprite(CSVManager::ReadInfo("timestop"));//create new sprite for the anchor
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
}

void TemporalAnchor::Update(f32 dt)
{
	if (active)
	{
		activeTimer += -1 * dt;
		if (!AreCirclesIntersecting(player->GetTransform().GetPosition().x, player->GetTransform().GetPosition().y, player->GetTransform().GetScale().x,
			anchorTfm.GetPosition().x, anchorTfm.GetPosition().y, zoneTfm.GetScale().x))
		{
			std::cout << "out of zone" << '\n';
			active = false;
		}
		else
		{
			std::cout << "in zone" << '\n';
			if (activeTimer <= 0)
			{
				active = false;
				player->GetTransform().SetPosition(anchorTfm.GetPosition());
			}
		}
	}
	else
	{
		activeTimer = activeTime;
	}
}

bool TemporalAnchor::Anchor(AEVec2 anchPos)
{
	if (active)
	{
		active = false;
		player->GetTransform().SetPosition(anchorTfm.GetPosition());
	}
	else
	{
		active = true;
		anchorTfm.SetPosition(anchPos);
		zoneTfm.SetPosition(anchPos);
	}
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
	}
}
