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

	Player* player = nullptr;//player pointer
	f32 slowCost{}, stopCost, temporalCost;//costs for the abilities
	f32 maxResource{ 100.f };//the max amt of resource available to the player
	f32 resource{ 100.f };//the resource used for stopping time
	Transform2D resourceBarTfm{};//transform for the resource bar 

	MeshManager* meshManager = nullptr;//mesh manager
	AEVec2 camPos{};//position of camera for resource bar

	AEGfxTexture* a1Tex , *a2Tex , *a3Tex,*resourceBarOverlay,*barBG;//UI textures
	Transform2D a1Tfm{},a2Tfm,a3Tfm,barTfm,barBgTfm;//UI transforms
	bool timeStopped{};//stopped time ?
	bool timeSlowed{};//slowed time ? 

	bool anchored{};//temporally anchored? 

	bool lasered{};
	f32 a1Alpha{}, a2Alpha{}, a3Alpha{};//alpha for the icons

	AudioManager* audioManager{};//audio manager

	bool showUI{true};//show ui for cutscene in bossScene
public:
	/*!
	@brief    Default constructor
	*//*______________________________________________________________*/
	AbilityHandler();

	/*!
	@brief    Default destructor
	*//*______________________________________________________________*/
	~AbilityHandler();

	/*!
	@brief    Updates the object every frame
	@param	  The list of enemies to affect
	@param	  Time inbetween each frame
	@return	  void
	*//*______________________________________________________________*/
	void AbilityUpdate(std::vector<BaseEnemy*> enemylist,float deltaTime);

	/*!
	@brief    Renders the ability's visuals
	@param	  void
	@return   void
	*//*______________________________________________________________*/
	void Draw();

	/*!
	@brief    Returns if the Temporal Anchor ability is currently active
	@param    void
	@return   bool,true if active, else false
	*//*______________________________________________________________*/
	bool Anchored()const;

	/*!
	@brief    Returns if the Temporal Anchor ability is currently being activated . and the player is already reversing
	@param	  void
	@return	  bool, true if currently reversing, else false
	*//*______________________________________________________________*/
	bool Reverse() const;

	/*!
	@brief    Toggles the Temporal Anchor ability
	@param void
    @return void
	*//*______________________________________________________________*/
	void TriggerAnchor();


	/*!
	@brief    Determines if the UI regarding abilities should be shown.
	@param	  bool whether or not to be shown
	@return   void
	*//*______________________________________________________________*/
	void ShowUI(bool show);
};

