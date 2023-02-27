#pragma once
#include "PlayerUI.h"

struct Drone_Bar {
	AEVec2	position;
	AEMtx33 transform;
	f32		width;
	f32		max_width;
	f32		height;
	f32		timer;
	f32		total_time;
};

struct Drone
{
	//--------------------Drone--------------------
	AEVec2			position;
	AEVec2			velocity;
	f32				size;
	f32				rot_speed;
	Planets			current_planet;
	f32				dist_from_planet;
	f32				shortest_distance;
	f32				direction;
	int				current_capacity;
	int				max_capacity;

	//--------------------Cooldown Bar--------------------
	Drone_Bar		cd_bar;

	//--------------------Tractor Beam--------------------
	AEVec2			beam_pos;
	f32				beam_str;
	f32				beam_width;
	f32				beam_height;

	//--------------------Transform--------------------
	AEMtx33			drone_transform;
	AEMtx33			beam_transform;

	void load();
	void init(Player);
	void update(f64, Player&, PlayerUI&);
	void draw(AEGfxVertexList*, PlayerUI);
	void free();
	void unload();
};