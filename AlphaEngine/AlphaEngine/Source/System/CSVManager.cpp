/*!
@file       CSVManager.cpp
@author     Yap Yu Xuan Daevon (yuxuandaevon.yap)
@brief      This file contains definition of functions used to read and write to CSV
*//*_______________________________________________________________________*/

#include "CSVManager.h"
#include <sstream>
#include <fstream>
#include <stdexcept> // runtime_error
#include <iostream>

const char* CSVManager::MAP_FILE_PATH = "Assets/Maps/";
const char* CSVManager::SPRITE_DATA_FILE_PATH = "Assets/Data/SpriteData.csv";

CSVManager::CSVManager()
{
	
}

CSVManager::~CSVManager()
{
}

/**
 @brief Write to a CSV file
 @param fileName A string containing the name of the input file
 @param TILE_ROW_NUM A const int containing the number of tiles in the x-axis
 @param TILE_COLUMN_NUM A const int containing the number of tiles in the y-axis
 @param data A vector containing the value of the vector which represents the tile map
 @return boolean variable
 */
bool CSVManager::write(std::string fileName, const int TILE_ROW_NUM, const int TILE_COLUMN_NUM, std::vector<std::vector<int>> data)
{
	bool writeSucessful = false;

	//Creates an output filestream and opens the file
	std::string filePath = MAP_FILE_PATH + fileName + ".csv";
	std::fstream file(filePath, std::fstream::out | std::fstream::trunc);

	// Make sure the file is open
	try 
	{
		if (!file.is_open())
		{
			throw std::runtime_error("Could not open file");
		}
	}
	catch (std::runtime_error e)
	{
		//std::cout << "Runtime error: " << e.what();
		return false;
	}

	//Checks if the number of columns is correct
	if (data.size() == TILE_COLUMN_NUM)
	{
		//Sets the iterator for the row to the beginning of x vector
		std::vector<std::vector<int>>::iterator rowIterator = data.begin();

		//Checks if the number of rows is correct
		if ((*rowIterator).size() == TILE_ROW_NUM)
		{
			//Writes the values into the file
			for (rowIterator = data.begin(); rowIterator != data.end(); rowIterator++)
			{
				//Set iterator for the column
				std::vector<int>::iterator columnIterator;

				//Loop through the column values
				for (columnIterator = (*rowIterator).begin(); columnIterator != (*rowIterator).end(); columnIterator++)
				{
					//Insert column value into file
					file << *columnIterator;

					//Use a tab token to split the values only if its not at the end
					if (columnIterator != (*rowIterator).end())
						file << '\t';
				}

				//Adds endline to file so that it starts from next row
				file << std::endl;
			}
		}

		writeSucessful = true;
	}

	//Close the file
	file.close();

	return writeSucessful;
}

std::vector<std::vector<int>> CSVManager::ReadMap(std::string fileName)
{
	//Double vector to store the tile data
	std::vector<std::vector<int>> rowValues;

	//Creates an output filestream and opens the file
	std::string filePath = MAP_FILE_PATH + fileName + ".csv";
	std::ifstream file{filePath, std::ios::in};

	// Make sure the file is open
	try 
	{
		if (!file.is_open())
		{
			throw std::runtime_error("Could not open file");
		}
	}
	catch (std::runtime_error e)
	{
		//std::cout << "Runtime error: " << e.what();
		return rowValues;
	}

	std::string line;
	int value = 0;

	//Gets the values of an entire row, repeats until end of csv file
	while (getline(file, line))
	{
		//Create stringstream to allow the program to read the values
		std::stringstream ss(line);

		//Create vector to store the column values
		std::vector<int> columnValues;

		while (ss >> value)
		{
			//Add the current column value to the vector
			columnValues.push_back(value);

			//If value is a comma, tab or whitespace, ignore it 
			if (ss.peek() == '\t' || ss.peek() == ' ' || ss.peek() == ',')
				ss.ignore();
		}

		rowValues.push_back(columnValues);
	}

	//Close the file
	file.close();

	std::cout << "Read map" << std::endl;

	return rowValues;
}

std::vector<std::string> CSVManager::ReadSpriteInfo(std::string spritesheetName)
{
	//Get sprite info in string
	std::vector<std::string> spriteInfo;

	//Creates an output filestream and opens the file
	std::ifstream file{ SPRITE_DATA_FILE_PATH, std::ios::in };

	// Make sure the file is open
	try
	{
		if (!file.is_open())
		{
			throw std::runtime_error("Could not open file");
		}
	}
	catch (std::runtime_error e)
	{
		//std::cout << "Runtime error: " << e.what();
		return spriteInfo;
	}

	std::string line;

	//Gets line by line until end of file
	while (getline(file, line))
	{
		//Get the spritesheet name
		std::string name = line.substr(0, line.find_first_of(","));

		//If name matches, save info in vector and return
		if (name == spritesheetName)
		{
			spriteInfo = Tokenise(line, ',');
			return spriteInfo;
		}
	}

	return spriteInfo;
}

std::vector<std::string> CSVManager::Tokenise(std::string line, char separator)
{
	//List of separated tokens
	std::vector<std::string> tokens;

	//Start and end of line
	size_t start, end;

	//Temp placeholder for individual token to be pushed into vector
	std::string token;

	//Find beginning of line
	start = line.find_first_not_of(separator, 0);

	do {
		//Find first separator from start onwards
		end = line.find_first_of(separator, start);

		//Quit loop if the start and the end is the same
		if (start == line.length() || start == end)
		{
			break;
		}
		//Get substr based of the starting pos and length of word
		else if (end >= 0)
		{
			token = line.substr(start, end - start);
		}
		//Get substr based off the starting pos and length of last word
		else
		{
			token = line.substr(start, line.length() - start);
		}

		tokens.push_back(token);
		
		start = end + 1;

	} while (end != std::string::npos);

	return tokens;
}
