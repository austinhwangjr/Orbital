#pragma once
#define PLANET_SIZE				200.0f		// Planet base size (radius)	
#define SHUTTLE_SPAWN_TIME_MAX	8			// Maximum time shuttles will spawn
#define SHUTTLE_SPAWN_TIME_MIN	4			// Minimum time shuttles will spawn
#define DEBRIS_MAX				20			// Maximum number of debris on a planet
#define DEBRIS_MIN				10			// Minimum number of debris on a planet
#define DRONES_MAX				5			// Maximum number of drones on a planet

#include "Debris.h"
#include <vector>

struct Planets
{
// PLANET VARIABLES------------------
	int					id;						//ID for current planet
	AEMtx33				scale;
	AEMtx33				rotate;
	AEMtx33				translate;
	AEMtx33				transform;
	AEVec2				position;
	double				size;					// Radius of planet
	bool				wave_complete;			// Flag for whether current planet has anymore shuttles to escape
// PLANET VARIABLES------------------

// SHUTTLE VARIABLES-----------------
	int					max_shuttle;			// Current maximum number of shuttles for the planet
	int					current_shuttle;		// Current number of shuttles for the planet
	AEVec2				shuttle_spawn_pos;		// Position at which shuttles are to spawn from planet
	f64					shuttle_timer;			// Timer to increment towards shuttle_time_to_spawn
	f64					shuttle_time_to_spawn;	// Randomized value based on range from SHUTTLE_SPAWN_TIME_MAX & SHUTTLE_SPAWN_TIME_MIN
// SHUTTLE VARIABLES-----------------

// DEBRIS VARIABLES------------------
	std::vector<Debris> debris_vector;			// Vector for debris for current planet
	int					max_debris;				// Current maximum number of debris for the planet
	int					current_debris;			// Current number of debris for the planet
// DEBRIS VARIABLES------------------

// DRONES VARIABLES------------------
	int					current_drones;			// Current nubmer of drones on the planet
// DRONES VARIABLES------------------

	void load();
	void init();
	void update(f64 frame_time);
	void draw(AEGfxVertexList* pMesh);
	void free();
	void unload();

	void spawn(int shuttle_randomize_amount);
	void check_spawn(Planets& new_planet);
	f32 get_max_x();
	f32 get_max_y();
	f32 get_min_x();
	f32 get_min_y();
};
