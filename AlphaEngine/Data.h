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


