#include "AbilityHandler.h"
#include <iostream>
#include "../Projectiles/BaseProjectile.h"
#include "../Utils/utils.h"

AbilityHandler::AbilityHandler()
	:player{ Player::GetInstance() },
	meshManager{ MeshManager::GetInstance() },
	stopCost{ 15 },
	slowCost{ 15 },
	temporalCost{ 50 }
{
	AEGfxGetCamPosition(&camPos.x, &camPos.y);
	AEMtx33	trans, rot, scale;
	AEMtx33Scale(&scale, 100, 100);
	AEMtx33Rot(&rot, 0);
	AEMtx33Trans(&trans, camPos.x, camPos.y);

	AEMtx33Concat(&a1Tfm, &rot, &scale);//concatenate rotation and scale together
	AEMtx33Concat(&a1Tfm, &trans, &a1Tfm);//concatenate the transform and the translate
	a1Tex = AEGfxTextureLoad("Assets/Abilities/hourglass.png");
}

AbilityHandler::~AbilityHandler()
{
	if(a1Tex)
		AEGfxTextureUnload(a1Tex);
	player = nullptr;
	meshManager = nullptr;
	delete timeStop;
	delete timeSlow;
	delete tempAnch;

	delete bossLaser;
}

void AbilityHandler::AbilityUpdate(std::vector<BaseEnemy*> enemyList,float dt)
{
	AEGfxGetCamPosition(&camPos.x, &camPos.y);
	timeStop->Update(enemyList,dt);
	timeSlow->Update(enemyList,dt);
	tempAnch->Update(dt);

	bossLaser->Update(dt);


	if (AEInputCheckTriggered(AEVK_E))
	{
		timeStopped = timeStop->ToggleTime(player->GetTransform().GetPosition());
	}

	if (AEInputCheckTriggered(AEVK_Q))
	{
		timeSlowed = timeSlow->ToggleSlow();
	}

	if (AEInputCheckTriggered(AEVK_R))
	{
		player->Health(100);
	}

	if (AEInputCheckTriggered(AEVK_F) && resource >= temporalCost)
	{
		TriggerAnchor();

		resource = anchored ? resource - temporalCost : resource;
	}
	timeStop->TempStop(Reverse());
	anchored = tempAnch->Anchor();

	if (AEInputCheckTriggered(AEVK_T))
	{
		lasered = bossLaser->Triggered(player->GetTransform().GetPosition());
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

	if (!timeStopped && !timeSlowed && !anchored)
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

	bossLaser->Draw();

	//resource bar for the abilties
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	//AEGfxSetColorToAdd(1.f, 1.f, 255.f, 0.f);
	AEGfxSetTransform(resourceBarTfm.GetMatrix().m);
	AEGfxMeshDraw(meshManager->GetMesh(CORNER_SQUARE), AE_GFX_MDM_TRIANGLES);


	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.f, 0.f, 0.f, 0.0f);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.f);

	AEGfxTextureSet(a1Tex, 0, 0);
	AEGfxSetTransform(a1Tfm.m);
	AEGfxMeshDraw(meshManager->GetMesh(TEXTURE_SQUARE), AE_GFX_MDM_TRIANGLES);
}

bool AbilityHandler::Anchored() const
{
	return anchored;
}

bool AbilityHandler::Reverse() const
{
	return tempAnch->Reversing();
}

void AbilityHandler::TriggerAnchor()
{
	anchored = tempAnch->Anchor(player->GetTransform().GetPosition());
}

