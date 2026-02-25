#include "AbilityHandler.h"
#include <iostream>
#include "../Projectiles/BaseProjectile.h"
#include "../utils.h"
AbilityHandler::AbilityHandler()
	:player{Player::GetInstance()},meshManager{MeshManager::GetInstance()}
{
	player = Player::GetInstance();
	
}

AbilityHandler::~AbilityHandler()
{
	timeStop->Free();
	delete timeStop;
}

void AbilityHandler::AbilityUpdate(float dt)
{
	timeStop->Update(dt);
	if (AEInputCheckTriggered(AEVK_F))
	{
		timeStopped = timeStop->ToggleTime(player->GetTransform().GetPosition());
	}
	if (AEInputCheckTriggered(AEVK_Q))
	{
		
	}

	if (timeStopped)
	{
		resource -= 30 * dt;
	}

	if (resource <= 0)
	{
		timeStopped = timeStop->ToggleTime(player->GetTransform().GetPosition());
	}

	if (!timeStopped)
	{
		resource += 100 * dt;
	}
	resource = AEClamp(resource, 0, maxResource);
	resourceBarTfm.SetScale(20, (resource / maxResource * 100.f));
	

	AEGfxGetCamPosition(&camPos.x, &camPos.y);
	resourceBarTfm.SetPosition(camPos.x + 450.f, camPos.y - 200.f);
}

void AbilityHandler::Draw()
{

	timeStop->Draw();

	//resource bar for timestop
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(1.f, 1.f, 255.f, 0.f);
	AEGfxSetTransform(resourceBarTfm.GetMatrix().m);
	AEGfxMeshDraw(meshManager->GetMesh(CORNER_SQUARE), AE_GFX_MDM_TRIANGLES);
}
