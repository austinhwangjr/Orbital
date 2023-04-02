/******************************************************************************/
/*!
\file		SpaceStation.h
\author 	Hwang Jing Rui, Austin (50%)
\co-author	Leong Wen Bin Aceson (50%)
\par    	email: jingruiaustin.hwang\@digipen.edu
				   leong.w\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the declaration of functions for the space station.

			Aceson	- Added cooldown bar and coin for space stations.
					- Added check so that space stations cannot be too close to
					  each other, to prevent overlap
					- Added collision check between space station and Nearest planet

			Austin 	- Integrated UI and player elements 

All content � 2023 DigiPen Institute of Technology Singapore. All rights reserved.
 */
/******************************************************************************/
#pragma once
#include "PlayerUI.h"

#define SPACE_STATION_BUFFER 

struct SpaceStation
{
	//--------------------Space Station--------------------
	AEVec2			position;
	f32				size;
	int				current_capacity;
	int				max_capacity;
	Planets			current_planet;
	f32				shortest_distance_spacestation;
	bool			initial_spawn;

	//--------------------Transform--------------------
	AEMtx33			transform;

	void load();
	void init();
	void update(Player&, PlayerUI&);
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
