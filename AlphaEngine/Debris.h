/******************************************************************************/
/*!
\file		Debris.h
\author 	Leong Wen Bin, Aceson
\co_author	Hwang Jing Rui, Austin
\par    	email: leong.w\@digipen.edu
				   jingruiaustin.hwang\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the declaration of functions for the Debris.

			Austin - Added debris state and behaviour in each state

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/
#pragma once
#include <vector>

struct Explosion {
	AEVec2	position;
	AEMtx33 transform;
	f32		width;
	f32		height;
	f32		timer = 0;
	f32		total_time = 1;
	int		is_draw = 0;
};

enum MoveState {
	ORBIT_AROUND_PLANET = 0,
	MOVE_TOWARDS_PLANET,
	MOVE_TOWARDS_PLAYER,
	MOVE_TOWARDS_DRONE
};

struct Debris
{
	int id;
	AEVec2 position;
	f32	size;
	AEMtx33 scale;
	AEMtx33 rotate;
	AEMtx33 translate;
	AEMtx33 transform;
	f32 angle;
	f32 turning_angle;
	f32 turning_speed;
	f32 distance;
	double speed;
	double is_collide;
	double is_collect;
	double texture; // maybe 3-5 textures
	bool active;
	MoveState state;
	bool to_erase = false;
	int move_towards_drone_id = 0;
	Explosion explosion;

	void load();
	void init();
	void update(f32 frame_time);
	void draw(AEGfxVertexList* pMesh);
	void free();
	void unload();

	std::vector<Debris> create_debris(f32 planet_x, f32 planet_y, double size, int total_debris);
};

extern std::vector<Debris> debris_vector;
void spawn_debris_shuttle(AEVec2 position, int planet_id, int num_of_debris);
std::vector<Debris> MM_create_debris(f32 planet_x, f32 planet_y, double size, int total_debris);
void MMspawn_debris_shuttle(AEVec2 position, int num_of_debris);
