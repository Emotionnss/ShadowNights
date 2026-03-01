#include "AbilityHandler.h"
#include <iostream>
#include "../Projectiles/BaseProjectile.h"
#include "../utils.h"
AbilityHandler::AbilityHandler()
	:player{ Player::GetInstance() },
	meshManager{ MeshManager::GetInstance() },
	stopCost{ 15 },
	slowCost{ 15 }
{
	AEGfxGetCamPosition(&camPos.x, &camPos.y);
}

AbilityHandler::~AbilityHandler()
{
	player = nullptr;
	delete timeStop;
	delete timeSlow;
	delete tempAnch;
}

void AbilityHandler::AbilityUpdate(std::vector<BaseEnemy*> enemyList,float dt)
{
	AEGfxGetCamPosition(&camPos.x, &camPos.y);
	timeStop->Update(enemyList,dt);
	timeSlow->Update(enemyList,dt);
	tempAnch->Update(dt);
	if (AEInputCheckTriggered(AEVK_E))
	{
		timeStopped = timeStop->ToggleTime(player->GetTransform().GetPosition());
	}
	if (AEInputCheckTriggered(AEVK_Q))
	{
		timeSlowed = timeSlow->ToggleSlow();
	}
	if (AEInputCheckTriggered(AEVK_F))
	{
		anchored = tempAnch->Anchor(player->GetTransform().GetPosition());
	}
	if (timeStopped)
	{
		resource -= 15 * dt;
	}
	if (timeSlowed)
	{
		resource -= 15 * dt;
	}

	if (resource <= 0)
	{
		timeStopped = timeStopped?timeStop->ToggleTime(player->GetTransform().GetPosition()):false;
		timeSlowed = timeSlowed?timeSlow->ToggleSlow():false;
	}

	if (!timeStopped && !timeSlowed)
	{
		resource += 100 * dt;
	}
	
	resource = AEClamp(resource, 0, maxResource);
	resourceBarTfm.SetScale(20, (resource / maxResource * 100.f));
	resourceBarTfm.SetPosition(camPos.x + 450.f, camPos.y - 200.f);
}

void AbilityHandler::Draw()
{

	timeStop->Draw();
	timeSlow->Draw();
	tempAnch->Draw();
	//resource bar for the abilties
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(1.f, 1.f, 255.f, 0.f);
	AEGfxSetTransform(resourceBarTfm.GetMatrix().m);
	AEGfxMeshDraw(meshManager->GetMesh(CORNER_SQUARE), AE_GFX_MDM_TRIANGLES);
}
