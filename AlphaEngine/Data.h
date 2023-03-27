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
int ImportDroneDataFromFile(const char* FileName, std::vector<Data>& DroneData, std::map<std::string, f32>& DroneDatamap);
int ImportPlayerProjDataFromFile(const char* FileName, std::vector<Data>& ProjData, std::map<std::string, f32>& ProjDatamap);
int ImportDataFromFileHowToPlay(const char* FileName);


