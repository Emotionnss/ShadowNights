#pragma once
#include "TimeStop.h"
#include "TimeSlow.h"
#include "../System/MeshManager.h"
class AbilityHandler
{
private:
	//TimeSlow timeSlow{};
	TimeStop *timeStop = new TimeStop();
	TimeSlow* timeSlow = new TimeSlow();
	Player* player = nullptr;
	f32 maxResource{ 100.f };//the max amt of resource available to the player
	f32 resource{ 100.f };//the resource used for stopping time
	Transform2D resourceBarTfm{};//transform for the resource bar 

	MeshManager* meshManager = nullptr;//mesh manager
	AEVec2 camPos;//position of camera for resource bar
	bool timeStopped{};
	bool timeSlowed{};
public:
	AbilityHandler();
	~AbilityHandler();
	void AbilityUpdate(std::vector<BaseEnemy*> enemylist,float deltaTime);
	void Draw();

};

