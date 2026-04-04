/*!
@file       AbilityHandler.cpp
@author     Chew Zavier (c.zavier)
@brief      This file contains definitions of functions used by the Ability Handler 
			which the Ability UIs, Cooldowns and activations
*//*_______________________________________________________________________*/
#include "AbilityHandler.h"
#include <iostream>
#include "../Projectiles/BaseProjectile.h"
#include "../Utils/utils.h"
#include "../System/Scenemanager.h"
#include "../Scene/BossScene.h"
AbilityHandler::AbilityHandler()
	:player{ Player::GetInstance() },
	meshManager{ MeshManager::GetInstance() },
	stopCost{ 15 },
	slowCost{ 15 },
	temporalCost{ 50 },
	audioManager{ AudioManager::GetInstance()}
{
	//icon scales
	a1Tfm.SetScale(100, 100);
	a2Tfm.SetScale(a1Tfm.GetScale().x, a1Tfm.GetScale().y);
	a3Tfm.SetScale(a1Tfm.GetScale().x, a1Tfm.GetScale().y);

	//resource bar scale
	barBgTfm.SetScale(350, 35);

	//rotation of the resource bar to be flat
	resourceBarTfm.SetRotation(-90 * PI / 180);

	//texture for the icons
	a1Tex = AEGfxTextureLoad("Assets/Abilities/timeslow.png");
	a2Tex = AEGfxTextureLoad("Assets/Abilities/hourglass.png");
	a3Tex = AEGfxTextureLoad("Assets/Abilities/tempAnch.png");

	//resource bar textures
	resourceBarOverlay = AEGfxTextureLoad("Assets/Abilities/healthbar.png");
	barBG = AEGfxTextureLoad("Assets/Abilities/resourcebar.png");
}

AbilityHandler::~AbilityHandler()
{
	//if the sprites exist, unload them
	if (a1Tex)
		AEGfxTextureUnload(a1Tex);
	if (a2Tex)
		AEGfxTextureUnload(a2Tex);
	if (a3Tex)
		AEGfxTextureUnload(a3Tex);
	if(resourceBarOverlay)
		AEGfxTextureUnload(resourceBarOverlay);
	if (barBG)
		AEGfxTextureUnload(barBG);

	//nullptr all pointers to other objects
	player = nullptr;
	meshManager = nullptr;
	audioManager = nullptr;

	//delete the abilities
	delete timeStop;
	delete timeSlow;
	delete tempAnch;
}

void AbilityHandler::AbilityUpdate(std::vector<BaseEnemy*> enemyList, float dt)
{

	//grab cam pos
	AEGfxGetCamPosition(&camPos.x, &camPos.y);

	//update the abilities
	timeStop->Update(enemyList, dt);
	timeSlow->Update(dt);
	tempAnch->Update(dt);
	//check for inputs and activate/deactivate the abilities
	if (AEInputCheckTriggered(AEVK_E))
	{
		timeStopped = timeStop->ToggleTime(player->GetTransform().GetPosition());
	}

	if (AEInputCheckTriggered(AEVK_Q))
	{
		timeSlowed = timeSlow->ToggleSlow(enemyList);
	}

	if (AEInputCheckTriggered(AEVK_F))
	{
		if(resource >= temporalCost || anchored)
			TriggerAnchor();

		resource = anchored ? resource - temporalCost : resource;
	}

	
	//temporarily stop time if the player is reversing 
	timeStop->TempStop(Reverse());

	//anchored boolean dependant on if hte player is currently anchored
	anchored = tempAnch->Anchor();
	//separated into 2 if statements so that the cost is additive
	if (timeStopped)
	{
		resource -= 15 * dt;
	}
	if (timeSlowed)
	{
		resource -= 15 * dt;
	}
	//if no more resource stop all abilities that are not temp anchor
	if (resource <= 0)
	{
		timeStopped = timeStopped ? timeStop->ToggleTime(player->GetTransform().GetPosition()) : false;
		timeSlowed = timeSlowed ? timeSlow->ToggleSlow(enemyList) : false;
	}

	//if no abilities active , slowly restore it
	if (!timeStopped && !timeSlowed && !anchored)
	{
		resource += 10 * dt;
	}

	//alpha based on if the ability is active / have enough resource for the ability
	a1Alpha = timeSlowed ? 0.5f : resource < stopCost? 0.5f:1.0f;
	a2Alpha = timeStopped ? 0.5f : resource < slowCost ? 0.5f: 1.f;
	a3Alpha = anchored ? 0.5f : resource < temporalCost ? 0.5f : 1.f;

	//clamp the amount of resource the player can have
	resource = AEClamp(resource, 0, maxResource);
	resourceBarTfm.SetScale(17, (resource / maxResource * 280.f));

	//set the resource bar position based on camera so its always top left
	resourceBarTfm.SetPosition(camPos.x - AEGfxGetWindowWidth() / 2.f + 213, camPos.y + AEGfxGetWindowHeight() / 2.f - 119.f);
	barBgTfm.SetPosition(camPos.x - AEGfxGetWindowWidth() / 2.f + 178, camPos.y + AEGfxGetWindowHeight()/2.f - 145.f);

	//set the positions of the icons based on the camera
	a1Tfm.SetPosition(camPos.x + AEGfxGetWindowWidth() / 2.f - a1Tfm.GetScale().x * 3.f, camPos.y - AEGfxGetWindowHeight() / 2.f);
	a2Tfm.SetPosition(camPos.x + AEGfxGetWindowWidth() / 2.f - a2Tfm.GetScale().x * 2.f, camPos.y - AEGfxGetWindowHeight() / 2.f);
	a3Tfm.SetPosition(camPos.x + AEGfxGetWindowWidth() / 2.f - a3Tfm.GetScale().x, camPos.y - AEGfxGetWindowHeight() / 2.f);
}

