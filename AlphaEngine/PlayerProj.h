/******************************************************************************/
/*!
\file		PlayerProj.cpp
\author 	Hwang Jing Rui, Austin
\par    	email: jingruiaustin.hwang\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the declaration of functions for the player 
			projectile.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
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
	void update(f64, Player&, PlayerUI&);
	void draw(AEGfxVertexList*);
	void free();
	void unload();
};
