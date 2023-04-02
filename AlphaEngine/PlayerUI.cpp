/******************************************************************************/
/*!
\file		PlayerUI.cpp
\author 	Hwang Jing Rui, Austin
\co-author	Tan Rui An Ryan
\par    	email: jingruiaustin.hwang\@digipen.edu
					ruianryan.t\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the definition of functions for the player
			UI (user interface).

			Ryan - All tutorial elements

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include <iostream>
#include <vector>
#include <string>
#include "AEEngine.h"
#include "PlayerUI.h"
#include "SpaceStation.h"
#include "Global.h"
#include "Easing.h"
#include "Data.h"
#include "Input.h"

// General UI textures
extern AEGfxTexture* player_tex;
AEGfxTexture* shop_icon_tex;
AEGfxTexture* shop_open_tex;
AEGfxTexture* shop_close_tex;
AEGfxTexture* space_station_tex;
AEGfxTexture* shop_background_tex;
AEGfxTexture* tutorial_open_tex;
AEGfxTexture* tutorial_background_tex;
AEGfxTexture* upgrade_level_hollow_tex;
AEGfxTexture* upgrade_level_solid_tex;
AEGfxTexture* player_hud_tex;
AEGfxTexture* shuttle_lost_tex;
AEGfxTexture* shop_indicator_tex;

// Upgrade preview textures
AEGfxTexture* speed_hover_tex;
AEGfxTexture* capacity_hover_tex;
AEGfxTexture* strength_hover_tex;

// Shop button textures
AEGfxTexture* mov_speed_button_tex;
AEGfxTexture* capacity_button_tex;
AEGfxTexture* strength_button_tex;
AEGfxTexture* drone_button_tex;
AEGfxTexture* space_station_button_tex;

// Variables for general UI
std::string	  score, credits, capacity;
extern s8	  font_id;
static int MAX_MOV_SPEED_LVL;
static int MAX_CAPACITY_LVL;
static int MAX_SPACE_STATION_CNT;
static int MAX_BEAM_STRENGTH_LVL;
static int UPGRADE_COUNT;
static float LOST_OVERLAY_TIME;
static int HUD_BUFFER;

// Variables for shop
extern s8	  font_id_shop;

// Vector of buttons and space stations
std::vector<ShopOption> button_vector;
extern std::vector<SpaceStation> space_station_vector;

// IMPORT DATA VECTOR
std::map < std::string, f32> 	GameUIDataMap;
std::vector<Data> 				GameUIData;

bool shop_sound_played = false;
bool shop_close_sound_played = false;
/******************************************************************************/
/*!
	Load Textures and Data
*/
/******************************************************************************/
void PlayerUI::load()
{
	// Load general UI textures
	shop_icon_tex			 = AEGfxTextureLoad("Assets/MainLevel/ml_YellowButtonBackground.png");
	shop_open_tex			 = AEGfxTextureLoad("Assets/MainLevel/ml_ShopOpenButton.png");
	shop_close_tex			 = AEGfxTextureLoad("Assets/MainLevel/ml_ShopCloseButton.png");
	space_station_tex		 = AEGfxTextureLoad("Assets/MainLevel/ml_SpaceStation.png");
	shop_background_tex		 = AEGfxTextureLoad("Assets/MainLevel/ml_ShopBackground.png");
	tutorial_open_tex		 = AEGfxTextureLoad("Assets/MainLevel/ml_TutorialBackground.png");
	tutorial_background_tex  = AEGfxTextureLoad("Assets/MainLevel/ml_TutorialBackground.png");
	upgrade_level_hollow_tex = AEGfxTextureLoad("Assets/MainLevel/ml_UpgradeLevelHollow.png");
	upgrade_level_solid_tex	 = AEGfxTextureLoad("Assets/MainLevel/ml_UpgradeLevelSolid.png");
	player_hud_tex			 = AEGfxTextureLoad("Assets/MainLevel/ml_HeadsUpDisplay.png");
	shuttle_lost_tex		 = AEGfxTextureLoad("Assets/MainLevel/neonCircle.png");
	shop_indicator_tex		 = AEGfxTextureLoad("Assets/MainLevel/neonCircle.png");

	// Upgrade preview textures
	speed_hover_tex = AEGfxTextureLoad("Assets/MainLevel/ml_MovSpeedUpgradePreview.png");
	capacity_hover_tex = AEGfxTextureLoad("Assets/MainLevel/ml_CapacityUpgradePreview.png");
	strength_hover_tex = AEGfxTextureLoad("Assets/MainLevel/ml_BeamStrengthUpgradePreview.png");

	// Shop button textures
	mov_speed_button_tex = AEGfxTextureLoad("Assets/MainLevel/ml_MovSpeedUpgradeButton.png");
	capacity_button_tex = AEGfxTextureLoad("Assets/MainLevel/ml_CapacityUpgradeButton.png");
	strength_button_tex = AEGfxTextureLoad("Assets/MainLevel/ml_BeamStrengthUpgradeButton.png");
	drone_button_tex = AEGfxTextureLoad("Assets/MainLevel/ml_DroneButton.png");
	space_station_button_tex = AEGfxTextureLoad("Assets/MainLevel/ml_SpaceStationButton.png");

	//DATA
	ImportDataFromFile("Assets/GameObjectData/GameUserInterfaceData.txt", GameUIData, GameUIDataMap);
}