void AbilityHandler::Draw()
{
	if (showUI)
	{
		//for the VFX of the abilities
		timeStop->Draw();
		timeSlow->Draw();
		tempAnch->Draw();

		//for resource bar bg
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetColorToAdd(0.f, 0.f, 0.f, 0.0f);

		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1);

		AEGfxTextureSet(barBG, 0, 0);

		AEGfxSetTransform(barBgTfm.GetMatrix().m);
		AEGfxMeshDraw(meshManager->GetMesh(CORNER_TEXTURE_SQUARE), AE_GFX_MDM_TRIANGLES);

		//resource bar for the abilties
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.f);
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetColorToAdd(1.f, 1.f, 255.f, 0.f);
		AEGfxSetTransform(resourceBarTfm.GetMatrix().m);
		AEGfxMeshDraw(meshManager->GetMesh(CORNER_SQUARE), AE_GFX_MDM_TRIANGLES);


		//for ability 1 UI
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetColorToAdd(0.f, 0.f, 0.f, 0.0f);

		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(a1Alpha);

		AEGfxTextureSet(a1Tex, 0, 0);
		AEGfxSetTransform(a1Tfm.GetMatrix().m);
		AEGfxMeshDraw(meshManager->GetMesh(CORNER_TEXTURE_SQUARE), AE_GFX_MDM_TRIANGLES);

		//for ability 2 UI
		AEGfxSetTransparency(a2Alpha);

		AEGfxTextureSet(a2Tex, 0, 0);
		AEGfxSetTransform(a2Tfm.GetMatrix().m);
		AEGfxMeshDraw(meshManager->GetMesh(CORNER_TEXTURE_SQUARE), AE_GFX_MDM_TRIANGLES);

		//ability 3 Ui
		AEGfxSetTransparency(a3Alpha);

		AEGfxTextureSet(a3Tex, 0, 0);
		AEGfxSetTransform(a3Tfm.GetMatrix().m);
		AEGfxMeshDraw(meshManager->GetMesh(CORNER_TEXTURE_SQUARE), AE_GFX_MDM_TRIANGLES);

	}
}
//returns if player is anchored
bool AbilityHandler::Anchored() const
{
	return anchored;
}
//returns if the player is currently already reversing in time
bool AbilityHandler::Reverse() const
{
	return tempAnch->Reversing();
}
//triggers the anchor
void AbilityHandler::TriggerAnchor()
{
	anchored = tempAnch->Anchor(player->GetTransform().GetPosition());
}
void AbilityHandler::ShowUI(bool show)
{
	showUI = show;
}
