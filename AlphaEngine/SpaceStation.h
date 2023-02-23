#pragma once
#include "PlayerUI.h"

#define SPACE_STATION_BUFFER 

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

struct Cooldown_Bar {
	AEVec2	position;
	AEMtx33 transform;
	f32		width;
	f32		height;
	f32		timer;
	f32		total_time;
};

struct Coin {
	AEVec2	position;
	AEMtx33 transform;
	f32		width;
	f32		height;
	int		is_draw;
	f32		lifespan;
};
