#pragma once
#include "PlayerUI.h"

struct PlayerProj
{
	//--------------------Player Projectile--------------------
	AEVec2			position;
	AEVec2			velocity;
	f32				size;
	f32				speed;
	f32				direction;
	int				is_delete;

	//--------------------Transform--------------------
	AEMtx33			transform;

	void load();
	void init();
	void update(f64, Player&, PlayerUI&);
	void draw(AEGfxVertexList*);
	void free();
	void unload();
};