/******************************************************************************/
/*!
	Initialize Variables
*/
/******************************************************************************/
void PlayerUI::init()
{
	MAX_MOV_SPEED_LVL			= static_cast<int>(GameUIDataMap["Maximum_Movement_Speed_Level"]);
	MAX_CAPACITY_LVL			= static_cast<int>(GameUIDataMap["Maximum_Capacity_Level"]);
	MAX_SPACE_STATION_CNT		= static_cast<int>(GameUIDataMap["Maximum_Space_Station_Level"]);
	MAX_BEAM_STRENGTH_LVL		= static_cast<int>(GameUIDataMap["Maximum_Beam_Strength_Level"]);
	UPGRADE_COUNT				= static_cast<int>(GameUIDataMap["Upgrade_Count"]);
	LOST_OVERLAY_TIME			= GameUIDataMap["Lost_Overlay_Time"];
	HUD_BUFFER					= static_cast<int>(GameUIDataMap["HUD_Buffer"]);

	// Player UI
	player_hud_width = g_windowWidth + HUD_BUFFER;
	player_hud_height = g_windowHeight + HUD_BUFFER;

	// Add buttons to vector
	ShopOption shop_open_button{};
	shop_open_button.width = GameUIDataMap["Shop_Open_Button_Width"];
	shop_open_button.height = GameUIDataMap["Shop_Open_Button_Height"];
	shop_open_button.button_type = SHOP_OPEN;
	button_vector.push_back(shop_open_button);

	for (int i = 0; i < UPGRADE_COUNT; ++i) {
		// Populate button vector
		ShopOption player_upgrade{};
		player_upgrade.width = GameUIDataMap["Player_Upgrade_Button_Width"];
		player_upgrade.height = GameUIDataMap["Player_Upgrade_Button_Height"];
		player_upgrade.button_type = MOVEMENT_SPEED + i;

		// Max upgrade levels for each upgrade type
		int count{};
		switch (player_upgrade.button_type) {
		case MOVEMENT_SPEED:
			count = MAX_MOV_SPEED_LVL;
			break;
		case CAPACITY:
			count = MAX_CAPACITY_LVL;
			break;
		case TRACTOR_BEAM_STRENGTH:
			count = MAX_BEAM_STRENGTH_LVL;
			break;
		case SPACE_STATION:
			count = MAX_SPACE_STATION_CNT;
			break;
		}

		// Add indicators (upgrade levels)
		for (int j = 0; j < count; ++j) {
			UpgradeLevelIndicator indicator{};
			indicator.width = GameUIDataMap["Upgrade_Indicator_Width"];
			indicator.height = GameUIDataMap["Upgrade_Indicator_Height"];
			player_upgrade.indicator_vector.push_back(indicator);
		}

		button_vector.push_back(player_upgrade);
	}

	ShopOption tutorial_button{};
	tutorial_button.width = GameUIDataMap["Tutorial_Button_Width"];
	tutorial_button.height = GameUIDataMap["Tutorial_Button_Height"];
	tutorial_button.button_type = TUTORIAL_OPEN;
	button_vector.push_back(tutorial_button);

	// Not in placing mode initially
	placing_drone = false;
	placing_station = false;
	drone_placement_flag = false;
	station_placement_flag = false;

	// Shop is closed initially
	shop_triggered = false;
	shop_transition = false;

	// Shop background
	shop_bg_width = g_windowWidth * 0.85f;
	shop_bg_height = g_windowHeight * 0.85f;

	// Tutorial is opened initially
	tutorial_triggered = true;
	tutorial_transition = false;

	// Tutorial background
	tutorial_bg_width = static_cast<f32>(12 * FONT_ID_SIZE);
	tutorial_bg_height = static_cast<f32>(10 * FONT_ID_SIZE);

	// Set the offset of the shop
	shop_offset = g_windowWidth;

	// Set the offset of the tutorial
	tutorial_offset = GameUIDataMap["Tutorial_OffSet"];

	// Icons in shop
	icon_size = GameUIDataMap["Shop_Icon_Size"];

	// Set the size of the upgrade preview
	upgrade_preview_size = GameUIDataMap["Upgrade_Preview_Size"];

	// Timer for shop transition
	shop_trans_timer = GameUIDataMap["Shop_Transition_Timer"];
	shop_trans_duration = GameUIDataMap["Shop_Transition_Duration"];

	// Timer for tutorial transition
	tutorial_trans_timer = GameUIDataMap["Tutorial_Transition_Timer"];
	tutorial_trans_duration = GameUIDataMap["Tutorial_Transition_Duration"];

	// Timer for shuttle lost overlay
	lost_overlay_timer = GameUIDataMap["Lost_Overlay_Timer"];

	// Shop Indicator
	shop_indicator_width = GameUIDataMap["Shop_Indicator_Width"];
	shop_indicator_height = GameUIDataMap["Shop_Indicator_Height"];
	shop_indicator_timer = GameUIDataMap["Shop_Indicator_Timer"];
	shop_indicator_speed = GameUIDataMap["Shop_Indicator_Speed"];
	clicked_on_shop = false;
}

