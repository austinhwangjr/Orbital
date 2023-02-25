#pragma once

#define PLANET_SIZE 200.0f
#define TIME_TO_SPAWN 8
#define SHUTTLE_SPAWN_TIME_MIN 4
#define DEBRIS_MAX 20
#define DEBRIS_MIN 10

#include "Debris.h"
#include <vector>

enum PLANET_ID
{
	PLANET_0 = 0,
	PLANET_1,
	PLANET_2,
	PLANET_3,
	PLANET_4,
	PLANET_5,
	PLANET_6,
	PLANET_7,
	PLANET_8,
	PLANET_9
};

struct Planets
{
// PLANET VARIABLES------------------
	int					id;
	AEVec2				position;
	AEMtx33				scale;
	AEMtx33				rotate;
	AEMtx33				translate;
	AEMtx33				transform;
	double				size;
	bool				wave_complete;
// PLANET VARIABLES------------------

// SHUTTLE VARIABLES-----------------
	int					max_shuttle;
	int					current_shuttle;
	AEVec2				shuttle_spawn_pos;
	f64					shuttle_timer;
	f64					shuttle_time_to_spawn;
// SHUTTLE VARIABLES-----------------

// DEBRIS VARIABLES------------------
	std::vector<Debris> debris_vector; //dynamic array of debris
	int					max_debris;
	int					current_debris;
// DEBRIS VARIABLES------------------

// DRONES VARIABLES------------------
	int					max_drones;
	int					current_drones;
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
