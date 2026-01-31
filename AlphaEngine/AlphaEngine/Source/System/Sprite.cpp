/*!
@file       Sprite.cpp
@author     Yap Yu Xuan Daevon (yuxuandaevon.yap)
@brief      This file contains definitions of functions and variables
			used for the Sprite class
*//*_______________________________________________________________________*/

#include "Sprite.h"
#include "MeshBuilder.h"

Sprite::Sprite() : 
	mesh{ nullptr },
	tex{ nullptr },
	spritesheetRows{ 0 },
	spritesheetColumns{ 0 },
	spritesheetMaxFrames{ 0 },
	spriteUVWidth{ 0.0f },
	spriteUVHeight{ 0.0f },
	animationTimer{ 0.0f },
	currentAnimationFrame{ 0 },
	currentUVCoords{ 0.0f },
	spritesheetFilename{""},
	animation_duration_per_frame{ 0.0f }
{
}

Sprite::Sprite(u32 spritesheetRows, u32 spritesheetColumns, u32 spritesheetMaxFrames, std::string name) :
	mesh{ nullptr },
	tex{ nullptr },
	spritesheetRows{ spritesheetRows },
	spritesheetColumns{ spritesheetColumns },
	spritesheetMaxFrames{ spritesheetMaxFrames },
	spriteUVWidth{ 1.f / spritesheetColumns },
	spriteUVHeight{ 1.f / spritesheetRows },
	animationTimer{ 0.0f },
	currentAnimationFrame{ 0 },
	currentUVCoords{ 0.0f },
	spritesheetFilename{ "Assets/Spritesheets/" + name },
	animation_duration_per_frame{ 0.1f }
{
	mesh = MeshBuilder::CreateSpriteMesh(spriteUVWidth, spriteUVHeight);
	tex = AEGfxTextureLoad(spritesheetFilename.c_str());
	animation_duration_per_frame = 0.1f;
}

Sprite::~Sprite()
{
	AEGfxMeshFree(mesh);
	AEGfxTextureUnload(tex);
}

void Sprite::Init(std::vector<std::string> spriteInfo)
{
	spritesheetRows = std::stoul(spriteInfo[1]);
	spritesheetColumns = std::stoul(spriteInfo[2]);
	spritesheetMaxFrames = std::stoul(spriteInfo[3]);
	spriteUVWidth = 1.f / spritesheetColumns;
	spriteUVHeight = 1.f / spritesheetRows;

	spritesheetFilename = "Assets/Spritesheets/" + spriteInfo[0] + ".png";
	mesh = MeshBuilder::CreateSpriteMesh(spriteUVWidth, spriteUVHeight);
	tex = AEGfxTextureLoad(spritesheetFilename.c_str());
	animation_duration_per_frame = 0.1f;
}

void Sprite::Update()
{
	animationTimer += (f32)AEFrameRateControllerGetFrameTime();

	if (animationTimer >= animation_duration_per_frame) {
		// When the time is up go to the next sprite.

		// Reset the timer.
		animationTimer = 0;

		// Calculate the next sprite UV
		currentAnimationFrame = ++currentAnimationFrame % spritesheetMaxFrames;

		u32 current_sprite_row = currentAnimationFrame / spritesheetColumns;
		u32 current_sprite_col = currentAnimationFrame % spritesheetColumns;

		currentUVCoords.x = spriteUVWidth * current_sprite_col;
		currentUVCoords.y = spriteUVHeight * current_sprite_row;

	}
}

void Sprite::Render(Transform2D& transform)
{
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);

	AEGfxTextureSet(tex, currentUVCoords.x, currentUVCoords.y);
	AEGfxSetTransform(transform.GetMatrix().m);
	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
}
