/*!
@file       Slider.cpp
@author     Yap Yu Xuan Daevon (yuxuandaevon.yap)
@brief      This file contains definition of functions and variables
			used for the slider class
*//*_______________________________________________________________________*/

#include "Slider.h"

Slider::Slider() :
	sliderValue{},
	meshManager{MeshManager::GetInstance()}
{
	LoadImages();
	UpdateThumbPositionBasedOnValue();
	trackerTransform.SetPosition({});
	trackerTransform.SetScale(400.f, 25.f);
	thumbTransform.SetPosition({});
	thumbTransform.SetScale(40.f, 50.f);
}

Slider::Slider(AEVec2 position, float value) :
	sliderValue{value},
	meshManager{ MeshManager::GetInstance() }
{
	LoadImages();
	UpdateThumbPositionBasedOnValue();
	trackerTransform.SetPosition(position);
	trackerTransform.SetScale(400.f, 25.f);
	thumbTransform.SetPosition(position);
	thumbTransform.SetScale(40.f, 50.f);
}

Slider::~Slider()
{
	AEGfxTextureUnload(thumb);
}

void Slider::LoadImages(void)
{
	thumb = AEGfxTextureLoad("Assets/Slider/thumb.png");
}

void Slider::UpdateThumbPositionBasedOnPosition(AEVec2 newPosition)
{
	AEVec2 thumbPosition = newPosition;
	float trackerWidth = trackerTransform.GetScale().x;

	ClampThumbPosition();

	sliderValue = (((thumbPosition.x - trackerWidth / 2) -  trackerWidth) / trackerWidth) * 100;
}

void Slider::UpdateThumbPositionBasedOnValue(void)
{
	float newXPos = trackerTransform.GetPosition().x - trackerTransform.GetScale().x / 2;

	newXPos += trackerTransform.GetScale().x * (sliderValue / 100);
	trackerTransform.SetPosition({newXPos, thumbTransform.GetPosition().y});
}

void Slider::ClampThumbPosition(void)
{
	float newPos = AEClamp(thumbTransform.GetPosition().x, trackerTransform.GetPosition().x - trackerTransform.GetScale().x / 2, trackerTransform.GetPosition().x + trackerTransform.GetScale().x / 2);

	thumbTransform.SetPosition(newPos, trackerTransform.GetPosition().y);
}

float Slider::GetSliderValue(void) const
{
	return sliderValue;
}

Transform2D Slider::GetThumbTransform(void) const
{
	return thumbTransform;
}

Transform2D Slider::GetTrackerTransform(void) const
{
	return trackerTransform;
}

void Slider::RenderSlider()
{
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	AEGfxVertexList* texSquareMesh = meshManager->GetMesh(TEXTURE_SQUARE);

	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);

	//Thumb render
	AEGfxTextureSet(thumb, 0, 0);
	AEGfxSetTransform(thumbTransform.GetMatrix().m);
	AEGfxMeshDraw(texSquareMesh, AE_GFX_MDM_TRIANGLES);

	//Tracker render
	AEGfxTextureSet(tracker, 0, 0);
	AEGfxSetTransform(trackerTransform.GetMatrix().m);
	AEGfxMeshDraw(texSquareMesh, AE_GFX_MDM_TRIANGLES);
}

