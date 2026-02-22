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
#include <algorithm>

const char* Map::TILE_ASSETS_FILE_PATH = "Assets/Tiles/";

Map::Map()
{
	LoadMapTileImages();

	meshManager = MeshManager::GetInstance();
}

Map::~Map()
{
	FreeImages();
	CleanAStarLists();
	ClearMap(true);
}

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

void Map::LoadMapTileImages()
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

	for (s32 i = 0; i < tileTypesVector.size(); i++)
	{
		AEGfxTexture* pTex = AEGfxTextureLoad(GetImageFilePath(tileTypesVector[i]).c_str());
		imageMap.insert({ tileTypesVector[i], pTex });
	}
}

Tile& Map::GetTile(f32 xPos, f32 yPos) const
{
	AEVec2 tilePos = GetTilePos(xPos, yPos);

	return GetTileFromTilePos(tilePos.x, tilePos.y);
}

Tile& Map::GetTileFromTilePos(f32 xPos, f32 yPos) const
{
	Tile tile = map[NUM_ROW - static_cast<s32>(yPos)][static_cast<s32>(xPos)];
	return map[NUM_ROW - static_cast<s32>(yPos)][static_cast<s32>(xPos)];
}

AEVec2 Map::GetTilePos(f32 xPos, f32 yPos) const
{
	s32 rowPos = 0, columnPos = 0;

	rowPos = static_cast<s32>(floor(xPos / TILE_WIDTH));
	columnPos = static_cast<s32>(floor(yPos / TILE_HEIGHT));

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

AEVec2 Map::GetTileWorldPos(AEVec2 tilePos) const
{
	return GetTileFromTilePos(tilePos.x, tilePos.y).transform.GetPosition();
}

void Map::SetTile(TILE_TYPE tile, f32 rowPos, f32 columnPos)
{
	map[static_cast<s32>(columnPos)][static_cast<s32>(rowPos)].tileType = tile;
}

Tile** Map::GetMap() const
{
	return this->map;
}

s32 Map::GetNumCol() const
{
	return NUM_COLUMN;
}

s32 Map::GetNumRow() const
{
	return NUM_ROW;
}

bool Map::LoadMap(std::string fileName)
{
	std::string file;

	file = fileName;

	//Get the map data from reading the file
	std::vector<std::vector<s32>> mapData = CSVManager::ReadMap(file);

	//Stop if empty
	if (mapData.empty())
	{
		return false;
	}

	//Reset the map
	ClearMap(firstLoad);

	if (!firstLoad)
	{
		firstLoad = true;
	}

	NUM_ROW = static_cast<s32>(mapData.size());
	NUM_COLUMN = static_cast<s32>(mapData[0].size());

	ResetMap();

	//Nested for loop to store the tile data read from file
	for (s32 i = 0; i < NUM_ROW; i++)
	{
		for (s32 j = 0; j < NUM_COLUMN; j++)
		{
			//Save the data s32o the map array from the mapData recieved from reading the csv file.
			//mapData row and column must be reversed for it to show properly

			map[i][j].tileType = static_cast<TILE_TYPE>(mapData[i][j]);
			AEVec2 vec = { (TILE_WIDTH * 0.5f) + TILE_WIDTH * j, (TILE_HEIGHT * 0.5f) + TILE_HEIGHT * (NUM_ROW - i) };
			map[i][j].transform.SetPosition(vec);
			map[i][j].transform.SetScale(TILE_WIDTH, TILE_HEIGHT);

			//std::cout << "Pos: " << vec.x << ", " << vec.y << std::endl;

			AEVec2 tilePos = GetTilePos(vec.x, vec.y);

			//std::cout << "Tile Pos: " << tilePos.x << ", " << tilePos.y << std::endl;

			/*if (map[i][j].tileType == STARTPOs32)
			{
				spawnpos32 = map[i][j].pos;
			}*/
		}
	}

	return true;
}


bool Map::SaveMap(std::string fileName)
{
	std::vector<std::vector<s32>> mapData;

	//mapData row and column must be reversed for it to save properly
	for (s32 i = 0; i < NUM_COLUMN; i++)
	{
		std::vector<s32> columnData;

		for (s32 j = 0; j < NUM_ROW; j++)
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

void Map::RenderMap()
{
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	AEGfxVertexList* texSquareMesh = meshManager->GetMesh(TEXTURE_SQUARE);

	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);

	//Render the map
	for (s32 i = 0; i < NUM_ROW; i++)
	{
		for (s32 j = 0; j < NUM_COLUMN; j++)
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

void Map::ResetMap()
{
	//Resets the map with new blank data
	map = new Tile*[NUM_ROW];

	for (s32 i = 0; i < NUM_ROW; i++)
	{
		map[i] = new Tile[NUM_COLUMN];

		for (s32 j = 0; j < NUM_COLUMN; j++)
		{
			map[i][j].tileType = NONE;
		}
	}
}

void Map::ClearMap(bool clear)
{
	if (clear)
	{
		for (s32 i = 0; i < NUM_ROW; i++)
		{
			delete[] map[i];
		}

		delete[] map;
	}
}

void Map::FreeImages()
{
	for (s32 i = 0; i < imageMap.size(); i++)
	{
		AEGfxTextureUnload(imageMap[tileTypesVector[i]]);
	}

	imageMap.clear();
	tileTypesVector.clear();
}

AEVec2 Map::GetDimensions(void) const
{
	AEVec2 vec = { static_cast<f32>(NUM_ROW), static_cast<f32>(NUM_COLUMN) };
	return vec;
}

f32 Map::GetTileSize(void) const
{
	return TILE_WIDTH;
}

AEVec2 Map::GetSpawnpoint(void) const
{
	return spawnpoint;
}

void Map::SetSpawnpoint(AEVec2 spawnpoint)
{
	this->spawnpoint = spawnpoint;
}

void Map::CleanAStarLists()
{
	/*while (!openList.empty())
	{
		Node* node = openList.top();
		delete node;
		node = nullptr;
		openList.pop();
	}*/

	for (Node* node : openList)
	{
		if (node != nullptr)
		{
			delete node;
			node = nullptr;
		}
	}

	openList.clear();

	for (Node* node : closedList)
	{
		if (node != nullptr)
		{
			delete node;
			node = nullptr;
		}
	}

	closedList.clear();
}

bool Map::GeneratePath(AEVec2 startPos, AEVec2 targetPos, AEVec2 minLimits, AEVec2 maxLimits, Node& node)
{
	CleanAStarLists();

	Node* currentNode{};
	AEVec2 currentPos{};

	//Get closest tile coords to node
	Tile tile = GetTile(startPos.x, startPos.y);

	openList.push_back(new Node(tile.transform.GetPosition()));

	while (!openList.empty())
	{
		//Sort open list for lowest f value
		std::sort(openList.begin(), openList.end(), 
			[](Node* const& lhs, Node* const& rhs)
			{
				return lhs->f < rhs->f;
			});

		//Get node with lowest f value on openList
		currentNode = openList[0];
		currentPos = currentNode->position;

		//End finding of path if current node is reached
		if (currentNode->position.x == targetPos.x && currentNode->position.y == targetPos.y)
		{
			node = *currentNode;
			return true;
		}

		//Remove the current node from openList
		openList.erase(openList.begin());
		
		//Push back current node to closed list
		closedList.push_back(currentNode);

		//Get surrounding nodes that are valid
		std::vector<Node*> nodes = GetSurroundingNodes(currentPos, minLimits, maxLimits);

		for (Node* node : nodes)
		{
			bool proceed{ true };

			//Check if current node has already been evaluated
			for (Node* closedNode : closedList)
			{
				if (node->position.x == closedNode->position.x && node->position.y == closedNode->position.y)
				{
					proceed = false;
					continue;
				}
			}

			//If already evaluated, skip to next node
			if (!proceed)
			{
				delete node;
				node = nullptr;
				continue;
			}

			//Set tentative g value
			double tempGValue = currentNode->g + 1.0;

			//Loop through open list to see if node is already inside
			std::vector<Node*>::iterator it = std::find(openList.begin(), openList.end(), node);

			//If not in open list, add it
			if (it == openList.end())
			{
				openList.push_back(node);
				//openVector.push_back(node);
			}
			//If tentative g value is not better, skip to next node
			else if (tempGValue >= node->g)
			{
				delete node;
				node = nullptr;
				continue;
			}

			//Set node values since this node is valid
			node->parent = currentNode;
			node->g = tempGValue;
			node->h = CalculateHeuristic(node->position, currentPos);
			node->f = node->g + node->h;
		}
	}

	return false;
}

std::vector<AEVec2>& Map::BuildPath(std::vector<AEVec2>& path, Node& node)
{
	path.clear();

	Node* currentNode = &node;

	//Generate path by backtracking from target node to start node
	while (currentNode != nullptr)
	{
		path.push_back(currentNode->position);
		currentNode = currentNode->parent;
	}

	//Reverse the path to get from start to target
	std::reverse(path.begin(), path.end());

	return path;
}

double Map::CalculateHeuristic(AEVec2 newTilePos, AEVec2 targetPos)
{
	return static_cast<double>(sqrt(pow((newTilePos.x - targetPos.x), 2) +
		pow((newTilePos.y - targetPos.y), 2)));;
}

std::vector<Node*> Map::GetSurroundingNodes(AEVec2 nodePos, AEVec2 minLimits, AEVec2 maxLimits)
{
	std::vector<Node*> nodeVector;

	//Doing this to get tilePos to use as index
	AEVec2 tilePos = GetTilePos(nodePos.x, nodePos.y);

	//Get surrounding tile indexes
	AEVec2 positions[8] = {
		{tilePos.x, tilePos.y + 1},
		{tilePos.x + 1, tilePos.y + 1},
		{tilePos.x + 1, tilePos.y},
		{tilePos.x + 1, tilePos.y - 1},
		{tilePos.x, tilePos.y - 1},
		{tilePos.x - 1, tilePos.y - 1},
		{tilePos.x - 1, tilePos.y},
		{tilePos.x - 1, tilePos.y + 1}
	};

	//Loop through the positions
	for (AEVec2 pos : positions)
	{
		//Check if position is valid
		if (CheckTile(pos, minLimits, maxLimits))
		{
			AEVec2 nodeWorldPos = GetTileWorldPos(pos);
			nodeVector.push_back(new Node(nodeWorldPos));
		}
	}

	return nodeVector;
}

bool Map::CheckTile(AEVec2 tilePos, AEVec2 minLimits, AEVec2 maxLimits)
{
	if (tilePos.x < minLimits.x || tilePos.x >= maxLimits.x || tilePos.y < minLimits.y || tilePos.y >= maxLimits.y || tilePos.x < 0 || tilePos.x >= NUM_COLUMN || tilePos.y < 0 || tilePos.y >= NUM_ROW || (map[NUM_ROW - static_cast<s32>(tilePos.y)][static_cast<s32>(tilePos.x)].tileType >= FLOOR) && (map[NUM_ROW - static_cast<s32>(tilePos.y)][static_cast<s32>(tilePos.x)].tileType <= LEFT_PLATFORM))
	{
		return false;
	}

	return true;
}

bool Map::CheckTile(TILE_TYPE type)
{
	return (type >= FLOOR && type <= RIGHT_PLATFORM);
}

void Map::RenderPathDebug(std::vector<AEVec2>& path)
{
	AEGfxVertexList* lineMesh = meshManager->GetMesh(LINE);

	for (int i = 0; i < path.size() - 1; ++i)
	{
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);

		AEGfxSetColorToAdd(1.0f, 0.0f, 0.0f, 0.5f);
		AEGfxSetColorToMultiply(0.0f, 0.0f, 0.0f, 0.5f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);

		//Set line rotation
		//Get angle in radians from currentPos to the nextPos
		float angle = atan2f(path[i + 1].y, path[i + 1].x) - atan2f(path[i].y, path[i].x);

		//Set line length
		//Get magnitude from one edge of the square to the other based on the rotation
		float magnitude = AEVec2Distance(&path[i + 1], &path[i]);

		//Create transform
		Transform2D transform{ path[i], {1.0f, 1.0f}, angle};

		AEGfxSetTransform(transform.GetMatrix().m);
		AEGfxMeshDraw(lineMesh, AE_GFX_MDM_TRIANGLES);
	}
}

