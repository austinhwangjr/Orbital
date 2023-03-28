/******************************************************************************/
/*!
\file		Data.h
\author 	 
\par    	email: \@digipen.edu
\date   	March 28, 2023
\brief		This file contains the declaration of functions to read data 
			from txt files.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/
#pragma once
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>

struct Data
{
	std::string variable_name;
	std::string data;
};

int ImportPlayerDataFromFile(const char* FileName, std::vector<Data>& PlayerData, std::map<std::string, f32>& PlayerDatamap);
int ImportDroneDataFromFile(const char* FileName, std::vector<Data>& DroneData, std::map<std::string, f32>& DroneDatamap);
int ImportPlayerProjDataFromFile(const char* FileName, std::vector<Data>& ProjData, std::map<std::string, f32>& ProjDatamap);
int ImportSpaceStationDataFromFile(const char* FileName, std::vector<Data>& StationData, std::map<std::string, f32>& StationDatamap);
int ImportDataFromFileHowToPlay(const char* FileName);