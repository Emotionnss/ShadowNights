/*!
@file       Map.h
@author     Yap Yu Xuan Daevon (yuxuandaevon.yap)
@brief      This file contains declarations of functions and variables
			used for the Map class
*//*_______________________________________________________________________*/

#pragma once
#include "SingletonTemplate.h"
#include "AEEngine.h"
#include <string>
#include <map>
#include <vector>
#include "../System/MeshManager.h"
#include "../System/SettingsManager.h"
#include "../Player/Transform2D.h"

enum class TILE_TYPE
{
	// 0 - 100: Background
	// 100 - 200: Collisions
	
	NONE = 0,
	LEFT_CHAIN = 1,
	MIDDLE_CHAIN = 2,
	RIGHT_CHAIN = 3,
	LEFT_SKELETON = 4,
	RIGHT_SKELETON = 5,
	LEFTMOST_WEB = 6,
	LEFT_WEB = 7,
	RIGHT_WEB = 8,
	RIGHTMOST_WEB = 9,
	CLOSED_DOOR = 99,
	BACKGROUND = 100,
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
	OPEN_DOOR = 161,
	BAT_ENEMY = 200,
	ARCHER_ENEMY = 201,
	SHADOW_ENEMY = 202,
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

//Node overloads
bool operator==(Node const& lhs, Node const& rhs);
bool operator!=(Node const& lhs, Node const& rhs);

struct Tile
{
	Transform2D transform;
	TILE_TYPE tileType = TILE_TYPE::NONE;
};

class Map : public SingletonTemplate<Map>
{
	friend class SingletonTemplate<Map>;

private:
	static const char* TILE_ASSETS_FILE_PATH;
	bool firstLoad = false;
	s32 NUM_COLUMN;
	s32 NUM_ROW;

	MeshManager* meshManager;

	SettingsManager* settingsManager;

	//2D array to store map info
	Tile** map;

	//Vector to allow checking of node in openList (since its very hard to iterate through queue)
	std::vector<Node*> openList;

	//Stores nodes that have already been checked
	std::vector<Node*> closedList;

	//ADT's to store tile and texture info
	std::vector<TILE_TYPE> tileTypesVector;
	std::map<TILE_TYPE, AEGfxTexture*> imageMap;

	/*!
	@brief	Gets the image file path based on the tileType of the tile
	@param	TILE_TYPE tileType - type of the tile based off the enum TILE_TYPE
	@return string - filePath of the file to get
	*/
	std::string GetImageFilePath(TILE_TYPE tileType);

	/**
	@brief	Load all the map tile images and inserts them s32o the imageMap
	@param	void
	@return void
	*/
	void LoadMapTileImages();

protected:
	/*!
	@brief Constructor. Makes it set the map array to empty and loads all map tile images
	*/
	Map();

	/*!
	@brief Destructor. Used to clean up the map
	*/
	~Map();

public:
	/*!
	@brief	Gets a tile based on the x & y postion given
	@param	f32 xPos - x position
	@param	f32 yPos - y position
	@return Tile& - reference to tile
	*/
	Tile& GetTile(f32 xPos, f32 yPos) const;

	/*!
	@brief	Gets a tile based on the x & y tile pos given
	@param	f32 xPos - x tile position
	@param	f32 yPos - y tile position
	@return Tile& - reference to tile
	*/
	Tile& GetTileFromTilePos(f32 xPos, f32 yPos) const;

	/*!
	@brief	Gets tile grid pos based on x & y position given
	@param	f32 xPos - x position
	@param	f32 yPos - y position
	@return AEVec2 - tile grid pos
	*/
	AEVec2 GetTilePos(f32 xPos, f32 yPos) const;

	/*!
	@brief	Gets tile world pos based on tile grid pos
	@param	AEVec2 tilePos - tile grid pos
	@return AEVec2 - tile world pos
	*/
	AEVec2 GetTileWorldPos(AEVec2 tilePos) const;

	/*!
	@brief	Set tile type based on given grid position
	@param	f32 rowPos - tile grid row pos
	@param	f32 columnPos - tile grid column pos
	@return void
	*/
	void SetTile(TILE_TYPE tile, f32 rowPos, f32 columnPos);

	/*!
	@brief	Get the map
	@param	void
	@return Tile** - map
	*/
	Tile** GetMap() const;

	/*!
	@brief	Get number of columns
	@param	void
	@return s32 - number of columns
	*/
	s32 GetNumCol() const;

