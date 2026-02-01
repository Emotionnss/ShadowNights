/*!
@file       MeshBuilder.h
@author     Yap Yu Xuan Daevon (yuxuandaevon.yap)
@brief      This file contains declarations of static functions
			used for the MeshBuilder class, which are used to create
			meshes of different types
*//*_______________________________________________________________________*/

#pragma once

#include "AEEngine.h"

class MeshBuilder
{
public:
	/*!
	@brief	Create a square mesh
	@param	bool texture - true to create mesh that has uv coords, false to create a mesh without uv coords
	@return AEGfxVertexList* - vertex list of the mesh that has been created
	*/
	static AEGfxVertexList* CreateSquareMesh(bool texture = false);

	/*!
	@brief	Create a triangle mesh
	@param	void
	@return AEGfxVertexList* - vertex list of the mesh that has been created
	*/
	static AEGfxVertexList* CreateTriangle();

	/*!
	@brief	Create a circle mesh
	@param	void
	@return AEGfxVertexList* - vertex list of the mesh that has been created
	*/
	static AEGfxVertexList* CreateCircle();

	/*!
	@brief	Create a line mesh
	@param	void
	@return AEGfxVertexList* - vertex list of the mesh that has been created
	*/
	static AEGfxVertexList* CreateLine();

	/*!
	@brief	Create a sprite square mesh
	@param	f32 spriteUVWidth - uv width (u value) for each frame
	@param	f32 spriteUVHeight - uv height (v value) for each frame
	@return AEGfxVertexList* - vertex list of the mesh that has been created
	*/
	static AEGfxVertexList* CreateSpriteMesh(f32 spriteUVWidth, f32 spriteUVHeight);
};
