#pragma once
#include "PlayerUI.h"

struct Drone
{
	AEVec2			position;
	AEVec2			velocity;
	f32				size;
	f32				dist_from_planet;
	f32				shortest_distance;
	//f32				mov_speed;
	f32				rot_speed;
	//f32				speed_upgrade;
	f32				direction;
	double			beam_str;
	double			beam_width;
	Planets			current_planet;
	int				current_capacity; // probably can shorten
	int				max_capacity;
	//bool			active;

	AEVec2			beam_pos;

	void load();
	void init(Player);
	void update(f64, Player, PlayerUI&);
	void draw(AEGfxVertexList*, PlayerUI);
	void free();
	void unload();
};