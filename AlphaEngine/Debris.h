#pragma once
#include "pch.h"

struct Debris
{
	int id;
	AEVec2 position;
	f32 scale_x;
	f32 scale_y;
	f32 angle;
	f32 turning_angle;
	f32 turning_speed;
	double dist_from_planet;
	double speed;
	double is_collide;
	double is_collect;
	double texture; // maybe 3-5 textures
	AEMtx33 scale;
	AEMtx33 rotate;
	AEMtx33 translate;
	AEMtx33 transform;


	void load();
	void init();
	void update(f64 frame_time);
	void draw(AEGfxVertexList* pMesh);
	void free();
	void unload();
};

