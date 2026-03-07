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
	anchorSprite = new Sprite(CSVManager::ReadInfo("anchor"));//create new sprite for the anchor
	anchorSprite->PlayAnimation("spawn", -1, 0.5f);
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
	savedPlayerPositions.clear();
}

void TemporalAnchor::Update(f32 dt)
{
	if (active && !reversing)
	{
		activeTimer += -1 * dt;
		saveTimer += -1 * dt;
		if (saveTimer <= 0)
		{
			saveTimer = saveTime;
			SavePositions(player->GetTransform().GetPosition());
		}
		if (!AreCirclesIntersecting(player->GetTransform().GetPosition().x, player->GetTransform().GetPosition().y, player->GetTransform().GetScale().x,
			anchorTfm.GetPosition().x, anchorTfm.GetPosition().y, zoneTfm.GetScale().x))
		{
			active = false;
			savedPlayerPositions.clear();
		}
		else
		{
			if (activeTimer <= 0 && !reversing)
			{
				active = false;
				reversing = true;
				iter = savedPlayerPositions.size() - 1;
			}
		}
	}
	if (reversing)
	{
		if (!savedPlayerPositions.empty())
		{
			player->GetTransform().SetPosition(savedPlayerPositions[iter]);
			savedPlayerPositions.erase(savedPlayerPositions.begin() + iter--);
		}
		else
		{
			reversing = false;
			//player->GetTransform().SetPosition(anchorTfm.GetPosition());
			player->Health(savedHealth);	
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
		iter = savedPlayerPositions.size() - 1;
		//player->GetTransform().SetPosition(anchorTfm.GetPosition());

	}
	else
	{
		active = true;
		savedHealth = player->Health();
		anchorTfm.SetPosition(anchPos);
		zoneTfm.SetPosition(anchPos);
		anchorSprite->PlayAnimation("spawn",-1,0.5f);
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
	}
}

void TemporalAnchor::SavePositions(AEVec2 playerPos)
{
	savedPlayerPositions.push_back(playerPos);
	//std::cout << "saving position" << playerPos.x << " " << playerPos.y << " vector size" << savedPlayerPositions.size() << '\n';
}

bool TemporalAnchor::Reversing() const
{
	return reversing;
}
