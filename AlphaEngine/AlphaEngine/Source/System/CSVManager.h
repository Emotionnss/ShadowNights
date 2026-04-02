/*!
@file       CSVManager.h
@author     Yap Yu Xuan Daevon (yuxuandaevon.yap)
@brief      This file contains declarations of functions used to read and write to CSV
*//*_______________________________________________________________________*/

#pragma once

#include <string>
#include <vector>
#include "AEEngine.h"

class CSVManager
{
private:
	static const char* MAP_FILE_PATH;

public:
	CSVManager();
	~CSVManager();

	/**
	 @brief Write to settings CSV file
	 @param std::string fileName - name of the file to read
	 @return std::vector<std::vector<int>> - map data
	 */
	static bool WriteSettingsInfo(std::vector<s32> data);

	/**
	 @brief Read map information from a CSV file
	 @param std::string fileName - name of the file to read
	 @return std::vector<std::vector<int>> - map data
	 */
	static std::vector<std::vector<s32>> ReadMap(std::string fileName);

	/**
	 @brief Read information from a CSV file
	 @param std::string headerName - header name to check against
	 @return std::vector<std::string>
	 */
	static std::vector<std::string> ReadInfo(std::string headerName);

	/**
	 @brief Read settings information from a CSV file
	 @param std::string headerName - header name to check against
	 @return std::vector<std::string>
	 */
	static std::vector<std::string> ReadSettingsInfo();
	
	/**
	 @brief Read animation information from a CSV file
	 @param std::string animationName - animation name to check against
	 @return sstd::vector<std::vector<std::string>> - animation data
	 */
	static std::vector<std::vector<std::string>> ReadAnimationInfo(std::string animationName);

	/**
	 @brief Read tutorial information from a CSV file
	 @param void
	 @return sstd::vector<std::vector<std::string>> - animation data
	 */
	static std::vector<std::vector<std::string>> ReadTutorialInfo();

	/**
	 @brief Separate a given string by using the given separator and stores all the information into a string vector
	 @param std::string line - line to tokenise
	 @param char separator - char that denotes what to separate with
	 @return std::vector<std::string> - tokenised information
	 */
	static std::vector<std::string> Tokenise(std::string line, char separator);
};

