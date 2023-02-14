#pragma once
#include "AEEngine.h"
#include <vector>

struct Shuttles
{
	AEVec2 position;
	AEVec2 vector;
	AEMtx33 scale;
	AEMtx33 rotate;
	AEMtx33 translate;
	AEMtx33 transform;
	f64 lifespan;
	double speed;
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

extern std::vector<Shuttles> shuttle_vector;
extern AEGfxTexture* shuttle_tex;
