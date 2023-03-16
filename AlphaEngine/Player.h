#pragma once
#define MAX_MOV_SPEED_LVL		5
#define MAX_CAPACITY_LVL		5
#define MAX_SPACE_STATION_CNT	5
#define MAX_BEAM_STRENGTH_LVL	5

#include "Planet.h"

enum PLAYER_STATES
{
	PLAYER_ORBIT = 0,
	PLAYER_FLY
};

struct Player
{
	//--------------------Player--------------------
	PLAYER_STATES	state;
	AEVec2			position;
	AEVec2			velocity;
	f32				size;
	f32				mov_speed;
	f32				rot_speed;
	Planets			current_planet;
	f32				shortest_distance;
	f32				direction;
	int				current_capacity;
	int				max_capacity;
	int				score;
	int				credits;
	bool			can_leave_orbit;

	//--------------------Upgrade Levels--------------------
	int				mov_speed_level;
	int				capacity_level;
	int				space_station_count;
	int				beam_level;

	//--------------------Tractor Beam--------------------
	AEVec2			beam_pos;
	f32				beam_str;
	f32				beam_width;
	f32				beam_height;

	//--------------------Transforms--------------------
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