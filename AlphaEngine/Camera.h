/******************************************************************************/
/*!
\file		Camera.h
\author 	Tan Rui An Ryan (100%)
\par    	email: ruianryan.t\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the declarations of functions for the camera.

All content � 2023 DigiPen Institute of Technology Singapore. All rights reserved.
 */
 /******************************************************************************/
#pragma once
#include "AEEngine.h"
#include "Player.h"



struct Camera
{
	AEVec2 position;
	AEVec2 velocity;
	f32 transition_time;

	void init(Player& current_player);
	void update(Player& current_player);
	void follow_player(Player& current_player);
	void planet_transition();
	void station_transition();
};