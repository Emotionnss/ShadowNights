/*!
@file       Animation.cpp
@author     Yap Yu Xuan Daevon (yuxuandaevon.yap)
@brief      This file contains definitions of functions and variables
			used for the Animation class
*//*_______________________________________________________________________*/

#include "Animation.h"

Animation::Animation() :
	repeatCount{ 0 },
	animTime{ 0.0f },
	active{ false }
{
}

Animation::Animation(u32 startFrame, u32 endFrame) :
	repeatCount{ 0 },
	animTime{ 0.0f },
	active{ false }
{
	//Adds frames to the frame vector
	for (u32 i = startFrame; i < endFrame; i++)
	{
		AddFrame(i);
	}
}

Animation::~Animation()
{
}

void Animation::AddFrame(u32 frame)
{
	frames.push_back(frame);
}

void Animation::SetAnimation(s32 count, f32 time)
{
	SetRepeatCount(count);
	SetAnimTime(time);
	SetActive(true);
}

u32 Animation::GetFrame(u32 frameNum) const
{
	return frameNum > frames.size() ? static_cast<u32>(frames[0] + frames.size() - 1) : static_cast<u32>(frames[frameNum]);
}

u32 Animation::GetFramesSize() const
{
	return static_cast<u32>(frames.size());
}

f32 Animation::GetAnimTime() const
{
	return animTime;
}

void Animation::SetAnimTime(f32 time)
{
	animTime = time;
}

s32 Animation::GetRepeatCount() const
{
	return repeatCount;
}

void Animation::SetRepeatCount(s32 count)
{
	repeatCount = count;
}

bool Animation::GetActive() const
{
	return active;
}

void Animation::SetActive(bool activeStatus)
{
	active = activeStatus;
}
