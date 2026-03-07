#pragma once
#include "TimeStop.h"
#include "TimeSlow.h"
#include "TemporalAnchor.h"

#include "BossLaser.h"
#include "BossExplosive.h"


#include "../System/MeshManager.h"
#include "../Sprite/Sprite.h"

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

	Player* player = nullptr;
	f32 slowCost{}, stopCost, temporalCost;
	f32 maxResource{ 100.f };//the max amt of resource available to the player
	f32 resource{ 100.f };//the resource used for stopping time
	Transform2D resourceBarTfm{};//transform for the resource bar 

	MeshManager* meshManager = nullptr;//mesh manager
	AEVec2 camPos;//position of camera for resource bar

	AEGfxTexture* a1Tex;
	AEMtx33 a1Tfm{};
	bool timeStopped{};
	bool timeSlowed{};

	bool anchored{};

public:
	void TriggerAnchor();
	AbilityHandler();
	~AbilityHandler();
	void AbilityUpdate(std::vector<BaseEnemy*> enemylist,float deltaTime);
	void Draw();
	bool Anchored()const;
	bool Reverse() const;
};

