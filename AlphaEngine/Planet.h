/******************************************************************************/
/*!
\file		Planet.h
\author 	Tan Rui An Ryan (90%)
\co-author	Ong You Yang (5%)
			Leong Wen Bin, Aceson (5%)
\par    	email: ruianryan.t\@digipen.edu
				   youyang.o\@digipen.edu
			       leong.w\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the declaration of functions for the planet.

			Ong You Yang - Planet texture variables

			Aceson - Debris variables

All content (C) 2023 DigiPen Institute of Technology Singapore. All rights reserved.
 */
/******************************************************************************/
#pragma once

#include "Debris.h"
#include <vector>

struct Planets
{
// RUNWAY VARIABLES------------------
	struct Runway
	{
		AEMtx33				scale;
		AEMtx33				rotate;
		AEMtx33				translate;
		AEMtx33				transform;
		AEVec2				position;
		AEVec2				velocity;
		f32					direction;
		f32					size;
		f32					lifespan;
	};
// RUNWAY VARIABLES------------------


// PLANET VARIABLES------------------
	int					id;						//ID for current planet
	int					texture_index; //-yy

	AEMtx33				scale;
	AEMtx33				rotate;
	AEMtx33				translate;
	AEMtx33				transform;
	AEVec2				position;
	f32					direction;				
	f32					size;					// Radius of planet
	bool				wave_complete;			// Flag for whether current planet has anymore shuttles to escape
// PLANET VARIABLES------------------


// ORBIT RANGE VARIABLES-------------
	f32					orbit_range = 75.f;
	AEMtx33				orbit_scale;
	AEMtx33				orbit_rotate;
	AEMtx33				orbit_translate;
	AEMtx33				orbit_transform;

// ORBIT RANGE VARIABLES-------------
	

// SHUTTLE VARIABLES-----------------
	f32					shuttle_direction;		// Direction shuttles will be spawning with
	int					max_shuttle;			// Current maximum number of shuttles for the planet
	int					current_shuttle;		// Current number of shuttles for the planet
	AEVec2				shuttle_spawn_pos;		// Position at which shuttles are to spawn from planet
	f32					shuttle_timer;			// Timer to increment towards shuttle_time_to_spawn
	f32					shuttle_time_to_spawn;	// Randomized value based on range from SHUTTLE_SPAWN_TIME_MAX & SHUTTLE_SPAWN_TIME_MIN
// SHUTTLE VARIABLES-----------------


// DEBRIS VARIABLES------------------
	std::vector<Debris> debris_vector;			// Vector for debris for current planet
	int					max_debris;				// Current maximum number of debris for the planet
// DEBRIS VARIABLES------------------

	
// DRONES VARIABLES------------------
	int					current_drones = 0;			// Current nubmer of drones on the planet
// DRONES VARIABLES------------------


// ============
// Planet Halo
// ============
	f32						halo_size{};
	f32						halo_scale_lerp{};
	AEMtx33					orbit_halo_transform{};

	void load();
	void init();
	void update();
	void draw(AEGfxVertexList* pMesh);
	void free();
	void unload();

	void spawn(int shuttle_randomize_amount);
	void check_spawn(Planets& new_planet);
	void add_runway(AEVec2 const& position);

	f32 get_max_x();
	f32 get_max_y();
	f32 get_min_x();
	f32 get_min_y();
};