	/*!
	@brief	Get number of rows
	@param	void
	@return s32 - number of rows
	*/
	s32 GetNumRow() const;

	/*!
	@brief	Loads the map using CSVManager based off the fileName,
			stores this into in the map array
	@param	std::string fileName - name of the file
	@return bool
	*/
	bool LoadMap(std::string fileName);

	/*!
	@brief	Saves the map by using CSVManager to write to csv
	@param	std::string fileName - name of the file
	@return bool
	*/
	bool SaveMap(std::string fileName);

	/*!
	@brief	Render the map by using the data from the map array
	@param	void
	@return void
	*/
	void RenderMap();

	/*!
	@brief	Resets the map by clearing it then initialising it with
			empty values
	@param	bool clear - whether to clear the map or not (used when first initialising the map)
	@return void
	*/
	void ResetMap();

	/*!
	@brief	Clears the map to prevent memory leak
	@param	bool clear - whether to clear the map or not (used when first initialising the map)
	@return void
	*/
	void ClearMap(bool clear);

	/*!
	@brief	Frees all the images and clears the vectors (used when program is shutting down)
	@param	void
	@return void
	*/
	void FreeImages();

	/*!
	@brief	Get dimensions of map in AEVec2 form
	@param	void
	@return AEVec2 - map dimensions
	*/
	AEVec2 GetDimensions() const;

	/*!
	@brief	Get size of tile
	@param	void
	@return f32 - size of tile
	*/
	f32 GetTileSize() const;

	/*!
	@brief	Clean all lists used for A* pathfinding
	@param	void
	@return void
	*/
	void CleanAStarLists();

	/*!
	@brief	Generate path using A* pathfinding
	@param	AEVec2 startPos - start position to generate path from
	@param	AEVec2 targetPos - target position to generate path to
	@param	AEVec2 minLimits - minimum x and y value to constrain path to
	@param	AEVec2 maxLimits - maximum x and y value to constrain path to
	@return bool - true if sucessful, false otherwise
	*/
	bool GeneratePath(AEVec2 startPos, AEVec2 targetPos, AEVec2 minLimits, AEVec2 maxLimits, Node& node);

	/*!
	@brief	Builds a path by iterating through all nodes connected to each other
	@param	std::vector<AEVec2>& path - reference to vector storing coordinates of the path
	@param	Node* node - target node to reach
	@return std::vector<AEVec2>& - reference to vector storing coordinates of the path
	*/
	std::vector<AEVec2>& BuildPath(std::vector<AEVec2>& path, Node& node);

	/*!
	@brief	Calculate heuristic (h) value for A* pathfinding
	@param	AEVec2 newTilePos - tile world position to calculate from
	@param	AEVec2 targetPos - tile world position to calculate to
	@return double - new h value
	*/
	double CalculateHeuristic(AEVec2 newTilePos, AEVec2 currentPos);

	/*!
	@brief	Get up to 8 surrounding nodes based on node pos given
			if they are "active" (not empty)
	@param	AEVec2 nodePos - node pos
	@param	AEVec2 minLimits - minimum x and y value to constrain path to
	@param	AEVec2 maxLimits - maximum x and y value to constrain path to
	@return std::vector<Node*> - vector that stores 0 - 8 nodes
	*/
	std::vector<Node*> GetSurroundingNodes(AEVec2 tilePos, AEVec2 minLimits, AEVec2 maxLimits);

	/*!
	@brief	Check whether the surrounding tile is valid by checking if it within bounds and the tile is not a solid tile
	@param	AEVec2 tilePos - tile grid pos
	@param	AEVec2 minLimits - minimum x and y value to constrain tile position to
	@param	AEVec2 maxLimits - maximum x and y value to constrain tile position to
	@return bool - true is valid, false is not
	*/
	bool CheckTile(AEVec2 tilePos, AEVec2 minLimits, AEVec2 maxLimits);

	/*!
	@brief	Checks if tile should be solid or not
	@param	TILE_TYPE type - type of tile returned
	@return bool - true is valid, false is not
	*/
	bool CheckTile(TILE_TYPE type);

	/*!
	@brief	Render a line that shows the path that was calculated
	@param	std::vector<AEVec2>& path - reference to the path
	@return void
	*/
	void RenderPathDebug(std::vector<AEVec2>& path);

	/*!
	@brief	Render all tiles that have been checked by A*
	@return void
	*/
	void RenderCheckedTilesDebug(float r = 1.0f, float g = 0.0f, float b = 0.0f);
};