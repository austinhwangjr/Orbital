/******************************************************************************/
/*!
\file		Data.h
\author 	Leong Wen Bin, Aceson (100%)
\par    	email: leong.w\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the declaration of functions to read data 
			from txt files.

All content � 2023 DigiPen Institute of Technology Singapore. All rights reserved.
 */
/******************************************************************************/
#pragma once
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include "AEEngine.h"
#include "GameStateManager.h"

struct Data
{
	std::string variable_name;
	std::string data;
};

int ImportDataFromFile(const char* FileName, std::vector<Data>& Data, std::map<std::string, f32>& Datamap);
int ImportDataFromFileHowToPlay(const char* FileName);