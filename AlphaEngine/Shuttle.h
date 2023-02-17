#pragma once

#define SHUTTLE_MAX_LIFESPAN 3.f
#define SHUTTLE_MAX_ACCEL 100.f

#include <vector>

struct Shuttles
{
	AEVec2 position;
	AEVec2 direction;
	AEVec2 velocity;
	AEMtx33 scale;
	AEMtx33 rotate;
	AEMtx33 translate;
	AEMtx33 transform;
	f64 lifespan;
	double acceleration;
	double value;
	bool active;
	int planet_id;

	void load();
	void init();
	void update(f64 frame_time);
	void draw(AEGfxVertexList* pMesh);
	void free();
	void unload();
	
	void spawn(int planet_id);
};
