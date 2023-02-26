#pragma once
#include "Player.h"

enum BUTTON_TYPE 
{
	MOVEMENT_SPEED = 0,
	CAPACITY,
	TRACTOR_BEAM_STRENGTH,
	CREATE_DRONE,
	SPACE_STATION,
	UPGRADE_COUNT,
	SHOP_OPEN,
};

struct UpgradeLevelIndicator
{
	AEVec2	position;
	AEMtx33 transform;
	f32		width;
	f32		height;
};

struct ShopOption
{
	AEVec2	position;
	AEMtx33	transform;
	f32		width;
	f32		height;
	int		button_type;
	std::vector<UpgradeLevelIndicator> indicator_vector;
};

struct PlayerUI
{
	bool			placing_drone;
	bool			placing_station;
	bool			shop_triggered;
	AEMtx33			transform;

	// Cost of upgrades
	const int mov_speed_cost		= 100;
	const int capacity_cost			= 200;
	const int beam_strength_cost	= 150;
	const int drone_cost			= 250;
	const int space_station_cost	= 300;


	void load();
	void init();
	void update(Player&);
	void draw(AEGfxVertexList*, Player&);
	void free();
	void unload();
	
	void shop_open(Player&);
	void shop_closed();
	bool button_clicked(ShopOption);
};