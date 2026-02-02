/*!
@file       Sprite.h
@author     Yap Yu Xuan Daevon (yuxuandaevon.yap)
@brief      This file contains declarations of functions and variables
			used for the Sprite class
*//*_______________________________________________________________________*/

#pragma once

#include "AEEngine.h"
#include "Animation.h"
#include "../Player/Transform2D.h"
#include <unordered_map>
#include <string>

class Sprite
{
private:
	//Mesh and texture
	AEGfxVertexList* mesh;
	AEGfxTexture* tex;

	//Spritesheet info
	u32 spritesheetRows;
	u32 spritesheetColumns;
	f32 spriteUVWidth;
	f32 spriteUVHeight;
	std::string spriteName;
	std::string spritesheetFilename;

	//Animation info
	f32 animationTimer;
	f32 animationDurationPerFrame;
	u32 currentAnimationFrame;
	f32 currentUVWidth;
	f32 currentUVHeight;
	s32 playCount;

	std::string currentAnimationName;
	std::string defaultAnimationName;
	std::unordered_map<std::string, Animation*> animationMap;

public:
	/*!
	@brief	Constructor
	*/
	Sprite();

	/*!
	@brief	Overloaded constructor to initialize with sprite info
	@param	std::vector<std::string> spriteInfo - sprite info
	*/
	Sprite(std::vector<std::string> spriteInfo);

	/*!
	@brief	Destructor to deallocate all heap memory used
	*/
	~Sprite();

	/*!
	@brief	Initialize sprite
	@param	std::vector<std::string> spriteInfo - string vector storing sprite info
	@return void
	*/
	void Init();

	/*!
	@brief	Update sprite
	@param	void
	@return void
	*/
	void Update();

	/*!
	@brief	Render sprite
	@param	Transform2D& transform - transform matrix of the object
	@return void
	*/
	void Render(Transform2D& transform);

	/*!
	@brief	Add animation to the sprites animation map
	@param	std::string animName - name of animation
	@param	u32 startFrame - first frame of animation
	@param	u32 endFrame - last frame of animation
	@return void
	*/
	void AddAnimation(std::string animName, u32 startFrame, u32 endFrame);

	/*!
	@brief	Play a new animation
	@param	std::string animName - name of animation
	@param	s32 playCount - number of times to play
	@param	f32 animTime - duration of entire animation
	@return void
	*/
	void PlayAnimation(std::string animName, s32 count, f32 animTime);

	/*!
	@brief	Set a default animation to fall back on if current anim ends
	@param	std::string animName - name of animation
	@return void
	*/
	void SetDefaultAnimation(std::string animName);

	/*!
	@brief	Play default animation (calls PlayAnimation())
	@param	s32 playCount - number of times to play
	@param	f32 animTime - duration of entire animation
	@return void
	*/
	void PlayDefaultAnimation(s32 count = -1, f32 animTime = 1.0f);

	/*!
	@brief	Get the current animation
	@param	void
	@return Animation* - pointer to current animation
	*/
	Animation* GetCurrentAnimation() const;
};