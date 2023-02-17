#pragma once
#include <vector>

struct Debris
{
	int id;
	AEVec2 position;
	f32 scale_x;
	f32 scale_y;
	AEMtx33 scale;
	AEMtx33 rotate;
	AEMtx33 translate;
	AEMtx33 transform;
	f32 angle;
	f32 turning_angle;
	f32 turning_speed;
	double dist_from_planet;
	double speed;
	double is_collide;
	double is_collect;
	double texture; // maybe 3-5 textures
	bool active;

	void load();
	void init();
	void update(f64 frame_time);
	void draw(AEGfxVertexList* pMesh);
	void free();
	void unload();

	std::vector<Debris> create_debris(f32 planet_x, f32 planet_y, double size, int total_debris);
};

extern std::vector<Debris> debris_vector;
void spawn_debris(int num_of_debris, int planet_id);
AEVec2 distance_from_radius(AEVec2 planet_radius, AEVec2 position, int planet_id);