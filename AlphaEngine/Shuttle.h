/******************************************************************************/
/*!
\file		Shuttle.h
\author		Tan Rui An Ryan (100%)
\par		email: ruianryan.t\@digipen.edu
\date		March 28, 2023
\brief		This file contains the declaration of functions for the shuttle.

All content (C) 2023 DigiPen Institute of Technology Singapore. All rights reserved.
 */
/******************************************************************************/
#pragma once
#include "Player.h"
#include <vector>

struct Shuttles
{
	AEMtx33		scale;
	AEMtx33		rotate;
	AEMtx33		translate;
	AEMtx33		transform;
	AEVec2		direction;
	AEVec2		velocity;
	AEVec2		position;
	f32			acceleration;
	f32			lifespan;		// Current life time for a shuttle
	int			planet_id;		// ID for planet shuttle spawns from
	bool		active;			// Flag for whether the shuttle is currently active
	int blastOffSoundID;

	void load();
	void init();
	void update(Player& player);
	void draw(AEGfxVertexList* pMesh);
	void free();
	void unload();
	
	void spawn(int const& current_planet_id, f32 const& rand_angle);
};

void MMspawn_shuttle();
void MMupdate_shuttle();
