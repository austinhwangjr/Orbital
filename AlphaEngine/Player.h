/******************************************************************************/
/*!
\file		Player.cpp
\author 	Hwang Jing Rui, Austin
\par    	email: jingruiaustin.hwang\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the declaration of functions for the player.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/
#pragma once
#include "Planet.h"

#define CREDIT_CHEAT	1000;

enum PLAYER_STATES
{
	PLAYER_ORBIT = 0,
	PLAYER_TRANSIT,
	PLAYER_FLY
};

struct Player
{
	// =======
	// Player
	// =======
	PLAYER_STATES	state;
	AEVec2			position;
	AEVec2			velocity;
	f32				size;
	f32				mov_speed;
	f32				rot_speed;
	Planets			current_planet;
	f32				shortest_distance;
	f32				direction;
	int				current_capacity;
	int				max_capacity;
	int				score;
	int				credits;
	bool			can_leave_orbit;
	bool			beam_active;
	f32				timer;
	f32 			max_timer;
	intptr_t movementSoundID;


	// ===============
	// Upgrade Levels
	// ===============
	int				mov_speed_level;
	int				capacity_level;
	int				space_station_count;
	int				beam_level;

	// =============
	// Tractor Beam
	// =============
	AEVec2			beam_pos;
	f32				beam_str;
	f32				beam_width;
	f32				beam_height;

	// ===========
	// Transforms
	// ===========
	AEMtx33			player_transform;
	AEMtx33			beam_transform;

	// ==========
	// Functions
	// ==========
	void load();
	void init();
	void update();
	void draw(AEGfxVertexList*);
	void free();
	void unload();

	void orbit_state();
	void transit_state();
	void flying_state();
};