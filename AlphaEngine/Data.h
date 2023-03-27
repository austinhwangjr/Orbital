#pragma once
#include <string>
#include <iostream>
#include <map>
#include <vector>


struct Data
{
	std::string variable_name;
	std::string data;
};

int ImportPlayerDataFromFile(const char* FileName, std::vector<Data>& PlayerData, std::map<std::string, f32>& PlayerDatamap);
int ImportDataFromFileHowToPlay(const char* FileName);

