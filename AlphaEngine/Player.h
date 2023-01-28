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
	AEVec2			direction;
	PLAYER_STATES	state;
	double			size;
	f32				dist_from_planet;
	double			speed;
	f32				angle;
	//double free; // what is free
	double			beam_str;
	double			beam_width;
	Planets			current_planet;
	int				current_capacity; // probably can shorten
	double			max_capacity;
	double			texture;
	double			velocity;


	void load();
	void init();
	void update(f64 frame_time);
	void draw(AEGfxVertexList* pMesh);
	void free();
	void unload();
};

// e.g
//player.x_pos = screen / 2;
//player.y_pos = screen / 2;