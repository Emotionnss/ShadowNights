/*!
@file       Map.cpp
@author     Yap Yu Xuan Daevon (yuxuandaevon.yap)
@brief      This file contains definitions of functions	
			used for the Map class
*//*_______________________________________________________________________*/

#include "Map.h"
#include "CSVManager.h"
#include <cmath>
#include <iostream>

const char* Map::TILE_ASSETS_FILE_PATH = "Assets/Tiles/";

/*!
@brief Constructor. Makes it set the map array to empty and loads all map tile images
*//*______________________________________________________________*/
Map::Map()
{
	resetMap(false);
	loadMapTileImages();
}

/*!
@brief	Gets the image file path based on the tileType of the tile
@param	TILE_TYPE tileType - type of the tile based off the enum TILE_TYPE
@return string - filePath of the file to get
*//*______________________________________________________________*/
std::string Map::getImageFilePath(TILE_TYPE tileType)
{
	std::string filePath = TILE_ASSETS_FILE_PATH;

	switch (tileType)
	{
	case NONE:
		filePath += "None.png";
		break;
	default:
		break;
	}

	return filePath;
}

/*!
@brief	Load all the map tile images and inserts them into the imageMap
@param	void
@return void
*//*______________________________________________________________*/
void Map::loadMapTileImages(void)
{
	tileTypesVector = {};

	for (int i = 0; i < tileTypesVector.size(); i++)
	{
		//CP_Image tempImage = CP_Image_Load(getImageFilePath(tileTypesVector[i]).c_str());
		//imageMap.insert({ tileTypesVector[i], tempImage });
	}
}

/*!
@brief	Gets a tile based on the x & y postion given
@param	xPos - x position
@param	yPos - y position
@return Tile
*//*______________________________________________________________*/
Tile Map::getTile(float xPos, float yPos)
{
	AEVec2 tilePos = getTilePos(xPos, yPos);

	Tile tile = map[(int)tilePos.x][(int)tilePos.y];

	return tile;
}

/*!
@brief	Gets a tile based on the x & y tile pos given
@param	xPos - x tile position
@param	yPos - y tile position
@return Tile
*//*______________________________________________________________*/
Tile Map::getTileFromTilePos(float xPos, float yPos)
{
	Tile tile = map[(int)xPos][(int)yPos];

	return tile;
}

/*!
@brief	Gets tile grid pos based on x & y position given
@param	xPos - x position
@param	yPos - y position
@return CP_Vector - tile grid pos
*//*______________________________________________________________*/
AEVec2 Map::getTilePos(float xPos, float yPos)
{
	int rowPos = 0, columnPos = 0;

	rowPos = (int)floor(xPos / TILE_WIDTH);
	columnPos = (int)floor(yPos / TILE_HEIGHT);

	if (rowPos >= NUM_ROW)
	{
		rowPos = NUM_ROW - 1;
	}
	else if (rowPos < 0)
	{
		rowPos = 0;
	}

	if (columnPos >= NUM_COLUMN)
	{
		columnPos = NUM_COLUMN - 1;
	}
	else if (columnPos < 0)
	{
		columnPos = 0;
	}

	AEVec2 vec = { rowPos, columnPos };

	return vec;
}

/*!
@brief	Gets tile world pos based on tile grid pos
@param	CP_Vector tilePos - tile grid pos
@return CP_Vector - tile world pos
*//*______________________________________________________________*/
AEVec2 Map::getTileWorldPos(AEVec2 tilePos)
{
	Tile tile = map[(int)tilePos.x][(int)tilePos.y];

	return tile.pos;
}

/*!
@brief	Set tile type based on given grid position
@param	rowPos - tile grid row pos
@param	columnPos - tile grid column pos
@return void
*//*______________________________________________________________*/
void Map::setTile(TILE_TYPE tile, float rowPos, float columnPos)
{
	map[(int)rowPos][(int)columnPos].tileType = tile;
}

/*!
@brief	Get the map
@param	void
@return Tile** - map
*//*______________________________________________________________*/
Tile** Map::getMap(void)
{
	return this->map;
}

/*!
@brief	Get number of columns
@param	void
@return int - number of columns
*//*______________________________________________________________*/
int Map::getNumCol(void) const
{
	return NUM_COLUMN;
}

/*!
@brief	Get number of rows
@param	void
@return int - number of rows
*//*______________________________________________________________*/
int Map::getNumRow(void) const
{
	return NUM_ROW;
}

