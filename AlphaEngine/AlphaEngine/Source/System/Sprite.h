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
	u32 spritesheetMaxFrames;
	f32 spriteUVWidth;
	f32 spriteUVHeight;
	std::string spritesheetFilename;

	//Animation info
	f32 animationTimer;
	f32 animation_duration_per_frame;
	u32 currentAnimationFrame;
	AEVec2 currentUVCoords;

	std::unordered_map<std::string, Animation> animationMap;

public:
	Sprite();
	Sprite(u32 spritesheetRows, u32 spritesheetColumns, u32 spritesheetMaxFrames, std::string spritesheetFilename);
	~Sprite();

	/*!
	@brief	Initialize sprite
	@param	u32 rows - number of rows in spritesheet
	@param	u32 columns - number of columns in spritesheet
	@param	u32 maxFrames - max number of frames in spritesheet
	@param	std::string name - name of spritesheet file
	@return void
	*/
	void Init(u32 rows, u32 columns, u32 maxFrames, std::string name);

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
};