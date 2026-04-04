/*!
@file       Background.h
@author     Chng Wei Sheng Bennett (chng.w)
@brief      This file contains the declaration of the Background class used to render
            a scene background texture
*//*_______________________________________________________________________*/

#pragma once
#include "../GameObject/GameObject.h"
#include "../System/MeshManager.h"

class Background : public GameObject
{
public:
	/*!
	@brief	Constructs a Background and loads the texture at the given path.
	@param	const char* texturePath - path to the background texture 
	@return	void
	*//*______________________________________________________________*/
	Background(const char* texturePath = "Assets/Sprites/Background/orig_big.png");

	/*!
	@brief	Destructor; releases any allocated background resources.
	@param	void
	@return	void
	*//*______________________________________________________________*/
	~Background();

	Transform2D transform;
	AEGfxTexture* projTex;

	/*!
	@brief	Draws the background using the provided mesh.
	@param	AEGfxVertexList* mesh - mesh to use when rendering the background
	@return	void
	*//*______________________________________________________________*/
	void Draw(AEGfxVertexList* mesh);
};