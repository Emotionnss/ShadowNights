/*!
@file       Animation.h
@author     Yap Yu Xuan Daevon (yuxuandaevon.yap)
@brief      This file contains declarations of functions and variables
			used for the Animation class
*//*_______________________________________________________________________*/

#pragma once

#include "AEEngine.h"
#include <vector>
#include <string>

class Animation
{
private:
	//Store all indexes of frames that comprise this animation
	std::vector<u32> frames;

	//-1 for repeated loop
	s32 repeatCount;

	//Time for entire animation to play
	f32 animTime;

	bool active;

public:
	/*!
	@brief	Constructor
	*/
	Animation();

	/*!
	@brief	Overloaded constructor to initialize with animation info
	@param	u32 startFrame - first frame of animation
	@param	u32 endFrame - last frame of animation
	*/
	Animation(u32 startFrame, u32 endFrame);

	/*!
	@brief	Destructor
	*/
	~Animation();

	/*!
	@brief	Add a frame into the frame vector
	@param	Frame to add
	*/
	void AddFrame(u32 frame);

	/*!
	@brief	Set animation values required for the animation to work, and set animation to active
	@param	s32 count - number of times to repeat. -1 for infinite loop
	@param	f32 time - total time for one instance of the animation to play
	*/
	void SetAnimation(s32 count, f32 time);

	/*!
	@brief	Get frame w.r.t animation frame vector
	@param	u32 frameNum - index for frame
	@return u32 - frame
	*/
	u32 GetFrame(u32 frameNum) const;

	/*!
	@brief	Get number of frames
	@param	void
	@return u32 - number of frames
	*/
	u32 GetFramesSize() const;

	/*!
	@brief	Get animation time
	@param	void
	@return f32 - animation time
	*/
	f32 GetAnimTime() const;

	/*!
	@brief	Set animation time
	@param	f32 time - the animation time
	@return void
	*/
	void SetAnimTime(f32 time);

	/*!
	@brief	Get repeat count
	@param	void
	@return s32 - repeat count. If its -1 its an infinite loop
	*/
	s32 GetRepeatCount() const;

	/*!
	@brief	Set repeat count
	@param	s32 count - number of times to repeat. -1 for infinite loop	
	@return void
	*/
	void SetRepeatCount(s32 count);

	/*!
	@brief	Get anim active status
	@param	void
	@return bool - active status
	*/
	bool GetActive() const;

	/*!
	@brief	Set anim active status
	@param	bool activeStatus - new active status
	@return void
	*/
	void SetActive(bool activeStatus);
};