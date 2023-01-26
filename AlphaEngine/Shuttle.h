#pragma once
#include "AEEngine.h"

struct Shuttles
{
	double x_pos;
	double y_pos;
	double size;
	double speed;
	double vector;
	double value;
	double lifespan;
	double above_below; //random choice to shrink or increase size when passing by other planet
	double texture;

	void load();
	void init();
	void update(f64 frame_time);
	void draw(AEGfxVertexList* pMesh);
	void free();
	void unload();
};