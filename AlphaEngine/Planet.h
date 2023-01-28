#pragma once
#define max_planet 10
#include "Debris.h"
#include <vector>

struct Planets
{
	//double list[i];	//isnt using int btr?
	Debris* debris_arr; //dynamic array of debris
	AEMtx33* debris_draw_array;
	double name; // might not need
	AEVec2 position;
	AEMtx33 scale;
	AEMtx33 rotate;
	AEMtx33 translate;
	AEMtx33 transform;
	AEVec2 shuttle_spawn_pos;
	double shuttle_timer; // shuttle countdown time displayed on planet
	double max_shuttle;
	double current_shuttle;
	int max_debris;
	double current_debris;
	double max_drones;
	double current_drones;
	double texture;

	void load();
	void init();
	void update(f64 elapsed_time);
	void draw(AEGfxVertexList* pMesh);
	void free();
	void unload();
};

extern std::vector<Planets> planet_vector;
extern AEGfxTexture* planet_tex;


Debris* create_debris(Planets planet, int total_debris);
AEMtx33* draw_debris(Planets planet, int total_debris);
