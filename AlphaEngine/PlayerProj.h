#pragma once
#include "PlayerUI.h"

struct PlayerProj
{
	AEVec2			position;
	AEVec2			velocity;
	f32				size;
	f32				speed;
	f32				direction;

	// Transform
	AEMtx33			transform;

	void load();
	void init(Player);
	void update(f64, Player&);
	void draw(AEGfxVertexList*);
	void free();
	void unload();
};
