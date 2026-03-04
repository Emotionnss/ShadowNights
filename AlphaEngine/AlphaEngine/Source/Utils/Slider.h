/*!
@file       Slider.h
@author     Yap Yu Xuan Daevon (yuxuandaevon.yap)
@brief      This file contains declaration of functions and variables
			used for the slider class
*//*_______________________________________________________________________*/

#pragma once
#include "AEEngine.h"
#include "../System/MeshManager.h"
#include "../Player/Transform2D.h"

class Slider
{
private:
	MeshManager* meshManager;

	AEGfxTexture* thumb;
	AEGfxTexture* tracker;

	Transform2D trackerTransform;
	Transform2D thumbTransform;

	//This is the value that will be changed by the slider. e.g volume in settings
	float sliderValue;

public:
	/*!
	@brief	Constructor
	*//*______________________________________________________________*/
	Slider();

	/*!
	@brief	Overloaded constructor
	@param	AEVec2 position - position of slider
	@param	float value - value of slider
	*//*______________________________________________________________*/
	Slider(AEVec2 position, float value);

	/*!
	@brief	Destuctor
	*//*______________________________________________________________*/
	~Slider();

	/*!
	@brief	Loads the images used for the slider
	@param	void
	@return void
	*//*______________________________________________________________*/
	void LoadImages(void);

	/*!
	@brief	Updates the thumb position based off a new position (used when dragging the thumb with mouse)
	@param	AEVec2 newPosition - the new position to set the thumb to
	@return void
	*//*______________________________________________________________*/
	void UpdateThumbPositionBasedOnPosition(AEVec2 newPosition);

	/*!
	@brief	Updates the thumb position based off the value attached to the slider (used when initializing slider)
	@param	void
	@return void
	*//*______________________________________________________________*/
	void UpdateThumbPositionBasedOnValue(void);

	/*!
	@brief	Clamp thumb position to the boundaries of the tracker
	@param	void
	@return void
	*//*______________________________________________________________*/
	void ClampThumbPosition(void);

	/*!
	@brief	Get value attached to the slider
	@param	void
	@return float - value attached to the slider
	*//*______________________________________________________________*/
	float GetSliderValue(void) const;

	/*!
	@brief	Get thumb Transform2D
	@param	void
	@return Transform2D - thumb Transform2D
	*//*______________________________________________________________*/
	Transform2D GetThumbTransform(void) const;

	/*!
	@brief	Get tracker Transform2D
	@param	void
	@return Transform2D - tracker Transform2D
	*//*______________________________________________________________*/
	Transform2D GetTrackerTransform(void) const;

	/*!
	@brief	Render the slider
	@param	void
	@return void
	*//*______________________________________________________________*/
	void RenderSlider();
};