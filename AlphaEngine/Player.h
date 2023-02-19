#pragma once
#include "Planet.h"

enum PLAYER_STATES
{
	PLAYER_ORBIT = 0,
	PLAYER_FLY
};

struct Player
{
	// Player
	PLAYER_STATES	state;
	AEVec2			position;
	AEVec2			velocity;
	f32				size;
	f32				mov_speed;
	f32				rot_speed;
	f32				speed_upgrade;
	Planets			current_planet;
	f32				dist_from_planet;
	f32				shortest_distance;
	f32				direction;
	int				current_capacity;
	int				max_capacity;
	bool			can_leave_orbit;

	// Tractor beam
	AEVec2			beam_pos;
	AEVec2			beam_collision_pos;
	f32				beam_str;
	f32				beam_width;

	// Transforms
	AEMtx33			player_transform;
	AEMtx33			beam_transform;

	void load();
	void init();
	void update(f64);
	void draw(AEGfxVertexList*);
	void free();
	void unload();

	void orbit_state(f64);
	void flying_state(f64);
};