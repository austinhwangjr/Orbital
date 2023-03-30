/******************************************************************************/
/*!
\file		Camera.h
\author 	Tan Rui An Ryan
\par    	email: ruianryan.t\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the declarations of functions for the camera.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#include "AEEngine.h"
#include "Player.h"

// Smaller the number, the faster the camera speed
#define CAM_PLAYER_SPEED		100.f	// Camera speed for player
#define CAM_PLANET_SPEED		1000.f	// Camera speed for planet transition
#define CAM_STATION_SPEED		1000.f	// Camera speed for station transition

#define TRANSITION_TIME_MAX		4.f		// Camera Transition time

struct Camera
{
	AEVec2 position;
	AEVec2 velocity;
	f32 transition_time;

	void init(Player& player);
	void update(f32 frame_time, Player& player);
	void follow_player(f32 frame_time, Player& player);
	void planet_transition(f32 frame_time);
	void station_transition(f32 frame_time);
};