#include "../System/MeshManager.h"
#include "../Player/Transform2D.h"
#include "../Player/Player.h"
#pragma once
class TimeStop
{
private:
	bool active{};
	float zoneScale = 500.f;
	MeshManager* meshManager = nullptr;
	Player *player = nullptr;
	float resource{ 100 };
	float maxResource{ 100 };
public:
	Transform2D resourceBarTfm{};
	Transform2D zoneTransform{};
	void Init();
	void Update(float deltaTime);
	void ToggleTime(AEVec2 position);
	bool TimeStopped();
	float GetScale();
	void SpawnZone(AEVec2 position);
	void Draw();
	void Free();
};

