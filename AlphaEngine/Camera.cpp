/******************************************************************************/
/*!
\file		Camera.cpp
\author 	Tan Rui An Ryan
\par    	email: ruianryan.t\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the definition of functions for the camera 
			system.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/
#include "Camera.h"
#include "WaveManager.h"
#include "SpaceStation.h"
#include "Data.h"
#include "GameStateList.h"

//Smaller the number, the faster the camera speed
static float CAM_PLAYER_SPEED;			// Camera speed for player
static float CAM_PLANET_SPEED;			// Camera speed for planet transition
static float CAM_STATION_SPEED;			// Camera speed for station transition
static float TRANSITION_TIME_MAX;		// Camera Transition time


extern WaveManager wave_manager;
extern std::vector<SpaceStation> space_station_vector;

// IMPORT DATA VECTOR
std::map<std::string, f32> 	CameraDataMap;
std::vector<Data> 			CameraData;

void Camera::init(Player& current_player)
{
	ImportDataFromFile("Assets/GameObjectData/CameraData.txt", CameraData, CameraDataMap);
	CAM_PLAYER_SPEED	= CameraDataMap["Camera_Player_Speed"];
	CAM_PLANET_SPEED	= CameraDataMap["Camera_Planet_Speed"];
	CAM_STATION_SPEED	= CameraDataMap["Camera_Station_Speed"];
	TRANSITION_TIME_MAX = CameraDataMap["Maximum_Transition_Time"];

	AEVec2Zero(&velocity);
	transition_time = 0.f;
	position.x = current_player.position.x;
	position.y = current_player.position.y;
}

void Camera::update(Player& current_player)
{
	if (wave_manager.planet_adding)
	{
		planet_transition();
	}
	else if (wave_manager.capacity_full)
	{
		station_transition();
	}
	else
	{
		follow_player(current_player);
	}

	AEGfxGetCamPosition(&g_camPos.x, &g_camPos.y);
	
	if (next_state != GS_RESTART) {
		CameraData.clear();
		CameraDataMap.clear();
	}
}

void Camera::follow_player(Player& current_player)
{
	f32 t{};
	t += g_dt * (AEVec2Distance(&current_player.position, &position) / CAM_PLAYER_SPEED);
	AEVec2Lerp(&position, &position, &current_player.position, t);

	AEGfxSetCamPosition(position.x, position.y);
}

void Camera::planet_transition()
{
	transition_time += g_dt;
	f32 t{};
	t += g_dt * (AEVec2Distance(&planet_vector[planet_vector.size() - 1].position, &position) / CAM_PLANET_SPEED);
	AEVec2Lerp(&position, &position, &planet_vector[planet_vector.size() - 1].position, t);

	AEGfxSetCamPosition(position.x, position.y);

	if (transition_time >= TRANSITION_TIME_MAX)
	{
		wave_manager.planet_adding = false;
		transition_time = 0.f;
	}
}

void Camera::station_transition()
{
	transition_time += g_dt;
	f32 t{};
	t += g_dt * (AEVec2Distance(&space_station_vector[0].position, &position) / CAM_STATION_SPEED);
	AEVec2Lerp(&position, &position, &space_station_vector[0].position, t);

	AEGfxSetCamPosition(position.x, position.y);

	if (transition_time >= TRANSITION_TIME_MAX)
	{
		wave_manager.capacity_full = false;
		transition_time = 0.f;
	}
}