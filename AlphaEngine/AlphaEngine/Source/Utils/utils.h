/*!
@file       utils.h
@author     Yap Yu Xuan Daevon (yuxuandaevon.yap)
@brief      This file contains declaration of functions that help with various calculations such as collision detection and coordinate conversions.
*//*_______________________________________________________________________*/

#pragma once

/*!
	@brief	Checks if a click at (click_x, click_y) is within the rectangular area defined by its center (area_center_x, area_center_y) and dimensions (area_width, area_height).
	@param	area_center_x The x-coordinate of the area's center.
	@param	area_center_y The y-coordinate of the area's center.
	@param	area_width The width of the area.
	@param	area_height The height of the area.
	@param	click_x The x-coordinate of the click.
	@param	click_y The y-coordinate of the click.
	@return True if the click is within the area, false otherwise.
	*//*______________________________________________________________*/
bool IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);

/*!
	@brief	Checks if a click at (click_x, click_y) is within the circular area defined by its center (circle_center_x, circle_center_y) and diameter.
	@param	circle_center_x The x-coordinate of the circle's center.
	@param	circle_center_y The y-coordinate of the circle's center.
	@param	diameter The diameter of the circle.
	@param	click_x The x-coordinate of the click.
	@param	click_y The y-coordinate of the click.
	@return True if the click is within the circle, false otherwise.
	*//*______________________________________________________________*/
bool IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y);


/*!
	@brief	Checks if two circles defined by their centers (c1_x, c1_y) and (c2_x, c2_y) and their respective radii (r1 and r2) are intersecting.
	@param	c1_x The x-coordinate of the first circle's center.
	@param	c1_y The y-coordinate of the first circle's center.
	@param	r1 The radius of the first circle.
	@param	c2_x The x-coordinate of the second circle's center.
	@param	c2_y The y-coordinate of the second circle's center.
	@param	r2 The radius of the second circle.
	@return True if the circles are intersecting, false otherwise.
	*//*______________________________________________________________*/
bool AreCirclesIntersecting(float c1_x, float c1_y, float r1, float c2_x, float c2_y, float r2);

/*!
	@brief	Checks if two axis-aligned bounding boxes (AABBs) defined by their centers (aPosition and bPosition) and their half-extents (aHalf and bHalf) are overlapping.
	@param	aPosition The center position of the first AABB.
	@param	aHalf The half-extents (half-width and half-height) of the first AABB.
	@param	bPosition The center position of the second AABB.
	@param	bHalf The half-extents (half-width and half-height) of the second AABB.
	@return True if the AABBs are overlapping, false otherwise.
	*//*______________________________________________________________*/
bool AABBOverlap(AEVec2 aPosition, AEVec2 aHalf, AEVec2 bPosition, AEVec2 bHalf);

/*!
	@brief	Converts a position from screen coordinates (e.g., mouse position) to world coordinates based on the current camera view and projection settings.
	@param	MouseScreenWorldPos The position in screen coordinates to be converted.

	*//*______________________________________________________________*/
AEVec2 ScreenToWorldPos(AEVec2 MouseScreenWorldPos);
