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
#include <queue>
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
	//7 0 1
	//6   2
	//5 4 3

	NORTH = 0,
	NORTH_EAST,
	EAST,
	SOUTH_EAST,
	SOUTH,
	SOUTH_WEST,
	WEST,
	NORTH_WEST,
	TILE_RELATIVE_TOTAL
};

struct Node
{
	Node() 
		: position{ 0.0f }, parent{ nullptr }, f{ 0.0 }, g{ 0.0 }, h{ 0.0 } {}
	Node(AEVec2 pos)
		: position{ pos }, parent{ nullptr }, f{ 0.0 }, g{ 0.0 }, h{ 0.0 } {}
	Node(AEVec2 pos, Node* p, double f, double g, double h)
		: position{ pos }, parent{ p }, f{ f }, g{ g }, h{ h } {
	}

	Node* parent;

	AEVec2 position;
	double f;
	double g;
	double h;
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

	//2D array to store map info
	Tile** map;

	std::priority_queue<Node*> openList;
	std::vector<Node*> openVector;
	std::vector<Node*> closedList;

	AEVec2 spawnpoint;

	std::string GetImageFilePath(TILE_TYPE tileType);
	void LoadMapTileImages(void);

	//Functions to store tile and texture info
	std::vector<TILE_TYPE> tileTypesVector;
	std::map<TILE_TYPE, AEGfxTexture*> imageMap;

protected:
	Map();
	~Map();

public:
	//Functions to get tile info
	Tile GetTile(float xPos, float yPos) const;
	Tile& GetTileFromTilePos(float xPos, float yPos) const;
	AEVec2 GetTilePos(float xPos, float yPos) const;
	AEVec2 GetTileWorldPos(AEVec2 tilePos) const;
	void SetTile(TILE_TYPE tile, float rowPos, float columnPos);

	//Functions to get map info
	Tile** GetMap(void) const;
	int GetNumCol(void) const;
	int GetNumRow(void) const;

	//Functions to load and save map
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

	//Pathfinding functions
	void CleanAStarLists();
	bool GeneratePath(AEVec2 startPos, AEVec2 targetPos, AEVec2 minLimits, AEVec2 maxLimits);
	std::vector<AEVec2>& BuildPath(std::vector<AEVec2>& path, Node* node);
	double CalculateHeuristic(AEVec2 newTilePos, AEVec2 currentPos);
	std::vector<Node*> GetSurroundingNodes(AEVec2 tilePos, AEVec2 minLimits, AEVec2 maxLimits);
	bool CheckNode(AEVec2 tilePos, AEVec2 minLimits, AEVec2 maxLimits);

	void RenderPathDebug(std::vector<AEVec2> const& path);
};