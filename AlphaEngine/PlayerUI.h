#pragma once
#include "Player.h"

#define DEBRIS_VALUE 10
#define DEBRIS_SCORE 100

enum BUTTON_TYPE 
{
	MOVEMENT_SPEED = 0,
	CAPACITY,
	TRACTOR_BEAM_STRENGTH,
	CREATE_DRONE,
	SPACE_STATION,
	UPGRADE_COUNT,
	SHOP_OPEN,
	TUTORIAL_OPEN
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
	//--------------------Player UI--------------------
	bool		placing_drone;
	bool		placing_station;
	bool		shop_triggered;
	bool		shop_transition;
	bool		tutorial_triggered;
	bool		tutorial_transition;
	bool		drone_placement_flag;
	bool		station_placement_flag;
	f32			shop_offset;
	f32			shop_trans_timer;
	f32			shop_trans_duration;
	f32			tutorial_offset;
	f32			tutorial_trans_timer;
	f32			tutorial_trans_duration;
	AEMtx33		transform;

	//--------------------Player HUD--------------------
	AEVec2		player_hud_position;
	f32			player_hud_width, player_hud_height;
	AEMtx33		player_hud_transform;

	//--------------------Icons in shop--------------------
	f32			icon_size;
	AEMtx33		drone_icon_transform;
	AEMtx33		space_station_icon_transform;

	AEVec2		upgrade_preview_position;
	f32			upgrade_preview_size;
	AEMtx33		upgrade_preview_transform;
	bool		upgrade_preview_display;

	//--------------------Shop background--------------------
	AEVec2		shop_bg_position;
	f32			shop_bg_width, shop_bg_height;
	AEMtx33		shop_bg_transform;

	//--------------------Tutorial background--------------------
	AEVec2		tutorial_bg_position;
	f32			tutorial_bg_width, tutorial_bg_height;
	AEMtx33		tutorial_bg_transform;

	//--------------------Cost of Upgrades--------------------
	const int mov_speed_cost		= 100;
	const int capacity_cost			= 200;
	const int beam_strength_cost	= 150;
	const int drone_cost			= 250;
	const int space_station_cost	= 500;

	void load();
	void init();
	void update(f64, Player&);
	void draw(AEGfxVertexList*, Player);
	void free();
	void unload();
	
	void shop_open(Player&);
	void shop_closed();
	void close_shop();
	bool button_clicked(ShopOption);
	bool click_outside_shop();
	bool hover_over_button(ShopOption);

	void tutorial_open();
	void tutorial_closed();
	void close_tutorial();
	bool click_outside_tutorial();
};