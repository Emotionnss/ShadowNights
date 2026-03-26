#include "AbilityHandler.h"
#include <iostream>
#include "../Projectiles/BaseProjectile.h"
#include "../Utils/utils.h"

AbilityHandler::AbilityHandler()
	:player{ Player::GetInstance() },
	meshManager{ MeshManager::GetInstance() },
	stopCost{ 15 },
	slowCost{ 15 },
	temporalCost{ 50 },
	audioManager{ AudioManager::GetInstance()}
{
	AEGfxGetCamPosition(&camPos.x, &camPos.y);

	a1Tfm.SetScale(100, 100);
	a2Tfm.SetScale(a1Tfm.GetScale().x, a1Tfm.GetScale().y);
	a3Tfm.SetScale(a1Tfm.GetScale().x, a1Tfm.GetScale().y);

	//barBgTfm.SetRotation(-90 * PI/180);
	//barOverlayTfm.SetRotation(-90 * PI / 180);

	barBgTfm.SetScale(350, 35);
	//barOverlayTfm.SetScale(500, -100);//negative scale cause if positive it scales mirrored for some reason ? ? ? ? ?

	resourceBarTfm.SetRotation(-90 * PI / 180);
	a1Tex = AEGfxTextureLoad("Assets/Abilities/timeslow.png");
	a2Tex = AEGfxTextureLoad("Assets/Abilities/hourglass.png");
	a3Tex = AEGfxTextureLoad("Assets/Abilities/tempAnch.png");
	resourceBarOverlay = AEGfxTextureLoad("Assets/Abilities/healthbar.png");
	barBG = AEGfxTextureLoad("Assets/Abilities/resourcebar.png");
}

AbilityHandler::~AbilityHandler()
{
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

	player = nullptr;
	meshManager = nullptr;
	audioManager = nullptr;
	delete timeStop;
	delete timeSlow;
	delete tempAnch;

	for (BossLaser* l : bossLasers)
		delete l;
	bossLasers.clear();

	for (BossExplosive* e : bossExplosives)
		delete e;
	bossExplosives.clear();
}

void AbilityHandler::AbilityUpdate(std::vector<BaseEnemy*> enemyList, float dt)
{
	AEGfxGetCamPosition(&camPos.x, &camPos.y);
	timeStop->Update(enemyList, dt);
	timeSlow->Update(dt);
	tempAnch->Update(dt);

	for (BossLaser* l : bossLasers)
		l->Update(dt);

	for (BossExplosive* e : bossExplosives)
		e->Update(dt);


	if (AEInputCheckTriggered(AEVK_E))
	{
		timeStopped = timeStop->ToggleTime(player->GetTransform().GetPosition());
	}

	if (AEInputCheckTriggered(AEVK_Q))
	{
		timeSlowed = timeSlow->ToggleSlow(enemyList);
	}

	if (AEInputCheckTriggered(AEVK_R))
	{
		player->SetHealth(100);
	}

	if (AEInputCheckTriggered(AEVK_F))
	{
		if(resource >= temporalCost || anchored)
			TriggerAnchor();

		resource = anchored ? resource - temporalCost : resource;
	}
	timeStop->TempStop(Reverse());
	anchored = tempAnch->Anchor();

	if (AEInputCheckTriggered(AEVK_T))
	{
		BossLaser* newLaser = new BossLaser();
		newLaser->Triggered(player->GetTransform().GetPosition());
		bossLasers.push_back(newLaser);
	}
	if (AEInputCheckTriggered(AEVK_G))
	{
		BossExplosive* newExplosive = new BossExplosive();
		newExplosive->Triggered(player->GetTransform().GetPosition(), { 1.f, 0.f });
		bossExplosives.push_back(newExplosive);
	}


	//separated into 2 if statements so that the cost is additive
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
		timeStopped = timeStopped ? timeStop->ToggleTime(player->GetTransform().GetPosition()) : false;
		timeSlowed = timeSlowed ? timeSlow->ToggleSlow(enemyList) : false;
	}

	if (!timeStopped && !timeSlowed && !anchored)
	{
		resource += 10 * dt;
	}
	if (resource >= maxResource)
	{
		showBar = false;
	}
	else
	{
		showBar= true;
	}
	a1Alpha = timeSlowed ? 0.5f : 1.f;
	a2Alpha = timeStopped ? 0.5f : 1.f;
	a3Alpha = anchored ? 0.5f : resource < temporalCost ? 0.5f : 1.f;

	resource = AEClamp(resource, 0, maxResource);
	resourceBarTfm.SetScale(17, (resource / maxResource * 280.f));

	resourceBarTfm.SetPosition(camPos.x - AEGfxGetWindowWidth() / 2.f + 203, camPos.y + AEGfxGetWindowHeight() / 2.f - 125.f);
	barOverlayTfm.SetPosition(camPos.x - AEGfxGetWindowWidth() / 2.f + 20, camPos.y);
	barBgTfm.SetPosition(camPos.x - AEGfxGetWindowWidth() / 2.f + 168, camPos.y + AEGfxGetWindowHeight()/2.f - 150.f);

	a1Tfm.SetPosition(camPos.x + AEGfxGetWindowWidth() / 2.f - a1Tfm.GetScale().x * 3.f, camPos.y - AEGfxGetWindowHeight() / 2.f);
	a2Tfm.SetPosition(camPos.x + AEGfxGetWindowWidth() / 2.f - a2Tfm.GetScale().x * 2.f, camPos.y - AEGfxGetWindowHeight() / 2.f);
	a3Tfm.SetPosition(camPos.x + AEGfxGetWindowWidth() / 2.f - a3Tfm.GetScale().x, camPos.y - AEGfxGetWindowHeight() / 2.f);
}

void AbilityHandler::Draw()
{

	timeStop->Draw();
	timeSlow->Draw();
	tempAnch->Draw();

	for (BossLaser* l : bossLasers)
		l->Draw();

	for (BossExplosive* e : bossExplosives)
		e->Draw();

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

		////for resource bar overlay
		//AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		//AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		//AEGfxSetColorToAdd(0.f, 0.f, 0.f, 0.0f);

		//AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		//AEGfxSetTransparency(1);
		//
		////AEGfxTextureSet(barBG, 0, 0);

		//AEGfxTextureSet(resourceBarOverlay, 0, 0);
		//AEGfxSetTransform(barOverlayTfm.GetMatrix().m);
		//AEGfxMeshDraw(meshManager->GetMesh(CORNER_TEXTURE_SQUARE), AE_GFX_MDM_TRIANGLES);


	

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