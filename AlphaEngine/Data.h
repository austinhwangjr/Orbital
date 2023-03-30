/******************************************************************************/
/*!
\file		Data.h
\author 	Leong Wen Bin, Aceson
\par    	email: leong.w\@digipen.edu
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
#include "AEEngine.h"
#include "GameStateManager.h"

struct Data
{
	std::string variable_name;
	std::string data;
};

int ImportDataFromFile(const char* FileName, std::vector<Data>& Data, std::map<std::string, f32>& Datamap);
int ImportDataFromFileHowToPlay(const char* FileName);