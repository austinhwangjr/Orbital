#pragma once

#define TIME_TO_SPAWN 5
#define MIN_SHUTTLE_TIME 3

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
	std::vector<Debris> debris_vector; //dynamic array of debris
	int id;
	AEVec2 position;
	AEMtx33 scale;
	AEMtx33 rotate;
	AEMtx33 translate;
	AEMtx33 transform;
	AEVec2 shuttle_spawn_pos;
	f64 shuttle_timer;
	f64 shuttle_time_to_spawn;
	double size;
	int max_shuttle;
	int current_shuttle;
	int max_debris;
	int current_debris;
	int max_drones;
	int current_drones;
	bool wave_complete;

	void load();
	void init();
	void update(f64 elapsed_time);
	void draw(AEGfxVertexList* pMesh);
	void free();
	void unload();

	void spawn(int shuttle_randomize_amount);
};
