/*!
@file       AbilityHandler.h
@author     Chew Zavier (c.zavier)
@brief      This file contains declarations of functions used by the Ability Handler
			which the Ability UIs, Cooldowns and activations
*//*_______________________________________________________________________*/
#pragma once
#include "TimeStop.h"
#include "TimeSlow.h"
#include "TemporalAnchor.h"

#include "BossLaser.h"
#include "BossExplosive.h"


#include "../System/MeshManager.h"
#include "../Sprite/Sprite.h"

#include "../System/AudioManager.h"

#include <vector>
class AbilityHandler
{
private:
	//abilities
	TimeStop* timeStop = new TimeStop();
	TimeSlow* timeSlow = new TimeSlow();
	TemporalAnchor* tempAnch = new TemporalAnchor();

	std::vector<BossLaser*> bossLasers;
	std::vector<BossExplosive*> bossExplosives;

	Player* player = nullptr;//player pointer
	f32 slowCost{}, stopCost, temporalCost;//costs for the abilities
	f32 maxResource{ 100.f };//the max amt of resource available to the player
	f32 resource{ 100.f };//the resource used for stopping time
	Transform2D resourceBarTfm{};//transform for the resource bar 

	MeshManager* meshManager = nullptr;//mesh manager
	AEVec2 camPos{};//position of camera for resource bar

	AEGfxTexture* a1Tex , *a2Tex , *a3Tex,*resourceBarOverlay,*barBG;//UI textures
	Transform2D a1Tfm{},a2Tfm,a3Tfm,barTfm,barOverlayTfm,barBgTfm;//UI transforms
	bool timeStopped{};//stopped time ?
	bool timeSlowed{};//slowed time ? 

	bool anchored{};//temporally anchored? 

	bool lasered{};
	f32 a1Alpha{}, a2Alpha{}, a3Alpha{};//alpha for the icons

	AudioManager* audioManager{};//audio manager
public:
	void TriggerAnchor();
	AbilityHandler();
	~AbilityHandler();
	void AbilityUpdate(std::vector<BaseEnemy*> enemylist,float deltaTime);
	void Draw();
	bool Anchored()const;
	bool Reverse() const;
};

