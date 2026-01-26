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
	LoadMapTileImages();

	LoadMap("TestMap");

	meshManager = MeshManager::GetInstance();
}

Map::~Map()
{
	FreeImages();
	ClearMap(true);
}

/*!
@brief	Gets the image file path based on the tileType of the tile
@param	TILE_TYPE tileType - type of the tile based off the enum TILE_TYPE
@return string - filePath of the file to get
*//*______________________________________________________________*/
std::string Map::GetImageFilePath(TILE_TYPE tileType)
{
	std::string filePath = TILE_ASSETS_FILE_PATH;

	switch (tileType)
	{
	case BACKGROUND:
		filePath += "Background.png";
		break;
	case FLOOR:
		filePath += "Floor.png";
		break;
	case CEILING:
		filePath += "Ceiling.png";
		break;
	case LEFT_WALL:
		filePath += "Left_Wall.png";
		break;
	case RIGHT_WALL:
		filePath += "Right_Wall.png";
		break;
	case NW_CORNER:
		filePath += "NW_Corner.png";
		break;
	case NE_CORNER:
		filePath += "NE_Corner.png";
		break;
	case SW_CORNER:
		filePath += "SW_Corner.png";
		break;
	case SE_CORNER:
		filePath += "SE_Corner.png";
		break;
	case LEFT_PLATFORM:
		filePath += "Left_Platform.png";
		break;
	case PLATFORM:
		filePath += "Platform.png";
		break;
	case RIGHT_PLATFORM:
		filePath += "Right_Platform.png";
		break;
	case LEFT_STAIR:
		filePath += "Left_Stair.png";
		break;
	case RIGHT_STAIR:
		filePath += "Right_Stair.png";
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
void Map::LoadMapTileImages(void)
{
	tileTypesVector = { 
		BACKGROUND,
		FLOOR,
		CEILING,
		LEFT_WALL,
		RIGHT_WALL,
		NW_CORNER,
		NE_CORNER,
		SW_CORNER,
		SE_CORNER,
		LEFT_PLATFORM,
		PLATFORM,
		RIGHT_PLATFORM,
		LEFT_STAIR,
		RIGHT_STAIR 
	};

	for (int i = 0; i < tileTypesVector.size(); i++)
	{
		AEGfxTexture* pTex = AEGfxTextureLoad(GetImageFilePath(tileTypesVector[i]).c_str());
		imageMap.insert({ tileTypesVector[i], pTex });
	}
}

/*!
@brief	Gets a tile based on the x & y postion given
@param	xPos - x position
@param	yPos - y position
@return Tile
*//*______________________________________________________________*/
Tile Map::GetTile(float xPos, float yPos) const
{
	AEVec2 tilePos = GetTilePos(xPos, yPos);

	Tile tile = map[(int)tilePos.x][(int)tilePos.y];

	return tile;
}

/*!
@brief	Gets a tile based on the x & y tile pos given
@param	xPos - x tile position
@param	yPos - y tile position
@return Tile
*//*______________________________________________________________*/
Tile& Map::GetTileFromTilePos(float xPos, float yPos) const
{
	Tile& tile = map[(int)xPos][(int)yPos];

	return tile;
}

/*!
@brief	Gets tile grid pos based on x & y position given
@param	xPos - x position
@param	yPos - y position
@return CP_Vector - tile grid pos
*//*______________________________________________________________*/
AEVec2 Map::GetTilePos(float xPos, float yPos) const
{
	int rowPos = 0, columnPos = 0;

	rowPos = (int)floor(xPos / TILE_WIDTH);
	columnPos = NUM_ROW - (int)floor(yPos / TILE_HEIGHT);

	/*if (rowPos >= NUM_ROW)
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
	}*/

	AEVec2 vec = { static_cast<f32>(rowPos), static_cast<f32>(columnPos) };

	return vec;
}

/*!
@brief	Gets tile world pos based on tile grid pos
@param	CP_Vector tilePos - tile grid pos
@return CP_Vector - tile world pos
*//*______________________________________________________________*/
AEVec2 Map::GetTileWorldPos(AEVec2 tilePos) const
{
	Tile tile = map[(int)tilePos.x][(int)tilePos.y];

	return tile.transform.GetPosition();
}

/*!
@brief	Set tile type based on given grid position
@param	rowPos - tile grid row pos
@param	columnPos - tile grid column pos
@return void
*//*______________________________________________________________*/
void Map::SetTile(TILE_TYPE tile, float rowPos, float columnPos)
{
	map[(int)rowPos][(int)columnPos].tileType = tile;
}

/*!
@brief	Get the map
@param	void
@return Tile** - map
*//*______________________________________________________________*/
Tile** Map::GetMap(void) const
{
	return this->map;
}

/*!
@brief	Get number of columns
@param	void
@return int - number of columns
*//*______________________________________________________________*/
int Map::GetNumCol(void) const
{
	return NUM_COLUMN;
}

/*!
@brief	Get number of rows
@param	void
@return int - number of rows
*//*______________________________________________________________*/
int Map::GetNumRow(void) const
{
	return NUM_ROW;
}

/*!
@brief	Loads the map using CSVManager based off the fileName,
		stores this into in the map array
@param	string fileName - name of the file
@return bool
*//*______________________________________________________________*/
bool Map::LoadMap(std::string fileName)
{
	std::string file;

	file = fileName;

	//Get the map data from reading the file
	std::vector<std::vector<int>> mapData = CSVManager::read(file);

	//Stop if empty
	if (mapData.empty())
	{
		return false;
	}

	//Reset the map
	NUM_ROW = static_cast<int>(mapData.size());
	NUM_COLUMN = static_cast<int>(mapData[0].size());

	ResetMap(false);

	//Nested for loop to store the tile data read from file
	for (int i = 0; i < NUM_ROW; i++)
	{
		for (int j = 0; j < NUM_COLUMN; j++)
		{
			//Save the data into the map array from the mapData recieved from reading the csv file.
			//mapData row and column must be reversed for it to show properly

			map[i][j].tileType = static_cast<TILE_TYPE>(mapData[i][j]);
			AEVec2 vec = { (TILE_WIDTH * 0.5f) + TILE_WIDTH * j, (TILE_HEIGHT * 0.5f) + TILE_HEIGHT * (NUM_ROW - i) };
			map[i][j].transform.SetPosition(vec);
			map[i][j].transform.SetScale(TILE_WIDTH, TILE_HEIGHT);

			std::cout << "Pos: " << vec.x << ", " << vec.y << std::endl;

			AEVec2 tilePos = GetTilePos(vec.x, vec.y);

			std::cout << "Tile Pos: " << tilePos.x << ", " << tilePos.y << std::endl;

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
bool Map::SaveMap(std::string fileName)
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
void Map::RenderMap(void)
{
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	AEGfxVertexList* texSquareMesh = meshManager->GetMesh(TEXTURE_SQUARE);

	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);

	//Render the map
	for (int i = 0; i < NUM_ROW; i++)
	{
		for (int j = 0; j < NUM_COLUMN; j++)
		{
			TILE_TYPE type = map[i][j].tileType;

			if (type != NONE)
			{
				AEGfxTextureSet(imageMap[map[i][j].tileType], 0, 0);
				AEGfxSetTransform(map[i][j].transform.GetMatrix().m);
				AEGfxMeshDraw(texSquareMesh, AE_GFX_MDM_TRIANGLES);
			}
		}
	}
}

/*!
@brief	Resets the map by clearing it then initialising it with
		empty values
@param	bool clear - whether to clear the map or not (used when first initialising the map)
@return void
*//*______________________________________________________________*/
void Map::ResetMap(bool clear)
{
	ClearMap(clear);

	//Resets the map with new blank data
	map = new Tile* [NUM_ROW];

	for (int i = 0; i < NUM_ROW; i++)
	{
		map[i] = new Tile[NUM_COLUMN];

		for (int j = 0; j < NUM_COLUMN; j++)
		{
			map[i][j].tileType = NONE;
		}
	}
}

/*!
@brief	Clears the map to prevent memory leak
@param	bool clear - whether to clear the map or not (used when first initialising the map)
@return void
*//*______________________________________________________________*/
void Map::ClearMap(bool clear)
{
	if (clear)
	{
		for (int i = 0; i < NUM_ROW; i++)
		{
			delete[] map[i];
		}

		delete[] map;
	}
}

/*!
@brief	Frees all the images and clears the vectors (used when program is shutting down)
@param	void
@return void
*//*______________________________________________________________*/
void Map::FreeImages()
{
	for (int i = 0; i < imageMap.size(); i++)
	{
		AEGfxTextureUnload(imageMap[tileTypesVector[i]]);
	}

	imageMap.clear();
	tileTypesVector.clear();
}

/*!
@brief	Get dimensions of map in CP_Vector form
@param	void
@return CP_Vector - map dimensions
*//*______________________________________________________________*/
AEVec2 Map::GetDimensions(void) const
{
	AEVec2 vec = { static_cast<f32>(NUM_ROW), static_cast<f32>(NUM_COLUMN) };
	return vec;
}

/*!
@brief	Get size of tile
@param	void
@return float - size of tile
*//*______________________________________________________________*/
float Map::GetTileSize(void) const
{
	return TILE_WIDTH;
}

/*!
@brief	Get spawnpoint of the enemies
@param	void
@return CP_Vector - vector of spawnpoint
*//*______________________________________________________________*/
AEVec2 Map::GetSpawnpoint(void) const
{
	return spawnpoint;
}

/*!
@brief	Set spawnpoint of the enemies
@param	CP_Vector - vector of spawnpoint
@return void
*//*______________________________________________________________*/
void Map::SetSpawnpoint(AEVec2 spawnpoint)
{
	this->spawnpoint = spawnpoint;
}


void Map::CleanAStarLists()
{
	while (!openList.empty())
	{
		delete openList.back();
		openList.pop();
	}

	for (Node* node : closedList)
	{
		delete node;
	}
	closedList.clear();
}

void Map::GeneratePath(AEVec2 startPos, AEVec2 targetPos)
{
	CleanAStarLists();

	AEVec2 currentPos{};

	openList.push(new Node(startPos));

	//while (!openList.empty())
	//{
	//	currentPos = openList.front()->position;

	//	//Quit if reached target
	//	if (currentPos.x == targetPos.x && currentPos.y == targetPos.y)
	//	{
	//		//Clean open list
	//		while (!openList.empty())
	//		{
	//			openList.pop();
	//		}

	//		break;
	//	}

	//	//Remove the current node from open list
	//	openList.pop();

	//	//Get 8 surrounding nodes
	//	std::vector<Tile> tiles = GetSurroundingTiles(currentPos);

	//	for (Tile& tile : tiles)
	//	{
	//		tile.parent = currentPos;

	//		AEVec2 currentTileWorldPos = tile.transform.GetPosition();

	//		if (currentTileWorldPos.x == targetPos.x && currentTileWorldPos.y == targetPos.y)
	//		{
	//			//Clean open list
	//			while (!openList.empty())
	//			{
	//				openList.pop();
	//			}

	//			break;
	//		}
	//		
	//		tile.g = currentTile.g + 1.0;
	//		tile.h = CalculateHeuristic(tile.transform.GetPosition(), currentPos);
	//		tile.f = tile.g + tile.h;


	//		closedList.push_back(currentTile);

	//	}

	//}

	// A* Search Algorithm
	/*1.  Initialize the open list
	* 
	2.  Initialize the closed list
	put the starting node on the open
	list(you can leave its f at zero)

	3.  while the open list is not empty
		a) find the node with the least f on
		the open list, call it "q"
		b) pop q off the open list

		c) generate q's 8 successors and set their 
		parents to q

		d) for each successor
		i) if successor is the goal, stop search

		ii) else, compute both g and h for successor
		successor.g = q.g + distance between
		successor and q
		successor.h = distance from goal to
		successor (Euclidean)
		h = sqrt ( (current_cell.x – goal.x)2 +
			(current_cell.y – goal.y)2 )

		successor.f = successor.g + successor.h

		iii) if a node with the same position as
		successor is in the OPEN list which has a
		lower f than successor, skip this successor

		iV) if a node with the same position as
		successor  is in the CLOSED list which has
		a lower f than successor, skip this successor
		otherwise, add  the node to the open list
		end(for loop)

		e) push q on the closed list
		end(while loop)*/
}

unsigned int Map::CalculateHeuristic(AEVec2 newTilePos, AEVec2 targetPos)
{
	return static_cast<unsigned int>(sqrt(pow((newTilePos.x - targetPos.x), 2) +
		pow((newTilePos.y - targetPos.y), 2)));;
}

/*!
@brief	Get up to 4 surrounding tiles based on tile grid pos given
		if they are "active" (not empty)
@param	tilePos - tile grid pos
@return std::map<TILE_RELATIVE_POSITION, Tile> - map that stores 0 - 4 tiles
*//*______________________________________________________________*/
std::vector<Tile> Map::GetSurroundingTiles(AEVec2 tilePos)
{
	std::vector<Tile> tileVector;

	/*AEVec2 positions[8] = { 
		(tilePos.x, tilePos.y + 1), 
		(tilePos.x + 1, tilePos.y + 1), 
		(tilePos.x + 1, tilePos.y), 
		(tilePos.x + 1, tilePos.y - 1), 
		(tilePos.x, tilePos.y - 1), 
		(tilePos.x - 1, tilePos.y - 1), 
		(tilePos.x - 1, tilePos.y),
		(tilePos.x - 1, tilePos.y + 1)
	};

	for (int i = 0; i < 8; i++)
	{
		if (CheckSurroundingTile(positions[i]))
		{
			tileVector.push_back(map[(int)positions[i].x][(int)positions[i].y]);
		}
	}*/

	return tileVector;
}

/*!
@brief	Check whether the surrounding tile is valid by checking if it within bounds and the tile is not
		a ground tile
@param	tilePos - tile grid pos
@return bool - true is valid, false is not
*//*______________________________________________________________*/
bool Map::CheckSurroundingTile(AEVec2 tilePos)
{
	/*if (tilePos.x < 0 || tilePos.x >= NUM_COLUMN || tilePos.y < 0 || tilePos.y >= NUM_ROW || (map[static_cast<int>(tilePos.x)][static_cast<int>(tilePos.y)].tileType >= FLOOR) && (map[static_cast<int>(tilePos.x)][static_cast<int>(tilePos.y)].tileType <= LEFT_PLATFORM))
	{
		return false;
	}*/

	return true;
}

