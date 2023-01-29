#pragma once
#define max_planet 10
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
	//double list[i];	//isnt using int btr?
	std::vector<Debris> debris_vector; //dynamic array of debris
	int id;
	AEVec2 position;
	AEMtx33 scale;
	AEMtx33 rotate;
	AEMtx33 translate;
	AEMtx33 transform;
	AEVec2 shuttle_spawn_pos;
	f64 shuttle_spawn_timer;
	f64 shuttle_time_spawn;
	int max_shuttle;
	int current_shuttle;
	int max_debris;
	double current_debris;
	double max_drones;
	double current_drones;
	bool complete;

	void load();
	void init();
	void update(f64 elapsed_time);
	void draw(AEGfxVertexList* pMesh);
	void free();
	void unload();

	void spawn();
};

extern std::vector<Planets> planet_vector;
extern AEGfxTexture* planet_tex;
