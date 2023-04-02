/******************************************************************************/
/*!
\file		PlayerProj.h
\author 	Hwang Jing Rui, Austin (100%)
\par    	email: jingruiaustin.hwang\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the declaration of functions for the player 
			projectile.

All content (C) 2023 DigiPen Institute of Technology Singapore. All rights reserved.
 */
/******************************************************************************/
#pragma once
#include "PlayerUI.h"

struct PlayerProj
{
	// ==================
	// Player Projectile
	// ==================
	AEVec2			position;
	AEVec2			velocity;
	f32				size;
	f32				speed;
	f32				direction;
	int				is_delete;

	// ==========
	// Transform
	// ==========
	AEMtx33			transform;

	// ==========
	// Functions
	// ==========
	void load();
	void init();
	void update(Player&, PlayerUI&);
	void draw(AEGfxVertexList*);
	void free();
	void unload();
};
