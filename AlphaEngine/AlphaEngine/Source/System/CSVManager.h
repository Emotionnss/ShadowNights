/*!
@file       CSVManager.h
@author     Yap Yu Xuan Daevon (yuxuandaevon.yap)
@brief      This file contains declaration of functions used to read and write to CSV
*//*_______________________________________________________________________*/

#pragma once

#include <string>
#include <vector>

class CSVManager
{
private:
	static const char* MAP_FILE_PATH;
	static const char* SPRITE_DATA_FILE_PATH;

public:
	CSVManager();
	~CSVManager();

	//Not used currently
	static bool write(std::string fileName, const int TILE_ROW_NUM, const int TILE_COLUMN_NUM, std::vector<std::vector<int>> data);

	/**
	 @brief Read map information from a CSV file
	 @param std::string fileName - name of the file to read
	 @return std::vector<std::vector<int>> - map data
	 */
	static std::vector<std::vector<int>> ReadMap(std::string fileName);

	/**
	 @brief Read spritesheet information from a CSV file
	 @param std::string spritesheetName - name of the spritesheet to get info from
	 @return std::vector<std::string> - spritesheet data
	 */
	static std::vector<std::string> ReadSpriteInfo(std::string spritesheetName);

	/**
	 @brief Separate a given string by using the given separator and stores all the information into a string vector
	 @param std::string line - line to tokenise
	 @param char separator - char that denotes what to separate with
	 @return std::vector<std::string> - tokenised information
	 */
	static std::vector<std::string> Tokenise(std::string line, char separator);
};