/******************************************************************************/
/*!
	Update Player UI
*/
/******************************************************************************/
void PlayerUI::update(Player& current_player, WaveManager const& wave_manager)
{
	// Shop transitions
	if (shop_triggered) {
		clicked_on_shop = true;

		// Start transition
		if (shop_transition) {
			// Play the sound effect when the transition starts
			if (!shop_sound_played) {
				AudioManager::PlayOnce("Assets/BGM/button-124476.mp3", 0.05f);
				shop_sound_played = true;
			}

			// Adjust shop offset
			shop_trans_timer += g_dt;
			shop_offset = EaseOutExpo(static_cast<f32>(g_windowWidth), 0, shop_trans_timer / shop_trans_duration);

			// Stop transition once duration is met
			if (shop_trans_timer >= shop_trans_duration) {
				shop_trans_timer = 0.f;
				shop_transition = false;
			}
		}
		// Transition done
		else
			shop_open(current_player);
	}
	else {
		// Start transition
		if (shop_transition) {
			// Play the sound effect when the transition starts
			if (!shop_close_sound_played) {
				AudioManager::PlayOnce("Assets/BGM/button-124476.mp3", 0.05f);
				shop_close_sound_played = true;
			}

			// Adjust shop offset
			shop_trans_timer += g_dt;
			shop_offset = EaseInOutBack(0, static_cast<f32>(g_windowWidth), shop_trans_timer / shop_trans_duration);

			// Stop transition once duration is met
			if (shop_trans_timer >= shop_trans_duration) {
				shop_trans_timer = 0.f;
				shop_transition = false;
			}
		}
		// Transition done
		else {
			shop_closed();
			// Reset the shop sound played states when the transition is done
			shop_sound_played = false;
			shop_close_sound_played = false;
		}
	}


	// Tutorial transitions
	if (tutorial_triggered) {
		// Start transition
		if (tutorial_transition) {
			// Adjust shop offset
			tutorial_trans_timer += g_dt;
			tutorial_offset = EaseOutExpo(-tutorial_bg_width, 0, tutorial_trans_timer / tutorial_trans_duration);

			// Stop transition once duration is met
			if (tutorial_trans_timer >= tutorial_trans_duration) {
				tutorial_trans_timer = 0.f;
				tutorial_transition = false;
			}
		}

		// Transition done
		else
			tutorial_open();
	}
	else {
		// Start transition
		if (tutorial_transition) {
			// Adjust tutorial offset
			tutorial_trans_timer += g_dt;
			tutorial_offset = EaseInOutBack(0, -tutorial_bg_width, tutorial_trans_timer / tutorial_trans_duration);

			// Stop transition once duration is met
			if (tutorial_trans_timer >= tutorial_trans_duration) {
				tutorial_trans_timer = 0.f;
				tutorial_transition = false;
			}
		}

		// Transition done
		else
			tutorial_closed();
	}

	// =================
	// Update positions
	// =================

	// Player HUD to follow camera
	player_hud_position.x = g_camPos.x;
	player_hud_position.y = g_camPos.y;

	// Shop background
	shop_bg_position.x = g_camPos.x + shop_offset;
	shop_bg_position.y = g_camPos.y;

	// Tutorial background
	tutorial_bg_position.x = g_camPos.x - (g_windowWidth - tutorial_bg_width) / 2.f + tutorial_offset;
	tutorial_bg_position.y = g_camPos.y + g_windowHeight / 2.f - button_vector[TUTORIAL_OPEN].height * 3.f;

	// Button to open shop
	button_vector[SHOP_OPEN].position.x = g_camPos.x + g_windowWidth / 2.f - button_vector[SHOP_OPEN].width / 2.f;
	button_vector[SHOP_OPEN].position.y = g_camPos.y + g_windowHeight / 2.f - button_vector[SHOP_OPEN].height * 3.f;

	// Button to open tutorial
	button_vector[TUTORIAL_OPEN].position.x = g_camPos.x - (g_windowWidth - button_vector[TUTORIAL_OPEN].width) / 2.f + tutorial_bg_width + tutorial_offset;
	button_vector[TUTORIAL_OPEN].position.y = g_camPos.y + g_windowHeight / 2.f - button_vector[TUTORIAL_OPEN].height * 3.f;

	// Upgrade preview on left half of shop
	upgrade_preview_position.x = g_camPos.x - shop_bg_width / 4.f + shop_offset;
	upgrade_preview_position.y = g_camPos.y + shop_bg_width / 8.f;

	// Movement speed, capacity and beam strength buttons on the left half
	for (int i = MOVEMENT_SPEED; i < CREATE_DRONE; ++i) {
		ShopOption& button = button_vector[i];
		button.position.x = g_camPos.x - shop_bg_width / 4.f - button.width * 1.3f * (2 - i) + shop_offset;
		button.position.y = g_camPos.y - shop_bg_height / 8.f;

		// Level indicators for upgrades
		for (int j = 0; j < button.indicator_vector.size(); ++j) {
			UpgradeLevelIndicator& indicator = button.indicator_vector[j];
			indicator.position.x = button.position.x - indicator.width * (2 - j);
			indicator.position.y = button.position.y - button.height;
		}
	}

	// Drone and space station buttons on the right half
	for (int i = CREATE_DRONE; i < TUTORIAL_OPEN; ++i) {
		ShopOption& button = button_vector[i];
		button.position.x = g_camPos.x + shop_bg_width / 4.f + shop_offset;
		button.position.y = button_vector[SHOP_OPEN].position.y - (i - 4) * button.height * 4.f;

		// Level indicators for upgrades
		for (int j = 0; j < button.indicator_vector.size(); ++j) {
			UpgradeLevelIndicator& indicator = button.indicator_vector[j];
			indicator.position.x = button.position.x - indicator.width * (2 - j);
			indicator.position.y = button.position.y - button.height;
		}
	}

	// Shuttle Lost timer start
	if (wave_manager.shuttle_has_collided)
	{
		lost_overlay_timer = LOST_OVERLAY_TIME;
	}
	if (lost_overlay_timer > 0.f) lost_overlay_timer -= g_dt;

	// Blinking indicator for shop icon
	if (wave_manager.wave_completed)
	{
		// Expanding indicator if not clicked on shop yet
		// When shop clicked removed indicator
		shop_indicator_timer = (!clicked_on_shop) ? shop_indicator_timer + g_dt : 0.f;

		// If player skips the wave interval
		if (AEInputCheckTriggered(AEVK_R)) shop_indicator_timer = 0.f;
	}

	// =======================
	// Calculate the matrices
	// =======================

	AEMtx33 scale, rot, trans;

	// Shuttle Lost Overlay
	AEMtx33Scale(&scale, g_windowWidth * 1.5f, g_windowHeight * 1.5f);
	AEMtx33Rot(&rot, 0.f);
	AEMtx33Trans(&trans, g_camPos.x, g_camPos.y);
	AEMtx33Concat(&lost_overlay_transform, &rot, &scale);
	AEMtx33Concat(&lost_overlay_transform, &trans, &lost_overlay_transform);

	// Shop Indicator
	// Use AESin() to produce oscillating effect, multiply by 3 to speed up effect
	AEMtx33Scale(&scale, shop_indicator_width * (1 + AESin(shop_indicator_timer * shop_indicator_speed)), shop_indicator_height * (1 + AESin(shop_indicator_timer * shop_indicator_speed)));
	AEMtx33Rot(&rot, 0.f);
	AEMtx33Trans(&trans, button_vector[SHOP_OPEN].position.x, button_vector[SHOP_OPEN].position.y);
	AEMtx33Concat(&shop_indicator_transform, &rot, &scale);
	AEMtx33Concat(&shop_indicator_transform, &trans, &shop_indicator_transform);

	// Player HUD
	AEMtx33Scale(&scale, player_hud_width, player_hud_height);
	AEMtx33Rot(&rot, 0.f);
	AEMtx33Trans(&trans, player_hud_position.x, player_hud_position.y);
	AEMtx33Concat(&player_hud_transform, &rot, &scale);
	AEMtx33Concat(&player_hud_transform, &trans, &player_hud_transform);

	// Shop background
	AEMtx33Scale(&scale, shop_bg_width, shop_bg_height);
	AEMtx33Rot(&rot, 0.f);
	AEMtx33Trans(&trans, shop_bg_position.x, shop_bg_position.y);
	AEMtx33Concat(&shop_bg_transform, &rot, &scale);
	AEMtx33Concat(&shop_bg_transform, &trans, &shop_bg_transform);

	// Tutorial background
	AEMtx33Scale(&scale, tutorial_bg_width, tutorial_bg_height);
	AEMtx33Rot(&rot, 0.f);
	AEMtx33Trans(&trans, tutorial_bg_position.x, tutorial_bg_position.y);
	AEMtx33Concat(&tutorial_bg_transform, &rot, &scale);
	AEMtx33Concat(&tutorial_bg_transform, &trans, &tutorial_bg_transform);

	// Upgrade preview
	AEMtx33Scale(&scale, upgrade_preview_size, upgrade_preview_size);
	AEMtx33Rot(&rot, PI);
	AEMtx33Trans(&trans, upgrade_preview_position.x, upgrade_preview_position.y);
	AEMtx33Concat(&upgrade_preview_transform, &rot, &scale);
	AEMtx33Concat(&upgrade_preview_transform, &trans, &upgrade_preview_transform);

	// Shop buttons
	for (int i = 0; i < button_vector.size(); ++i) {
		ShopOption& button = button_vector[i];
		AEMtx33Scale(&scale, button.width, button.height);
		AEMtx33Rot(&rot, PI);
		AEMtx33Trans(&trans, button.position.x, button.position.y);
		AEMtx33Concat(&button.transform, &rot, &scale);
		AEMtx33Concat(&button.transform, &trans, &button.transform);

		// Level indicators for upgrades
		if (i > SHOP_OPEN) {
			for (int j = 0; j < button.indicator_vector.size(); ++j) {
				UpgradeLevelIndicator& indicator = button.indicator_vector[j];
				AEMtx33Scale(&scale, indicator.width, indicator.height);
				AEMtx33Rot(&rot, PI);
				AEMtx33Trans(&trans, indicator.position.x, indicator.position.y);
				AEMtx33Concat(&indicator.transform, &rot, &scale);
				AEMtx33Concat(&indicator.transform, &trans, &indicator.transform);
			}
		}

		// Shop icons (drone/space station)
		if (button.button_type == CREATE_DRONE) {
			AEMtx33Scale(&scale, icon_size * 4, icon_size * 4);
			AEMtx33Rot(&rot, PI);
			AEMtx33Trans(&trans, button.position.x, button.position.y);
			AEMtx33Concat(&drone_icon_transform, &rot, &scale);
			AEMtx33Concat(&drone_icon_transform, &trans, &drone_icon_transform);
		}
		else if (button.button_type == SPACE_STATION) {
			AEMtx33Scale(&scale, icon_size * 4, icon_size * 4);
			AEMtx33Rot(&rot, PI);
			AEMtx33Trans(&trans, button.position.x, button.position.y);
			AEMtx33Concat(&space_station_icon_transform, &rot, &scale);
			AEMtx33Concat(&space_station_icon_transform, &trans, &space_station_icon_transform);
		}
	}
}

