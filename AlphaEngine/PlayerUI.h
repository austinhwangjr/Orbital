#pragma once
#include "Player.h"

struct PlayerUI
{
	bool			placing_drone;
	bool			placing_station;
	AEMtx33			transform;

	void load();
	void init();
	void update(Player&);
	void draw(AEGfxVertexList*, Player&);
	void free();
	void unload();
	
	void shop_open(Player&);
	void shop_closed();
};

enum BUTTON_TYPE {
	SHOP_OPEN = 0,
	MOVEMENT_SPEED,
	CAPACITY,
	CREATE_DRONE,
	SPACE_STATION,
	TRACTOR_BEAM_STRENGTH
};

struct ShopOption
{
	AEVec2	position;
	AEMtx33 transform;
	f32		size;
	int		button_type;
};