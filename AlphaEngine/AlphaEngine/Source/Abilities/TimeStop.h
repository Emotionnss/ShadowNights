#include "../System/MeshManager.h"
#include "../Player/Transform2D.h"
#include "../Player/Player.h"
#include "../Sprite/Sprite.h"
#pragma once
class TimeStop
{
private:
	bool active{};
	f32 originalScale = 1.f;
	f32 zoneScale = 1.f;
	f32 maxScale = 500.f;
	MeshManager* meshManager = nullptr;
	Player *player = nullptr;
	f32 resource{ 100 };
	f32 maxResource{ 100 };
	f32 timeStopped{ 0.f };
	f32 originalProjSpeed{ 500.f };
	Sprite* sprite;
public:
	Transform2D resourceBarTfm{};
	Transform2D zoneTransform{};
	void Init();
	void Update(float deltaTime);
	void ToggleTime(AEVec2 position);
	bool TimeStopped();
	f32 GetScale();
	void SetZonePos(AEVec2 position);
	void SetZoneScale();
	void Draw();
	void Free();
};

