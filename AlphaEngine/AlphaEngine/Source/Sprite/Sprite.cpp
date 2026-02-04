/*!
@file       Sprite.cpp
@author     Yap Yu Xuan Daevon (yuxuandaevon.yap)
@brief      This file contains definitions of functions and variables
			used for the Sprite class
*//*_______________________________________________________________________*/

#include <iostream>
#include "Sprite.h"
#include "../System/MeshBuilder.h"
#include "../System/CSVManager.h"

Sprite::Sprite() : 
	mesh{ nullptr },
	tex{ nullptr },
	spritesheetRows{ 0 },
	spritesheetColumns{ 0 },
	spriteUVWidth{ 0.0f },
	spriteUVHeight{ 0.0f },
	animationTimer{ 0.0f },
	currentAnimationFrame{ 0 },
	currentUVWidth{ 0.0f },
	currentUVHeight{ 0.0f },
	playCount{ 0 },
	spriteName{ "" },
	spritesheetFilename{""},
	animationDurationPerFrame{ 0.0f }
{
}

Sprite::Sprite(std::vector<std::string> spriteInfo) :
	mesh{ nullptr },
	tex{ nullptr },
	spritesheetRows{ std::stoul(spriteInfo[1]) },
	spritesheetColumns{ std::stoul(spriteInfo[2]) },
	spriteUVWidth{ 1.f / spritesheetColumns },
	spriteUVHeight{ 1.f / spritesheetRows },
	animationTimer{ 0.0f },
	currentAnimationFrame{ 0 },
	currentUVWidth{ 0.0f },
	currentUVHeight{ 0.0f },
	playCount{ 0 },
	spriteName{ spriteInfo[0] },
	spritesheetFilename{ "Assets/Spritesheets/" + spriteName + ".png" },
	animationDurationPerFrame{ 0.0f }
{
	mesh = MeshBuilder::CreateSpriteMesh(spriteUVWidth, spriteUVHeight);
	tex = AEGfxTextureLoad(spritesheetFilename.c_str());

	//Add animations
	std::vector<std::vector<std::string>> animInfo = CSVManager::ReadAnimationInfo(spriteInfo[0]);
	for (std::vector<std::string> info : animInfo)
	{
		AddAnimation(
			info[1],													  //Name
			std::stoul(info[2]) * spritesheetColumns,						  //Row index
			std::stoul(info[2]) * spritesheetColumns + std::stoul(info[3])); //Column index
	}
}

Sprite::~Sprite()
{
	AEGfxMeshFree(mesh);
	AEGfxTextureUnload(tex);

	for (std::pair<std::string, Animation*> animation : animationMap)
	{
		delete animation.second;
	}

	animationMap.clear();
}

void Sprite::Init()
{
	mesh = MeshBuilder::CreateSpriteMesh(spriteUVWidth, spriteUVHeight);
	tex = AEGfxTextureLoad(spritesheetFilename.c_str());

	//Add animations
	std::vector<std::vector<std::string>> animInfo = CSVManager::ReadAnimationInfo(spriteName);
	for (std::vector<std::string> info : animInfo)
	{
		AddAnimation(info[1], std::stoul(info[2]), std::stoul(info[3]));
	}
}

void Sprite::Update()
{
	animationTimer += (f32)AEFrameRateControllerGetFrameTime();

	//Get current animation
	Animation* currentAnim = GetCurrentAnimation();

	if (currentAnim->GetActive())
	{
		//Get the current frame
		currentAnimationFrame = currentAnim->GetFrame(static_cast<u32>(std::fmin(static_cast<u32>(currentAnim->GetFramesSize() - 1), static_cast<u32>(animationTimer / animationDurationPerFrame))));

		//Checks if animation reached end
		if (animationTimer >= currentAnim->GetAnimTime())
		{
			//Get repeat count of current animation
			s32 currentAnimRepeatCount = currentAnim->GetRepeatCount();

			//Set animation to beginning animation
			if (currentAnimRepeatCount == -1)
			{
				animationTimer = 0;
				currentAnimationFrame = currentAnim->GetFrame(0);
			}
			//Continue to play animation until play count goes over repeat count
			else if (playCount < currentAnimRepeatCount)
			{
				++playCount;
				animationTimer = 0;
				currentAnimationFrame = currentAnim->GetFrame(0);
			}
			else
			{
				currentAnim->SetActive(false);
			}
		}

		u32 current_sprite_row = currentAnimationFrame / spritesheetColumns;
		u32 current_sprite_col = currentAnimationFrame % spritesheetColumns;

		currentUVWidth = spriteUVWidth * current_sprite_col;
		currentUVHeight = spriteUVHeight * current_sprite_row;
	}
}

void Sprite::Render(Transform2D& transform)
{
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);

	AEGfxTextureSet(tex, currentUVWidth, currentUVHeight);
	AEGfxSetTransform(transform.GetMatrix().m);
	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
}

void Sprite::AddAnimation(std::string animName, u32 startFrame, u32 endFrame)
{
	//Only add animation if animation does not exist
	if (animationMap.find(animName) == animationMap.end())
	{
		Animation* animation = new Animation{ startFrame, endFrame };
		animationMap.insert({ animName, animation });

		//Set current & default animation if it doesnt exist
		if (currentAnimationName == "")
		{
			currentAnimationName = defaultAnimationName = animName;
			PlayDefaultAnimation();
		}
	}
}

void Sprite::PlayAnimation(std::string animName, s32 count, f32 animTime)
{
	//Only play animation if it exists
	if (animationMap.find(animName) != animationMap.end())
	{
		currentAnimationName = animName;

		//Set animation
		Animation* currentAnimation = GetCurrentAnimation();
		currentAnimation->SetAnimation(count, animTime);

		//Set animation duration per frame
		animationDurationPerFrame = currentAnimation->GetAnimTime() / currentAnimation->GetFramesSize();
		
		animationTimer = 0.0f;

		std::cout << "Playing new anim " << animName << std::endl;
	}
}

void Sprite::SetDefaultAnimation(std::string animName)
{
	defaultAnimationName = animName;
}

void Sprite::PlayDefaultAnimation(s32 count, f32 animTime)
{
	PlayAnimation(defaultAnimationName, count, animTime);
}

Animation* Sprite::GetCurrentAnimation() const
{
	return animationMap.find(currentAnimationName)->second;
}
