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

enum TILE_TYPE
{
	NONE = 0,
	GROUND = 1,
	GRASS = 2,
	FLOWER = 3,
	MIDDLE_ROAD = 100,
	HORIZONTAL_LEFT_ROAD = 101,
	HORIZONTAL_MIDDLE_ROAD = 102,
	HORIZONTAL_RIGHT_ROAD = 103,
	VERTICAL_TOP_ROAD = 104,
	VERTICAL_MIDDLE_ROAD = 105,
	VERTICAL_BOTTOM_ROAD = 106,
	CORNER_TOP_LEFT_ROAD = 107,
	CORNER_TOP_RIGHT_ROAD = 108,
	CORNER_BOTTOM_LEFT_ROAD = 109,
	CORNER_BOTTOM_RIGHT_ROAD = 110,
	JUNCTION_MIDDLE_TOP_ROAD = 111,
	JUNCTION_MIDDLE_LEFT_ROAD = 112,
	JUNCTION_MIDDLE_RIGHT_ROAD = 113,
	JUNCTION_MIDDLE_BOTTOM_ROAD = 114,
	JUNCTION_MIDDLE_ROAD = 115,
	STARTPOINT = 201,
	ENDPOINT = 202,
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
	AEVec2 pos;
	TILE_TYPE tileType;
};

class Map : public SingletonTemplate<Map>
{
	friend class SingletonTemplate<Map>;

private:
	static const char* TILE_ASSETS_FILE_PATH;
	const float TILE_WIDTH = 50.f;
	const float TILE_HEIGHT = 50.f;
	const int NUM_COLUMN = 18;
	const int NUM_ROW = 26;

	Tile** map;

	std::vector<TILE_TYPE> tileTypesVector;
	AEVec2 spawnpoint;

	std::string getImageFilePath(TILE_TYPE tileType);
	void loadMapTileImages(void);

protected:
	Map();

public:

	Tile getTile(float xPos, float yPos);
	Tile getTileFromTilePos(float xPos, float yPos);
	AEVec2 getTilePos(float xPos, float yPos);
	AEVec2 getTileWorldPos(AEVec2 tilePos);
	void setTile(TILE_TYPE tile, float rowPos, float columnPos);

	Tile** getMap(void);
	int getNumCol(void) const;
	int getNumRow(void) const;

	bool loadMap(std::string fileName);
	bool saveMap(std::string fileName);

	void renderMap(void);

	void resetMap(bool clear);
	void clearMap(bool clear);

	void freeImages(bool free);

	AEVec2 getDimensions(void);
	float getTileSize(void) const;

	AEVec2 getSpawnpoint(void) const;
	void setSpawnpoint(AEVec2 spawnpoint);

	std::map<TILE_TYPE, AEGfxTexture*> imageMap;
};