/*!
@brief	Loads the map using CSVManager based off the fileName,
		stores this into in the map array
@param	string fileName - name of the file
@return bool
*//*______________________________________________________________*/
bool Map::loadMap(std::string fileName)
{
	std::string file;

	file = fileName;

	//Get the map data from reading the file
	std::vector<std::vector<int>> mapData = CSVManager::read(file, NUM_COLUMN, NUM_ROW);

	//Stop if empty
	if (mapData.empty())
	{
		return false;
	}

	//Reset the map
	resetMap(true);

	//Nested for loop to store the tile data read from file
	for (int i = 0; i < NUM_ROW; i++)
	{
		for (int j = 0; j < NUM_COLUMN; j++)
		{
			//Save the data into the map array from the mapData recieved from reading the csv file.
			//mapData row and column must be reversed for it to show properly
			map[i][j].tileType = static_cast<TILE_TYPE>(mapData[j][i]);
			AEVec2 vec = { (TILE_WIDTH * 0.5f) + TILE_WIDTH * i, (TILE_HEIGHT * 0.5f) + TILE_HEIGHT * j };
			map[i][j].pos = vec;

			/*if (map[i][j].tileType == STARTPOINT)
			{
				spawnpoint = map[i][j].pos;
			}*/
		}
	}

	return true;
}

/*!
@brief	Saves the map by using CSVManager to write to csv
@param	string fileName - name of the file
@return bool
*//*______________________________________________________________*/
bool Map::saveMap(std::string fileName)
{
	std::vector<std::vector<int>> mapData;

	//mapData row and column must be reversed for it to save properly
	for (int i = 0; i < NUM_COLUMN; i++)
	{
		std::vector<int> columnData;

		for (int j = 0; j < NUM_ROW; j++)
		{
			columnData.push_back(map[j][i].tileType);
		}

		mapData.push_back(columnData);
	}

	if (CSVManager::write(fileName, NUM_ROW, NUM_COLUMN, mapData))
	{
		return true;
	}

	return false;
}

/*!
@brief	Render the map by using the data from the map array
@param	void
@return void
*//*______________________________________________________________*/
void Map::renderMap(void)
{
	//Render the map
	for (int i = 0; i < NUM_ROW; i++)
	{
		for (int j = 0; j < NUM_COLUMN; j++)
		{
			//CP_Image_Draw(imageMap[map[i][j].tileType], map[i][j].pos.x, map[i][j].pos.y, TILE_WIDTH, TILE_HEIGHT, 255);
		}
	}
}

/*!
@brief	Resets the map by clearing it then initialising it with
		empty values
@param	bool clear - whether to clear the map or not (used when first initialising the map)
@return void
*//*______________________________________________________________*/
void Map::resetMap(bool clear)
{
	clearMap(clear);

	//Resets the map with new blank data
	map = new Tile * [NUM_ROW];

	for (int i = 0; i < NUM_ROW; i++)
	{
		map[i] = new Tile[NUM_COLUMN];

		for (int j = 0; j < NUM_COLUMN; j++)
		{
			map[i][j].tileType = NONE;
			map[i][j].pos = { 0 };
		}
	}
}

/*!
@brief	Clears the map to prevent memory leak
@param	bool clear - whether to clear the map or not (used when first initialising the map)
@return void
*//*______________________________________________________________*/
void Map::clearMap(bool clear)
{
	if (clear)
	{
		for (int i = 0; i < NUM_ROW; i++)
		{
			delete map[i];
		}

		delete map;
	}
}

/*!
@brief	Frees all the images and clears the vectors (used when program is shutting down)
@param	void
@return void
*//*______________________________________________________________*/
void Map::freeImages(bool free)
{
	if (free)
	{
		/*for (int i = 0; i < tileTypesVector.size(); i++)
		{
			CP_Image_Free(imageMap[tileTypesVector[i]]);
		}

		imageMap.clear();*/
		tileTypesVector.clear();
	}
}

/*!
@brief	Get dimensions of map in CP_Vector form
@param	void
@return CP_Vector - map dimensions
*//*______________________________________________________________*/
AEVec2 Map::getDimensions(void)
{
	AEVec2 vec = { static_cast<float>(NUM_ROW), static_cast<float>(NUM_COLUMN) };
	return vec;
}

/*!
@brief	Get size of tile
@param	void
@return float - size of tile
*//*______________________________________________________________*/
float Map::getTileSize(void) const
{
	return TILE_WIDTH;
}

/*!
@brief	Get spawnpoint of the enemies
@param	void
@return CP_Vector - vector of spawnpoint
*//*______________________________________________________________*/
AEVec2 Map::getSpawnpoint(void) const
{
	return spawnpoint;
}


/*!
@brief	Set spawnpoint of the enemies
@param	CP_Vector - vector of spawnpoint
@return void
*//*______________________________________________________________*/
void Map::setSpawnpoint(AEVec2 spawnpoint)
{
	this->spawnpoint = spawnpoint;
}

