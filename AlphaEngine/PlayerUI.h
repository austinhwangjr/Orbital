#pragma once
#include "Player.h"

struct PlayerUI
{
	bool drone_active;
	bool placing_drone;
	bool placing_station;

	void load();
	void init();
	void update(f64, Player&);
	void draw(AEGfxVertexList*, Player&);
	void free();
	void unload();
};