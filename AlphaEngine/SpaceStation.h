#pragma once
#include "PlayerUI.h"

struct SpaceStation
{
	AEVec2			position;
	f32				size;
	int				current_capacity;
	int				max_capacity;
	Planets			current_planet;
	f32				shortest_distance_spacestation;
	// Transform
	AEMtx33			transform;

	void load();
	void init();
	void update(f64, Player&, PlayerUI&);
	void draw(AEGfxVertexList*, PlayerUI);
	void free();
	void unload();
};