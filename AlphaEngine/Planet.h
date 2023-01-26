#pragma once
#define max_planet 10

struct Planets
{
	//double list[i];	//isnt using int btr?

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
	double max_debris;
	double current_debris;
	double max_drones;
	double current_drones;
	double texture;
};

extern Planets* planet_array;

namespace planet
{
	void load();
	void init();
	void update(f64 elapsed_time);
	void draw(AEGfxVertexList* pMesh);
	void free();
	void unload();
}