/******************************************************************************/
/*!
	Draw Player UI
*/
/******************************************************************************/
void PlayerUI::draw(AEGfxVertexList* pMesh, WaveManager const& wave_manager)
{
	AEGfxSetBlendColor(0.f, 0.f, 0.f, 0.f);

	// Losing a shuttle
	if (lost_overlay_timer > 0.f)
	{
		AEGfxSetTintColor(1.f, 0.f, 0.f, 0.7f * lost_overlay_timer / static_cast<f32>(LOST_OVERLAY_TIME));
		AEGfxTextureSet(shuttle_lost_tex, 0, 0);
		AEGfxSetTransform(lost_overlay_transform.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	}
	AEGfxSetTintColor(1.f, 1.f, 1.f, 1.f);

	// Shop Indicator
	AEGfxTextureSet(shop_indicator_tex, 0, 0);
	AEGfxSetTransform(shop_indicator_transform.m);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

	// Player HUD
	AEGfxTextureSet(player_hud_tex, 0, 0);
	AEGfxSetTransform(player_hud_transform.m);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

	// Print score
	score = "Score: " + std::to_string(player.score);
	AEGfxPrint(font_id, const_cast<s8*>(score.c_str()), -0.45f - (static_cast<f32>(score.length()) * static_cast<f32>(FONT_ID_SIZE) / g_windowWidth), 0.86f, 1.f, 1.f, 1.f, 1.f);

	// Print credits
	credits = "Credits: " + std::to_string(player.credits);
	AEGfxPrint(font_id, const_cast<s8*>(credits.c_str()), 0.45f, 0.86f, 1.f, 1.f, 1.f, 1.f);

	// Print capacity
	capacity = std::to_string(player.current_capacity) + " / " + std::to_string(player.max_capacity + player.capacity_level);
	AEVec2 capacity_pos;
	AEVec2Sub(&capacity_pos, &player.position, &g_camPos);

	// Capacity to fade to red when close to max capacity
	AEGfxPrint(font_id, const_cast<s8*>(capacity.c_str()),
		static_cast<f32>((capacity_pos.x - (0.5f * FONT_ID_SIZE)) / (g_windowWidth / 2.f)),
		static_cast<f32>((capacity_pos.y + (0.5f * FONT_ID_SIZE)) / (g_windowHeight / 2.f)),
		0.8f, 1.f, static_cast<f32>((player.max_capacity + player.capacity_level - player.current_capacity)) / static_cast<f32>((player.max_capacity + player.capacity_level)),
		static_cast<f32>((player.max_capacity + player.capacity_level - player.current_capacity)) / static_cast<f32>((player.max_capacity + player.capacity_level)));

	// Shop background
	AEGfxTextureSet(shop_background_tex, 0, 0);
	AEGfxSetTransparency(0.5f);
	AEGfxSetTransform(shop_bg_transform.m);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

	// Tutorial background
	AEGfxTextureSet(tutorial_background_tex, 0, 0);
	AEGfxSetTransparency(0.5f);
	AEGfxSetTransform(tutorial_bg_transform.m);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

	// Reset transparency
	AEGfxSetTransparency(1.f);

	// Upgrade preview
	for (int i = 0; i < button_vector.size(); ++i) {
		ShopOption& button = button_vector[i];
		if (hover_over_button(button)) {
			if (button.button_type == MOVEMENT_SPEED) {
				AEGfxTextureSet(speed_hover_tex, 0, 0);
				break;
			}

			else if (button.button_type == CAPACITY) {
				AEGfxTextureSet(capacity_hover_tex, 0, 0);
				break;
			}

			else if (button.button_type == TRACTOR_BEAM_STRENGTH) {
				AEGfxTextureSet(strength_hover_tex, 0, 0);
				break;
			}
		}
		else
			AEGfxTextureSet(player_tex, 0, 0);
	}
	AEGfxSetTransform(upgrade_preview_transform.m);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

	// Shop buttons
	for (int i = 0; i < button_vector.size(); ++i) {
		ShopOption& button = button_vector[i];

		if (button.button_type == SHOP_OPEN) {
			if (shop_triggered)
				AEGfxTextureSet(shop_close_tex, 0, 0);
			else
				AEGfxTextureSet(shop_open_tex, 0, 0);

			AEGfxSetTransform(button.transform.m);
			AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
		}

		// Shop upgrades
		if (i > SHOP_OPEN) {
			switch (button.button_type) {
			case MOVEMENT_SPEED:
				AEGfxTextureSet(mov_speed_button_tex, 0, 0);
				AEGfxSetTransform(button.transform.m);
				AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

				for (int j = 0; j < player.mov_speed_level; ++j) {
					AEGfxTextureSet(upgrade_level_solid_tex, 0, 0);
					UpgradeLevelIndicator& indicator = button.indicator_vector[j];
					AEGfxSetTransform(indicator.transform.m);
					AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
				}
				for (int j = static_cast<int>(player.mov_speed_level); j < MAX_MOV_SPEED_LVL; ++j) {
					AEGfxTextureSet(upgrade_level_hollow_tex, 0, 0);
					UpgradeLevelIndicator& indicator = button.indicator_vector[j];
					AEGfxSetTransform(indicator.transform.m);
					AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
				}
				break;

			case CAPACITY:
				AEGfxTextureSet(capacity_button_tex, 0, 0);
				AEGfxSetTransform(button.transform.m);
				AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

				for (int j = 0; j < player.capacity_level; ++j) {
					AEGfxTextureSet(upgrade_level_solid_tex, 0, 0);
					UpgradeLevelIndicator& indicator = button.indicator_vector[j];
					AEGfxSetTransform(indicator.transform.m);
					AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
				}
				for (int j = static_cast<int>(player.capacity_level); j < MAX_CAPACITY_LVL; ++j) {
					AEGfxTextureSet(upgrade_level_hollow_tex, 0, 0);
					UpgradeLevelIndicator& indicator = button.indicator_vector[j];
					AEGfxSetTransform(indicator.transform.m);
					AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
				}
				break;

			case TRACTOR_BEAM_STRENGTH:
				AEGfxTextureSet(strength_button_tex, 0, 0);
				AEGfxSetTransform(button.transform.m);
				AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

				for (int j = 0; j < player.beam_level; ++j) {
					AEGfxTextureSet(upgrade_level_solid_tex, 0, 0);
					UpgradeLevelIndicator& indicator = button.indicator_vector[j];
					AEGfxSetTransform(indicator.transform.m);
					AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
				}
				for (int j = static_cast<int>(player.beam_level); j < MAX_BEAM_STRENGTH_LVL; ++j) {
					AEGfxTextureSet(upgrade_level_hollow_tex, 0, 0);
					UpgradeLevelIndicator& indicator = button.indicator_vector[j];
					AEGfxSetTransform(indicator.transform.m);
					AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
				}
				break;

			case CREATE_DRONE:
				AEGfxTextureSet(drone_button_tex, 0, 0);
				AEGfxSetTransform(button.transform.m);
				AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
				break;

			case SPACE_STATION:
				AEGfxTextureSet(space_station_button_tex, 0, 0);
				AEGfxSetTransform(button.transform.m);
				AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

				for (int j = 0; j < player.space_station_count; ++j) {
					AEGfxTextureSet(upgrade_level_solid_tex, 0, 0);
					UpgradeLevelIndicator& indicator = button.indicator_vector[j];
					AEGfxSetTransform(indicator.transform.m);
					AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
				}
				for (int j = static_cast<int>(player.space_station_count); j < MAX_SPACE_STATION_CNT; ++j) {
					AEGfxTextureSet(upgrade_level_hollow_tex, 0, 0);
					UpgradeLevelIndicator& indicator = button.indicator_vector[j];
					AEGfxSetTransform(indicator.transform.m);
					AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
				}
				break;

			case TUTORIAL_OPEN:
				AEGfxTextureSet(tutorial_open_tex, 0, 0);
				AEGfxSetTransform(button.transform.m);
				AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
				break;
			}
		}
	}

	// Shop buttons (text/icons)
	std::string	shop_text;
	shop_text = "PLAYER UPGRADES";
	AEGfxPrint(font_id_shop, const_cast<s8*>(shop_text.c_str()), -0.61f + shop_offset / (g_windowWidth / 2.f), 0.7f, 3.f, 0.f, 0.f, 0.f);

	shop_text = "PLACEABLES";
	AEGfxPrint(font_id_shop, const_cast<s8*>(shop_text.c_str()), 0.28f + shop_offset / (g_windowWidth / 2.f), 0.7f, 3.f, 0.f, 0.f, 0.f);


	//Description for Placeables (DRONE)
	shop_text = "LMB          Select/Place Drone";
	AEGfxPrint(font_id_shop, const_cast<s8*>(shop_text.c_str()), 0.3f + shop_offset / (g_windowWidth / 2.f), 0.35f, 1.f, 1.f, 1.f, 1.f);

	shop_text = "Automatically clears Debris";
	AEGfxPrint(font_id_shop, const_cast<s8*>(shop_text.c_str()), 0.3f + shop_offset / (g_windowWidth / 2.f), 0.25f, 1.f, 1.f, 1.f, 1.f);

	shop_text = "Place onto Planets";
	AEGfxPrint(font_id_shop, const_cast<s8*>(shop_text.c_str()), 0.3f + shop_offset / (g_windowWidth / 2.f), 0.15f, 1.f, 1.f, 1.f, 1.f);


	//Description for Placeables (SPACE STATION)
	shop_text = "LMB          Select/Place Space Station";
	AEGfxPrint(font_id_shop, const_cast<s8*>(shop_text.c_str()), 0.3f + shop_offset / (g_windowWidth / 2.f), -0.45f, 1.f, 1.f, 1.f, 1.f);

	shop_text = "Add additional Space Station";
	AEGfxPrint(font_id_shop, const_cast<s8*>(shop_text.c_str()), 0.3f + shop_offset / (g_windowWidth / 2.f), -0.55f, 1.f, 1.f, 1.f, 1.f);

	shop_text = "Place in Space";
	AEGfxPrint(font_id_shop, const_cast<s8*>(shop_text.c_str()), 0.3f + shop_offset / (g_windowWidth / 2.f), -0.65f, 1.f, 1.f, 1.f, 1.f);

	// Print upgrade name onto buttons
	for (int i = MOVEMENT_SPEED; i < TUTORIAL_OPEN; ++i) {
		ShopOption& button = button_vector[i];

		if (button.button_type == MOVEMENT_SPEED)
			shop_text = "Movement Speed";

		else if (button.button_type == CAPACITY)
			shop_text = "Increase Capacity";

		else if (button.button_type == TRACTOR_BEAM_STRENGTH)
			shop_text = "Beam Strength";

		else if (button.button_type == CREATE_DRONE)
			shop_text = "Drone";

		else if (button.button_type == SPACE_STATION)
			shop_text = "Space Station";

		AEGfxPrint(font_id_shop, const_cast<s8*>(shop_text.c_str()),
			(button.position.x - button.width / 2.f - g_camPos.x) / (g_windowWidth / 2.f),
			(button.position.y - button.height / 2.f - g_camPos.y) / (g_windowHeight / 2.f),
			1.f, 1.f, 1.f, 1.f);
	}

	shop_text = "PLAYER UPGRADES";
	AEGfxPrint(font_id_shop, const_cast<s8*>(shop_text.c_str()), -0.61f + shop_offset / (g_windowWidth / 2.f), 0.7f, 3.f, 0.f, 0.f, 0.f);

	shop_text = "PLACEABLES";
	AEGfxPrint(font_id_shop, const_cast<s8*>(shop_text.c_str()), 0.28f + shop_offset / (g_windowWidth / 2.f), 0.7f, 3.f, 0.f, 0.f, 0.f);

	// Placed into function to avoid cluttering draw
	tutorial(wave_manager);
}

/******************************************************************************/
/*!
	Clean Object Instances
*/
/******************************************************************************/
void PlayerUI::free()
{
	button_vector.clear();
	if (next_state != GS_RESTART) {
		GameUIData.clear();
		GameUIDataMap.clear();
	}
}

/******************************************************************************/
/*!
	Free Textures
*/
/******************************************************************************/
void PlayerUI::unload()
{
	AEGfxTextureUnload(shop_icon_tex);
	AEGfxTextureUnload(shop_open_tex);
	AEGfxTextureUnload(shop_close_tex);
	AEGfxTextureUnload(space_station_tex);
	AEGfxTextureUnload(shop_background_tex);
	AEGfxTextureUnload(tutorial_open_tex);
	AEGfxTextureUnload(tutorial_background_tex);
	AEGfxTextureUnload(upgrade_level_hollow_tex);
	AEGfxTextureUnload(upgrade_level_solid_tex);
	AEGfxTextureUnload(player_hud_tex);
	AEGfxTextureUnload(speed_hover_tex);
	AEGfxTextureUnload(capacity_hover_tex);
	AEGfxTextureUnload(strength_hover_tex);
	AEGfxTextureUnload(mov_speed_button_tex);
	AEGfxTextureUnload(capacity_button_tex);
	AEGfxTextureUnload(strength_button_tex);
	AEGfxTextureUnload(drone_button_tex);
	AEGfxTextureUnload(space_station_button_tex);
	AEGfxTextureUnload(shuttle_lost_tex);
	AEGfxTextureUnload(shop_indicator_tex);
}

/******************************************************************************/
/*!
	Additional Functions
*/
/******************************************************************************/
// Shop is open
void PlayerUI::shop_open(Player& current_player)
{
	// ===================
	// Check player input
	// ===================

	// Player clicks outside shop
	if (click_outside_shop() || AEInputCheckTriggered(AEVK_ESCAPE))
		close_shop();

	else {
		// =================
		// Shop interaction
		// =================

		for (int i = 0; i < button_vector.size(); ++i) {
			ShopOption& button = button_vector[i];

			if (button_clicked(button)) {
				if (button.button_type == MOVEMENT_SPEED) {
					AudioManager::PlayOnce("Assets/BGM/button-124476.mp3", 0.05f);
					if (current_player.credits >= mov_speed_cost && current_player.mov_speed_level < MAX_MOV_SPEED_LVL) {
						current_player.credits -= mov_speed_cost;
						current_player.mov_speed_level++;
					}
				}
				else if (button.button_type == CAPACITY) {
					AudioManager::PlayOnce("Assets/BGM/button-124476.mp3", 0.05f);
					if (current_player.credits >= capacity_cost && current_player.capacity_level < MAX_CAPACITY_LVL) {
						current_player.credits -= capacity_cost;
						current_player.capacity_level++;
					}
				}
				else if (button.button_type == TRACTOR_BEAM_STRENGTH) {
					AudioManager::PlayOnce("Assets/BGM/button-124476.mp3", 0.05f);
					if (current_player.credits >= beam_strength_cost && current_player.beam_level < MAX_BEAM_STRENGTH_LVL) {
						current_player.credits -= beam_strength_cost;
						current_player.beam_level++;
					}
				}
				else if (button.button_type == CREATE_DRONE || button.button_type == SPACE_STATION) {
					AudioManager::PlayOnce("Assets/BGM/button-124476.mp3", 0.05f);
					if (button.button_type == CREATE_DRONE && !placing_drone && current_player.credits >= drone_cost)
						placing_drone = true;

					else if (button.button_type == SPACE_STATION && !placing_station && current_player.credits >= space_station_cost)
						placing_station = true;

					close_shop();
				}
			}
		}
	}
}

// Shop is closed
void PlayerUI::shop_closed()
{
	// Open the shop and start transition
	if (button_clicked(button_vector[SHOP_OPEN])) {
		shop_triggered = true;
		shop_transition = true;
	}
}

// Function to close shop
void PlayerUI::close_shop()
{
	// Close shop
	shop_triggered = false;
	shop_transition = true;
}

// Check if button is clicked
bool PlayerUI::button_clicked(ShopOption button)
{
	// Get position of each side of button
	f32 button_left = button.position.x - button.width / 2.f;
	f32 button_right = button.position.x + button.width / 2.f;
	f32 button_top = button.position.y + button.height / 2.f;
	f32 button_bottom = button.position.y - button.height / 2.f;

	if (AEInputCheckTriggered(AEVK_LBUTTON)) {
		if ((button_left < g_mouseWorld.x) && (button_right > g_mouseWorld.x)) {
			if ((button_bottom < g_mouseWorld.y) && (button_top > g_mouseWorld.y)) {
				return true;
			}
		}
	}

	return false;
}

// Check if LEFT MOUSE is triggered when cursor is outside shop
bool PlayerUI::click_outside_shop()
{
	// Get position of each side of shop background
	f32 shop_background_left = g_camPos.x - shop_bg_width / 2.f;
	f32 shop_background_right = g_camPos.x + shop_bg_width / 2.f;
	f32 shop_background_top = g_camPos.y + shop_bg_height / 2.f;
	f32 shop_background_bottom = g_camPos.y - shop_bg_height / 2.f;

	if (AEInputCheckTriggered(AEVK_LBUTTON)) {
		// Left/right/top/bottom border
		if ((g_mouseWorld.x > (g_camPos.x - static_cast<f32>(g_windowWidth) / 2.f) && g_mouseWorld.x < shop_background_left) ||
			(g_mouseWorld.x > shop_background_right && g_mouseWorld.x < (g_camPos.x + static_cast<f32>(g_windowWidth) / 2.f)) ||
			(g_mouseWorld.y > (g_camPos.y - static_cast<f32>(g_windowHeight) / 2.f) && g_mouseWorld.y < shop_background_bottom) ||
			(g_mouseWorld.y > shop_background_top && g_mouseWorld.y < (g_camPos.y + static_cast<f32>(g_windowHeight) / 2.f)))

			return true;
	}

	return false;
}

// Check if cursor is hovering over button
bool PlayerUI::hover_over_button(ShopOption button)
{
	// Get position of each side of button
	f32 button_left = button.position.x - button.width / 2.f;
	f32 button_right = button.position.x + button.width / 2.f;
	f32 button_top = button.position.y + button.height / 2.f;
	f32 button_bottom = button.position.y - button.height / 2.f;

	if ((button_left < g_mouseWorld.x) && (button_right > g_mouseWorld.x)) {
		if ((button_bottom < g_mouseWorld.y) && (button_top > g_mouseWorld.y)) {
			return true;
		}
	}

	return false;
}

// Tutorial box is open
void PlayerUI::tutorial_open()
{
	// ===================
	// Check player input
	// ===================

	// Player clicks outside tutorial
	if (button_clicked(button_vector[TUTORIAL_OPEN]) || AEInputCheckTriggered(AEVK_ESCAPE))
		close_tutorial();
}

// Tutorial box is closed
void PlayerUI::tutorial_closed()
{
	// Open the tutorial box and start transition
	if (button_clicked(button_vector[TUTORIAL_OPEN])) {
		tutorial_triggered = true;
		tutorial_transition = true;
	}
}

// Function to close tutorial box
void PlayerUI::close_tutorial()
{
	// Close tutorial
	tutorial_triggered = false;
	tutorial_transition = true;
}

void PlayerUI::tutorial(WaveManager const& wave_manager)
{
	// Tutorial text---------------------------------------------------------------------------------------------------------------------------------------------------
	AEVec2 tutorial_pos;
	std::string tutorial;
	tutorial = "Controls";
	AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()), -0.98f + tutorial_offset / (g_windowWidth / 2.f), 0.75f, 1.5f, 1.f, 1.f, 1.f);

	if (PLAYER_FLY == player.state)
	{
		// Controls Tutorial text
		tutorial = "W          Accelerate";
		AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()), -0.98f + tutorial_offset / (g_windowWidth / 2.f), 0.65f, 1.f, 1.f, 1.f, 1.f);

		tutorial = "S           Decelerate";
		AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()), -0.98f + tutorial_offset / (g_windowWidth / 2.f), 0.6f, 1.f, 1.f, 1.f, 1.f);

		tutorial = "A           Rotate Anti-Clockwise";
		AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()), -0.98f + tutorial_offset / (g_windowWidth / 2.f), 0.55f, 1.f, 1.f, 1.f, 1.f);

		tutorial = "D           Rotate Clockwise";
		AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()), -0.98f + tutorial_offset / (g_windowWidth / 2.f), 0.5f, 1.f, 1.f, 1.f, 1.f);

		tutorial = "LMB      Release Debris";
		AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()), -0.98f + tutorial_offset / (g_windowWidth / 2.f), 0.42f, 1.f, 1.f, 1.f, 1.f);

		// Guide player to planet
		if (1 == wave_manager.wave_number && !wave_manager.planet_adding && !wave_manager.wave_completed && player.current_capacity != player.max_capacity + player.capacity_level)
		{
			tutorial = "W A S D";
			AEVec2Sub(&tutorial_pos, &player.position, &camera.position);

			// Draw timer at center of planet using position calculated above
			AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()),
				(tutorial_pos.x - (1.5f * FONT_ID_SIZE)) / (g_windowWidth / 2.f),
				(tutorial_pos.y + (3.f * FONT_ID_SIZE)) / (g_windowHeight / 2.f),
				1.5f, 1.f, 1.f, 1.f);

			tutorial = "Move to a Planet!";
			AEVec2Sub(&tutorial_pos, &player.position, &camera.position);

			AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()),
				static_cast<f32>((tutorial_pos.x - (3 * FONT_ID_SIZE)) / (g_windowWidth / 2.f)),
				static_cast<f32>((tutorial_pos.y + (1.5 * FONT_ID_SIZE)) / (g_windowHeight / 2.f)),
				1.2f, 1.f, 1.f, 1.f);
		}
	}
	else if (PLAYER_TRANSIT == player.state) {
		tutorial = "W          Hold to Leave";
		AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()), -0.98f + tutorial_offset / (g_windowWidth / 2.f), 0.65f, 1.f, 1.f, 1.f, 1.f);

		tutorial = "Leaving Planet...";
		AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()), -0.98f + tutorial_offset / (g_windowWidth / 2.f), 0.55f, 1.f, 1.f, 1.f, 1.f);

		// Show "Leaving Planet" message above player
		if (1 == wave_manager.wave_number && !wave_manager.planet_adding && !wave_manager.wave_completed && player.current_capacity != player.max_capacity + player.capacity_level)
		{
			tutorial = "Hold W";
			AEVec2Sub(&tutorial_pos, &player.position, &camera.position);

			// Draw timer at center of planet using position calculated above
			AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()),
				(tutorial_pos.x - (1.5f * FONT_ID_SIZE)) / (g_windowWidth / 2.f),
				(tutorial_pos.y + (3.f * FONT_ID_SIZE)) / (g_windowHeight / 2.f),
				1.5f, 1.f, 1.f, 1.f);

			tutorial = "Leaving Planet...";
			AEVec2Sub(&tutorial_pos, &player.position, &camera.position);

			AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()),
				(tutorial_pos.x - (3.f * FONT_ID_SIZE)) / (g_windowWidth / 2.f),
				(tutorial_pos.y + (1.5f * FONT_ID_SIZE)) / (g_windowHeight / 2.f),
				1.2f, 1.f, 1.f, 1.f);
		}
	}
	else
	{
		// Controls Tutorial text
		tutorial = "W          Hold to Leave";
		AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()), -0.98f + tutorial_offset / (g_windowWidth / 2.f), 0.65f, 1.f, 1.f, 1.f, 1.f);

		tutorial = "A           Rotate Anti-Clockwise";
		AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()), -0.98f + tutorial_offset / (g_windowWidth / 2.f), 0.6f, 1.f, 1.f, 1.f, 1.f);

		tutorial = "D           Rotate Clockwise";
		AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()), -0.98f + tutorial_offset / (g_windowWidth / 2.f), 0.55f, 1.f, 1.f, 1.f, 1.f);

		tutorial = "Space   Collect Debris";
		AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()), -0.98f + tutorial_offset / (g_windowWidth / 2.f), 0.48f, 1.f, 1.f, 1.f, 1.f);

		tutorial = "LMB      Release Debris";
		AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()), -0.98f + tutorial_offset / (g_windowWidth / 2.f), 0.43f, 1.f, 1.f, 1.f, 1.f);

		// Tell player what to do
		if (1 == wave_manager.wave_number && !wave_manager.planet_adding && !wave_manager.wave_completed && player.current_capacity != player.max_capacity + player.capacity_level)
		{
			tutorial = "Spacebar";
			AEVec2Sub(&tutorial_pos, &player.position, &camera.position);

			// Draw timer at center of planet using position calculated above
			AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()),
				static_cast<f32>((tutorial_pos.x - (2.25f * FONT_ID_SIZE)) / (g_windowWidth / 2.f)),
				static_cast<f32>((tutorial_pos.y + (3.f * FONT_ID_SIZE)) / (g_windowHeight / 2.f)),
				1.5f, 1.f, 1.f, 1.f);

			tutorial = "Collect Debris!";
			AEVec2Sub(&tutorial_pos, &player.position, &camera.position);

			// Draw timer at center of planet using position calculated above
			AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()),
				static_cast<f32>((tutorial_pos.x - (3.f * FONT_ID_SIZE)) / (g_windowWidth / 2.f)),
				static_cast<f32>((tutorial_pos.y + (1.5f * FONT_ID_SIZE)) / (g_windowHeight / 2.f)),
				1.2f, 1.f, 1.f, 1.f);
		}
	}

	if (player.current_capacity == player.max_capacity + player.capacity_level)
	{
		tutorial = "LMB";
		AEVec2Sub(&tutorial_pos, &player.position, &camera.position);

		// Draw timer at center of planet using position calculated above
		AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()),
			(tutorial_pos.x - FONT_ID_SIZE) / (g_windowWidth / 2.f),
			(tutorial_pos.y + (3.f * FONT_ID_SIZE)) / (g_windowHeight / 2.f),
			1.5f, 1.f, 1.f, 1.f);

		tutorial = "Unload Debris!";
		AEVec2Sub(&tutorial_pos, &player.position, &camera.position);

		// Draw timer at center of planet using position calculated above
		AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()),
			(tutorial_pos.x - (3.f * FONT_ID_SIZE)) / (g_windowWidth / 2.f),
			(tutorial_pos.y + (1.5f * FONT_ID_SIZE)) / (g_windowHeight / 2.f),
			1.2f, 1.f, 1.f, 1.f);
	}

	// Guide for first few waves
	if (wave_manager.wave_number < 4)
	{
		tutorial = "You can purchase more at the shop!";
		AEVec2Sub(&tutorial_pos, &space_station_vector[0].position, &camera.position);

		AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()),
			static_cast<f32>((tutorial_pos.x - (static_cast<f32>(tutorial.length()) / 6.f * FONT_ID_SIZE)) / (g_windowWidth / 2.f)),
			static_cast<f32>((tutorial_pos.y + (5 * FONT_ID_SIZE)) / (g_windowHeight / 2)),
			1.f, 1.f, 1.f, 1.f);

		tutorial = "LMB     Unload Debris Here!";
		AEVec2Sub(&tutorial_pos, &space_station_vector[0].position, &camera.position);

		AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()),
			static_cast<f32>((tutorial_pos.x - (static_cast<f32>(tutorial.length()) / 6.f * FONT_ID_SIZE)) / (g_windowWidth / 2.f)),
			static_cast<f32>((tutorial_pos.y - (5 * FONT_ID_SIZE)) / (g_windowHeight / 2)),
			1.f, 1.f, 1.f, 1.f);

		tutorial = "Debris processed here for credits!";
		AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()),
			static_cast<f32>((tutorial_pos.x - (static_cast<f32>(tutorial.length()) / 6.f * FONT_ID_SIZE)) / (g_windowWidth / 2.f)),
			static_cast<f32>((tutorial_pos.y - (4 * FONT_ID_SIZE)) / (g_windowHeight / 2.f)),
			1.f, 1.f, 1.f, 1.f);

		// Shop Tutorial between waves
		if (wave_manager.wave_completed)
		{
			tutorial = "Check the Shop!";
			AEVec2Sub(&tutorial_pos, &player.position, &camera.position);

			// Draw timer at center of planet using position calculated above
			AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()),
				static_cast<f32>((tutorial_pos.x - (2.5 * FONT_ID_SIZE)) / (g_windowWidth / 2.f)),
				static_cast<f32>((tutorial_pos.y + FONT_ID_SIZE) / (g_windowHeight / 2.f)),
				1.f, 1.f, 1.f, 1.f);

			if (3 == wave_manager.wave_number)
			{
				tutorial = "Check out the other planet!";
				AEVec2Sub(&tutorial_pos, &player.position, &camera.position);

				// Draw timer at center of planet using position calculated above
				AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()),
					static_cast<f32>((tutorial_pos.x - (FONT_ID_SIZE * 4.5f)) / (g_windowWidth / 2.f)),
					static_cast<f32>((tutorial_pos.y + FONT_ID_SIZE * 2.f) / (g_windowHeight / 2.f)),
					1.f, 1.f, 1.f, 1.f);
			}

			if (wave_manager.shuttle_destroyed)
			{
				tutorial = "Oh no! A shuttle has crashed!";
				AEVec2Sub(&tutorial_pos, &planet_vector[0].position, &camera.position);

				// Draw timer at center of planet using position calculated above
				AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()),
					static_cast <f32>((tutorial_pos.x - (static_cast<f32>(tutorial.length()) / 4.5f * FONT_ID_SIZE)) / (g_windowWidth / 2.f)),
					static_cast <f32>((tutorial_pos.y + FONT_ID_SIZE * 4.5f) / (g_windowHeight / 2.f)),
					1.5f, 1.f, 1.f, 1.f);

				tutorial = "Clear debris to help them leave!";
				AEVec2Sub(&tutorial_pos, &planet_vector[0].position, &camera.position);

				// Draw timer at center of planet using position calculated above
				AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()),
					static_cast <f32>((tutorial_pos.x - (static_cast<f32>(tutorial.length()) / 4.5f * FONT_ID_SIZE)) / (g_windowWidth / 2.f)),
					static_cast <f32>((tutorial_pos.y + FONT_ID_SIZE * 3.5f) / (g_windowHeight / 2.f)),
					1.5f, 1.f, 1.f, 1.f);
			}
			else
			{
				tutorial = "Good Job!";
				AEVec2Sub(&tutorial_pos, &planet_vector[0].position, &camera.position);

				// Draw timer at center of planet using position calculated above
				AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()),
					static_cast <f32>((tutorial_pos.x - (static_cast<f32>(tutorial.length()) / 4.5f * FONT_ID_SIZE)) / (g_windowWidth / 2.f)),
					static_cast <f32>((tutorial_pos.y + FONT_ID_SIZE * 4.f) / (g_windowHeight / 2.f)),
					1.5f, 1.f, 1.f, 1.f);
			}

			tutorial = "Shuttles that escape reward you with credits!";
			AEVec2Sub(&tutorial_pos, &planet_vector[0].position, &camera.position);

			// Draw timer at center of planet using position calculated above
			AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()),
				static_cast <f32>((tutorial_pos.x - (static_cast<f32>(tutorial.length()) / 4.5f * FONT_ID_SIZE)) / (g_windowWidth / 2.f)),
				static_cast <f32>((tutorial_pos.y + FONT_ID_SIZE) / (g_windowHeight / 2.f)),
				1.5f, 1.f, 1.f, 1.f);

			tutorial = "Be sure to check out the shop at the top right!";
			AEVec2Sub(&tutorial_pos, &planet_vector[0].position, &camera.position);

			// Draw timer at center of planet using position calculated above
			AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()),
				static_cast <f32>((tutorial_pos.x - (static_cast<f32>(tutorial.length()) / 4.5f * FONT_ID_SIZE)) / (g_windowWidth / 2.f)),
				static_cast <f32>((tutorial_pos.y - FONT_ID_SIZE) / (g_windowHeight / 2.f)),
				1.5f, 1.f, 1.f, 1.f);

			tutorial = "No more than       can be destroyed per wave!";
			AEVec2Sub(&tutorial_pos, &planet_vector[0].position, &camera.position);

			// Draw timer at center of planet using position calculated above
			AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()),
				static_cast <f32>((tutorial_pos.x - (static_cast<f32>(tutorial.length()) / 4.5f * FONT_ID_SIZE)) / (g_windowWidth / 2.f)),
				static_cast <f32>((tutorial_pos.y - (FONT_ID_SIZE * 3.5)) / (g_windowHeight / 2.f)),
				1.5f, 1.f, 1.f, 1.f);

			extern int LOSE_CONDITION;
			tutorial = std::to_string(LOSE_CONDITION);
			AEVec2Sub(&tutorial_pos, &planet_vector[0].position, &camera.position);

			// Draw timer at center of planet using position calculated above
			AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()),
				static_cast <f32>((tutorial_pos.x - (static_cast<f32>(tutorial.length()) * 3.f * FONT_ID_SIZE)) / (g_windowWidth / 2.f)),
				static_cast <f32>((tutorial_pos.y - (FONT_ID_SIZE * 4.f)) / (g_windowHeight / 2.f)),
				2.5f, 1.f, 0.f, 0.f);
		}

		if (!wave_manager.wave_completed)
		{

			if (wave_manager.first_contact)
			{
				// Reset position of tutorial_pos for planet stuff
				AEVec2Sub(&tutorial_pos, &planet_vector[0].position, &camera.position);

				// Shuttle Tutorial on Planet
				tutorial = "Shuttles are leaving soon. They can crash into debris!";

				// Draw timer at center of planet using position calculated above
				AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()),
					static_cast <f32>((tutorial_pos.x - (static_cast<f32>(tutorial.length()) / 4.5f * FONT_ID_SIZE)) / (g_windowWidth / 2.f)),
					static_cast <f32>((tutorial_pos.y + FONT_ID_SIZE * 5.5) / (g_windowHeight / 2.f)),
					1.5f, 1.f, 1.f, 1.f);

				tutorial = "Collect debris to clear a path. [SPACEBAR]";

				// Draw timer at center of planet using position calculated above
				AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()),
					static_cast <f32>((tutorial_pos.x - (static_cast<f32>(tutorial.length()) / 4.5f * FONT_ID_SIZE)) / (g_windowWidth / 2.f)),
					static_cast <f32>((tutorial_pos.y + FONT_ID_SIZE * 4.25f) / (g_windowHeight / 2.f)),
					1.5f, 1.f, 1.f, 1.f);

				tutorial = "Do not let them crash!";

				// Draw timer at center of planet using position calculated above
				AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()),
					static_cast <f32>((tutorial_pos.x - (static_cast<f32>(tutorial.length()) / 4.5f * FONT_ID_SIZE)) / (g_windowWidth / 2.f)),
					static_cast <f32>((tutorial_pos.y + FONT_ID_SIZE * 3.f) / (g_windowHeight / 2.f)),
					1.5f, 1.f, 1.f, 1.f);


				tutorial = "Arrows indicate their direction.";
				AEVec2Sub(&tutorial_pos, &planet_vector[0].position, &camera.position);

				// Draw timer at center of planet using position calculated above
				AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()),
					static_cast <f32>((tutorial_pos.x - (static_cast<f32>(tutorial.length()) / 4.5f * FONT_ID_SIZE)) / (g_windowWidth / 2.f)),
					static_cast <f32>((tutorial_pos.y - FONT_ID_SIZE * 8.f) / (g_windowHeight / 2.f)),
					1.5f, 1.f, 1.f, 1.f);

				tutorial = "Countdown ->";
				AEVec2Sub(&tutorial_pos, &planet_vector[0].position, &camera.position);

				// Draw timer at center of planet using position calculated above
				AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()),
					static_cast <f32>((tutorial_pos.x - (static_cast<f32>(tutorial.length()) / 1.7f * FONT_ID_SIZE)) / (g_windowWidth / 2.f)),
					static_cast <f32>((tutorial_pos.y - (FONT_ID_SIZE * 0.5f)) / (g_windowHeight / 2.f)),
					1.5f, 1.f, 1.f, 1.f);

				tutorial = "Shuttles Left ->";
				AEVec2Sub(&tutorial_pos, &planet_vector[0].position, &camera.position);

				// Draw timer at center of planet using position calculated above
				AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()),
					static_cast <f32>((tutorial_pos.x - (static_cast<f32>(tutorial.length()) / 2.f * FONT_ID_SIZE)) / (g_windowWidth / 2.f)),
					static_cast <f32>((tutorial_pos.y - FONT_ID_SIZE * 4.f) / (g_windowHeight / 2.f)),
					1.5f, 1.f, 1.f, 1.f);
			}
		}
	}

	// Tutorial for Planets being added
	if (2 == wave_manager.planet_count && wave_manager.wave_number < 5 && wave_manager.wave_completed)
	{
		tutorial = "A new planet is added every         waves!";
		AEVec2Sub(&tutorial_pos, &planet_vector[1].position, &camera.position);

		// Draw timer at center of planet using position calculated above
		AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()),
			static_cast <f32>((tutorial_pos.x - (static_cast<f32>(tutorial.length()) / 4.5f * FONT_ID_SIZE)) / (g_windowWidth / 2.f)),
			static_cast <f32>((tutorial_pos.y + FONT_ID_SIZE * 5.5f) / (g_windowHeight / 2.f)),
			1.5f, 1.f, 1.f, 1.f);

		tutorial = "4";
		AEVec2Sub(&tutorial_pos, &planet_vector[1].position, &camera.position);

		// Draw timer at center of planet using position calculated above
		AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()),
			static_cast <f32>((tutorial_pos.x + (static_cast<f32>(tutorial.length()) * 4.5f * FONT_ID_SIZE)) / (g_windowWidth / 2.f)),
			static_cast <f32>((tutorial_pos.y + FONT_ID_SIZE * 5.25f) / (g_windowHeight / 2.f)),
			2.5f, 1.f, 0.f, 0.f);

		tutorial = "Notice the drone here? It auto collects debris for you!";
		AEVec2Sub(&tutorial_pos, &planet_vector[1].position, &camera.position);

		// Draw timer at center of planet using position calculated above
		AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()),
			static_cast <f32>((tutorial_pos.x - (static_cast<f32>(tutorial.length()) / 4.5f * FONT_ID_SIZE)) / (g_windowWidth / 2.f)),
			static_cast <f32>((tutorial_pos.y + FONT_ID_SIZE * 4.f) / (g_windowHeight / 2.f)),
			1.5f, 1.f, 1.f, 1.f);

		tutorial = "You can purchase more from the shop!";
		AEVec2Sub(&tutorial_pos, &planet_vector[1].position, &camera.position);

		// Draw timer at center of planet using position calculated above
		AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()),
			static_cast <f32>((tutorial_pos.x - (static_cast<f32>(tutorial.length()) / 4.5f * FONT_ID_SIZE)) / (g_windowWidth / 2.f)),
			static_cast <f32>((tutorial_pos.y + FONT_ID_SIZE * 2.5f) / (g_windowHeight / 2.f)),
			1.5f, 1.f, 1.f, 1.f);

		tutorial = "Now survive as long as you can!";
		AEVec2Sub(&tutorial_pos, &planet_vector[1].position, &camera.position);

		// Draw timer at center of planet using position calculated above
		AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()),
			static_cast <f32>((tutorial_pos.x - (static_cast<f32>(tutorial.length()) / 4.5f * FONT_ID_SIZE)) / (g_windowWidth / 2.f)),
			static_cast <f32>((tutorial_pos.y) / (g_windowHeight / 2.f)),
			1.5f, 1.f, 1.f, 1.f);

		tutorial = "Good luck!";
		AEVec2Sub(&tutorial_pos, &planet_vector[1].position, &camera.position);

		// Draw timer at center of planet using position calculated above
		AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()),
			static_cast <f32>((tutorial_pos.x - (static_cast<f32>(tutorial.length()) / 4.5f * FONT_ID_SIZE)) / (g_windowWidth / 2.f)),
			static_cast <f32>((tutorial_pos.y - FONT_ID_SIZE) / (g_windowHeight / 2.f)),
			1.5f, 1.f, 1.f, 1.f);
	}
}