/*!
@file       TimeStop.h
@author     Chew Zavier (c.zavier)
@brief      This file contains declarations of functions used by the Time Stop ability which is a
			player ability that allows the user to stop time in an area around the activated position
*//*_______________________________________________________________________*/
#include "../System/MeshManager.h"
#include "../GameObject/Transform2D.h"
#include "../Player/Player.h"
#include "../Sprite/Sprite.h"
#include "../Enemy/BaseEnemy.h"
#pragma once
class TimeStop
{
private:
	bool active{};//if the ability is active
	bool despawning{};
	bool tempStop{};//for if the player is reversing back to temporal anchor
	f32 originalScale = 1.f;//the starting scale for the zone
	f32 zoneScale = 1.f;//the current scale of the zone
	f32 maxScale = 500.f;//the maximum size of the zie

	f32 despawnTime{ 0.7f }, despawnTimer{ despawnTime };
	MeshManager* meshManager = nullptr;//mesh manager
	Player *player = nullptr;//player

	Transform2D centerTransform{};//transform the center

	Transform2D zoneTransform{};//transform for the zone for the time stopping functionality
	Transform2D zoneSpriteTfm{};//transform for the zones sprite ONLY as they are of different sizes
	Sprite* zoneSprite = nullptr;//sprite of the zone
	Sprite* zoneCenterSprite = nullptr;
	Sprite* centerDespawn = nullptr;
public:
	/*!
	@brief    Default constructor
	*//*______________________________________________________________*/
	TimeStop();

	/*!
	@brief    Default destructor
	*//*______________________________________________________________*/
	~TimeStop();

	/*!
	@brief    Updates the object every frame
	@param	  The list of enemies to affect
	@param	  Time inbetween each frame
	@return   void
	*//*______________________________________________________________*/
	void Update(std::vector<BaseEnemy*> enemyList,float deltaTime);

	/*!
	@brief    Toggles the abilitys activity
	@param    The position of the center of the Ability
	@return   bool, true if active, false if not
	*//*______________________________________________________________*/
	bool ToggleTime(AEVec2 position);

	/*!
	@brief    Temporary stop in time, not reliant on the actual ability
	@param    the boolean to set the activity to
	@return   void
	*//*______________________________________________________________*/
	void TempStop(bool active);

	/*!
	@brief    Returns if the ability is currently active
	@param    void
	@return	  bool,true if active, false if not
	*//*______________________________________________________________*/
	bool TimeStopped() const;

	/*!
	@brief    Returns the scale of the zone of the ability
	@param    void
	@return   f32, the scale of the zone
	*//*______________________________________________________________*/
	f32 GetScale() const;
	/*!
	@brief    Sets the position of the zone
	@param    The position to set the center of the zone to
	@return   void
	*//*______________________________________________________________*/
	void SetZonePos(AEVec2 position);

	/*!
	@brief    Renders the ability's visuals
	@param void
	@return void
	*//*______________________________________________________________*/
	void Draw();
};

