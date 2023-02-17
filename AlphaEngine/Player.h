#pragma once
#include "Planet.h"

enum PLAYER_STATES
{
	PLAYER_ORBIT = 0,
	PLAYER_FLY
};

struct Player
{
	AEVec2			position;
	AEVec2			velocity;
	PLAYER_STATES	state;
	f32				size;
	f32				dist_from_planet;
	f32				shortest_distance;
	f32				mov_speed;
	f32				rot_speed;
	f32				speed_upgrade;
	f32				direction;
	Planets			current_planet;
	int				current_capacity; // probably can shorten
	int				max_capacity;

	//double			beam_str;
	//double			beam_width;
	AEVec2			beam_pos;

	void load();
	void init();
	void update(f64);
	void draw(AEGfxVertexList*);
	void free();
	void unload();
};