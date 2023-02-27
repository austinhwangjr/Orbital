#pragma once
#include "PlayerUI.h"

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