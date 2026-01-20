/*!
@file       Map.h
@author     Yap Yu Xuan Daevon (yuxuandaevon.yap)
@brief      This file contains declaration of functions and variables
			used for the Map class
*//*_______________________________________________________________________*/

#pragma once
#include "SingletonTemplate.h"
#include "../../AlphaEngine/include/AEEngine.h"
#include <string>
#include <map>
#include <vector>
#include "../System/MeshManager.h"
#include "../Player/Transform2D.h"

enum TILE_TYPE
{
	NONE = 0,
	BACKGROUND,
	FLOOR = 101,
	CEILING = 102,
	LEFT_WALL = 103,
	RIGHT_WALL = 104,
	NW_CORNER = 111,
	NE_CORNER = 112,
	SW_CORNER = 113,
	SE_CORNER = 114,
	LEFT_PLATFORM = 120,
	PLATFORM = 121,
	RIGHT_PLATFORM = 122,
	LEFT_STAIR = 150,
	RIGHT_STAIR = 151,
	TOTAL
};

enum TILE_RELATIVE_POSITION
{
	TOP_MIDDLE,
	MIDDLE_LEFT,
	MIDDLE_RIGHT,
	BOTTOM_MIDDLE,
	TILE_RELATIVE_TOTAL
};

struct Tile
{
	Transform2D transform;
	TILE_TYPE tileType;
};

class Map : public SingletonTemplate<Map>
{
	friend class SingletonTemplate<Map>;

private:
	static const char* TILE_ASSETS_FILE_PATH;
	const float TILE_WIDTH = 75.f;
	const float TILE_HEIGHT = 75.f;
	int NUM_COLUMN;
	int NUM_ROW;

	MeshManager* meshManager;

	Tile** map;

	std::vector<TILE_TYPE> tileTypesVector;
	AEVec2 spawnpoint;

	std::map<TILE_TYPE, AEGfxTexture*> imageMap;

	std::string GetImageFilePath(TILE_TYPE tileType);
	void LoadMapTileImages(void);

protected:
	Map();
	~Map();

public:
	Tile GetTile(float xPos, float yPos) const;
	Tile GetTileFromTilePos(float xPos, float yPos) const;
	AEVec2 GetTilePos(float xPos, float yPos) const;
	AEVec2 GetTileWorldPos(AEVec2 tilePos) const;
	void SetTile(TILE_TYPE tile, float rowPos, float columnPos);

	Tile** GetMap(void) const;
	int GetNumCol(void) const;
	int GetNumRow(void) const;

	bool LoadMap(std::string fileName);
	bool SaveMap(std::string fileName);

	void RenderMap(void);

	void ResetMap(bool clear);
	void ClearMap(bool clear);

	void FreeImages();

	AEVec2 GetDimensions(void) const;
	float GetTileSize(void) const;

	AEVec2 GetSpawnpoint(void) const;
	void SetSpawnpoint(AEVec2 spawnpoint);
};