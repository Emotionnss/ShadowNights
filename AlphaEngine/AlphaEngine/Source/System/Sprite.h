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

	void Update();
	void Render(Transform2D& transform);
};