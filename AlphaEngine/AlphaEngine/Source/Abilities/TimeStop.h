#include "../System/MeshManager.h"
#include "../Player/Transform2D.h"
#include "../Player/Player.h"
#include "../Sprite/Sprite.h"
#include "../Enemy/BaseEnemy.h"
#pragma once
class TimeStop
{
private:
	bool active{};//if the ability is active
	f32 originalScale = 1.f;//the starting scale for the zone
	f32 zoneScale = 1.f;//the current scale of the zone
	f32 maxScale = 500.f;//the maximum size of the zie

	MeshManager* meshManager = nullptr;//mesh manager
	Player *player = nullptr;//player
	Sprite* zoneSprite = nullptr;//sprite of the zone

	std::string spriteName;
	Transform2D spriteTransform{};//transform the sprite

	Transform2D zoneTransform{};//transform for the zone
public:

	TimeStop();//time stop constructor
	~TimeStop();//destructor
	void Update(std::vector<BaseEnemy*> enemyList,float deltaTime);//update
	bool ToggleTime(AEVec2 position);//toggle the ability off & on 

	bool TimeStopped() const; //returns if the ablity is active
	f32 GetScale() const;//returns the scale of the ability
	void SetZonePos(AEVec2 position);//the position of zone
	void Draw();//renderings
};

