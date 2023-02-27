#pragma once
#define SHUTTLE_MAX_LIFESPAN	5.f			// Maximum life time for a shuttle before escaping (expiring)
#define SHUTTLE_MAX_ACCEL		100.f		// Maximum acceleration for a shuttle
#define SHUTTLE_VALUE			2			// Credit value for a shuttle

#include "Player.h"
#include <vector>

struct Shuttles
{
	AEMtx33		scale;
	AEMtx33		rotate;
	AEMtx33		translate;
	AEMtx33		transform;
	AEVec2		direction;
	AEVec2		velocity;
	AEVec2		position;
	f32			acceleration;
	f32			lifespan;		// Current life time for a shuttle
	int			planet_id;		// ID for planet shuttle spawns from
	bool		active;			// Flag for whether the shuttle is currently active

	void load();
	void init();
	void update(f64 frame_time, Player& player);
	void draw(AEGfxVertexList* pMesh);
	void free();
	void unload();
	
	void spawn(int const& planet_id);
};
