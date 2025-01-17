/******************************************************************************/
/*!
\file		PlayerUI.h
\author 	Hwang Jing Rui, Austin (60%)
\co-author	Tan Rui An Ryan (35%)
			Leong Wen Bin, Aceson (2.5%)
			Ong You Yang (2.5%)
\par    	email: jingruiaustin.hwang\@digipen.edu
				   ruianryan.t\@digipen.edu
				   leong.w\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the declarations of functions for the player
			UI (user interface).

			Ryan - All tutorial elements

			Aceson - File I/O

			You Yang - Audio-related aspects

All content (C) 2023 DigiPen Institute of Technology Singapore. All rights reserved.
 */
 /******************************************************************************/
#pragma once
#include "Player.h"
#include "WaveManager.h"

#define DEBRIS_VALUE 10
#define DEBRIS_SCORE 100

enum BUTTON_TYPE 
{
	SHOP_OPEN = 0,
	MOVEMENT_SPEED,
	CAPACITY,
	TRACTOR_BEAM_STRENGTH,
	CREATE_DRONE,
	SPACE_STATION,
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
	// ==========
	// Player UI
	// ==========
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

	// ===========
	// Player HUD
	// ===========
	AEVec2		player_hud_position;
	f32			player_hud_width, player_hud_height;
	AEMtx33		player_hud_transform;

	// ===========
	// Shop Icons
	// ===========
	f32			icon_size;
	AEMtx33		drone_icon_transform;
	AEMtx33		space_station_icon_transform;

	// ================
	// Upgrade Preview
	// ================
	AEVec2		upgrade_preview_position;
	f32			upgrade_preview_size;
	AEMtx33		upgrade_preview_transform;

	// ================
	// Shop Background
	// ================
	AEVec2		shop_bg_position;
	f32			shop_bg_width, shop_bg_height;
	AEMtx33		shop_bg_transform;

	// ====================
	// Tutorial Background
	// ====================
	AEVec2		tutorial_bg_position;
	f32			tutorial_bg_width, tutorial_bg_height;
	AEMtx33		tutorial_bg_transform;

	// =================
	// Cost of Upgrades
	// =================
	const int mov_speed_cost		= 100;
	const int capacity_cost			= 200;
	const int beam_strength_cost	= 150;
	const int drone_cost			= 250;
	const int space_station_cost	= 500;

	// =====================
	// Shuttle Lost Overlay
	// =====================
	AEMtx33		lost_overlay_transform;
	f32			lost_overlay_timer;

	// ===============
	// Shop Indicator
	// ===============
	AEMtx33		shop_indicator_transform;
	f32			shop_indicator_width, shop_indicator_height;
	f32			shop_indicator_timer;
	f32			shop_indicator_speed;
	bool		clicked_on_shop;

	// ==========
	// Functions
	// ==========
	void load();
	void init();
	void update(Player&, WaveManager const&);
	void draw(AEGfxVertexList*, WaveManager const&);
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
	void tutorial(WaveManager const&);
};