#pragma once
#include "pch.h"


struct Debris
{
	int id;
	AEVec2 position;
	f32 scale_x;
	f32 scale_y;
	int angle;
	f32 turning_angle;
	f32 turning_speed;
	double dist_from_planet;
	double speed;
	double is_collide;
	double is_collect;
	double texture; // maybe 3-5 textures
};

namespace debris
{
	void load();
	void init(f32 planet_x_pos, f32 planet_y_pos, float scale_x, float scale_y, double max_debris);
	void update(f64 frame_time);
	void draw(AEGfxVertexList* pMesh);
	void free();
	void unload();
}
