/******************************************************************************/
/*!
\file		SpaceStation.cpp
\author 	Hwang Jing Rui, Austin
\co-author	Leong Wen Bin Aceson
\par    	email: jingruiaustin.hwang\@digipen.edu
				   leong.w\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the definition of functions for the space station.

			Aceson	- Added cooldown bar and coin for space stations.
					- Added check so that space stations cannot be too close to
					  each other, to prevent overlap
					- Added collision check between space station and Nearest planet 

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/
#include "AEEngine.h"
#include "SpaceStation.h"
#include <vector>
#include "Debris.h"
#include <string>
#include "Camera.h"
#include "Data.h"
#include "GameStateList.h"

//VARIABLES FOR COIN AND COOLDOWN BAR
static float COOLDOWN_HEIGHT;
static float COOLDOWN_WIDTH;
static float COOLDOWN_TIME;
static float COIN_HEIGHT;
static float COIN_WIDTH;

// Textures
extern AEGfxTexture* space_station_tex;
extern AEGfxTexture* shop_icon_tex;

// Variables for SpaceStation
bool space_station_valid_placement = false;
bool space_station_added;
static int BUFFER_SAFE_DISTANCE;
static int MAX_SPACE_STATION_CNT;

// Vector of space stations, planets and debris
std::vector<SpaceStation> space_station_vector;
extern std::vector<Planets> planet_vector;
extern std::vector<std::vector<Debris>> debris_vector_all;

// Variables to print
std::string capacity_spacestation;
extern s8 font_id;
extern Camera camera;

// Vector of cooldown bar
std::vector<Cooldown_Bar> cooldown_vector;

//Coin Sprite
AEGfxTexture* coin_tex;
std::vector<Coin> coin_vector;

//IMPORT DATA VECTOR
std::map<std::string, f32>				SpaceStationDataMap;
std::vector<Data>						SpaceStationData;
extern std::map < std::string, f32> 	GameUIDataMap;

void SpaceStation::load()
{
	coin_tex = AEGfxTextureLoad("Assets/MainLevel/ml_Coin.png");
	ImportDataFromFile("Assets/GameObjectData/SpaceStationData.txt", SpaceStationData, SpaceStationDataMap);
}

void SpaceStation::init()
{
	position.x				= SpaceStationDataMap["Position_X"];
	position.y				= SpaceStationDataMap["Position_Y"];
	size					= SpaceStationDataMap["Size"];
	current_capacity		= static_cast<int>(SpaceStationDataMap["Current_Capacity"]);
	max_capacity			= static_cast<int>(SpaceStationDataMap["Max_Capacity"]);
	initial_spawn			= static_cast<bool>(SpaceStationDataMap["Initial_spawn"]);

	MAX_SPACE_STATION_CNT	= static_cast<int>(GameUIDataMap["Maximum_Space_Station_Level"]);
	COOLDOWN_HEIGHT			= SpaceStationDataMap["CoolDownBar_height"];
	COOLDOWN_WIDTH			= SpaceStationDataMap["CoolDownBar_width"];
	COOLDOWN_TIME			= SpaceStationDataMap["CoolDownBar_time"];
	COIN_HEIGHT				= SpaceStationDataMap["Coin_height"];
	COIN_WIDTH				= SpaceStationDataMap["Coin_width"];
	BUFFER_SAFE_DISTANCE	= static_cast<int>(SpaceStationDataMap["Buffer_safe_distance"]);
}

void SpaceStation::update(Player& current_player, PlayerUI& player_ui)
{
	int safe_position = 0;

	// =========================
	// Update according to input
	// =========================
	
	// Make flag true when left mouse button is not pressed currently
	if (AEInputCheckPrev(AEVK_LBUTTON) && !AEInputCheckCurr(AEVK_LBUTTON))
		player_ui.station_placement_flag = true;

	// If left mouse is triggered, place drone if valid
	if (AEInputCheckTriggered(AEVK_LBUTTON) && player_ui.placing_station && player_ui.station_placement_flag) {
		player_ui.placing_station = false;
	}

	// Make flag false when no longer placing drone
	if (!player_ui.placing_station) {
		player_ui.station_placement_flag = false;
	}

	// =================================
	// Update position of space station
	// =================================

	// if placing space station
	if (player_ui.placing_station) {
		// drone follow mouse pos
		position.x = g_mouseWorld.x;
		position.y = g_mouseWorld.y;


		// =================================
		// COLLISION CHECK BETWEEN STATION
		// =================================
		for (int k = 0; k < space_station_vector.size(); ++k) {
			if (AEVec2Distance(&position, &space_station_vector[k].position) >= size * 5) {
				safe_position += 1;
			}
		}

		// ======================================
		// COLLISION CHECK BETWEEN NEAREST PLANET
		// ======================================
		if ((AEVec2Distance(&current_planet.position, &position) > (current_planet.size)/2 + BUFFER_SAFE_DISTANCE) && safe_position == space_station_vector.size()) {
			if (current_player.space_station_count < MAX_SPACE_STATION_CNT) {
				space_station_valid_placement = true;
				space_station_added = false;
			}
		}
		else {
			space_station_valid_placement = false;
		}
	}

	// ==================================================
	// Update SPACE STATION, COOLDOWN AND COIN instances
	// ==================================================

	// Spawn initial space station
	if (!initial_spawn) {
		position.x = planet_vector[0].position.x;
		position.y = planet_vector[0].position.y + planet_vector[0].size;
		space_station_vector.push_back(*this);

		// Add cooldown bar and coin to initial space station
		Cooldown_Bar cooldown;
		cooldown.height = COOLDOWN_HEIGHT;
		cooldown.width = COOLDOWN_WIDTH;
		cooldown.position.x = this->position.x;
		cooldown.position.y = this->position.y - 35;
		cooldown.timer = 0;
		cooldown.total_time = COOLDOWN_TIME;
		cooldown_vector.push_back(cooldown);

		Coin coin;
		coin.height = COIN_HEIGHT;
		coin.width = COIN_WIDTH;
		coin.position.x = this->position.x;
		coin.position.y = this->position.y + 60;
		coin_vector.push_back(coin);

		// Set initial_spawn flag to true
		initial_spawn = true;
	}

	// If no longer placing space station
	if (!player_ui.placing_station) {
		if (space_station_valid_placement && !space_station_added && current_player.credits >= player_ui.space_station_cost) {
			// Add space station to vector
			space_station_vector.push_back(*this);

			Cooldown_Bar cooldown;
			cooldown.height = COOLDOWN_HEIGHT;
			cooldown.width = COOLDOWN_WIDTH;
			cooldown.position.x = this->position.x;
			cooldown.position.y = this->position.y - 35;
			cooldown.timer = 0;
			cooldown.total_time = COOLDOWN_TIME;
			cooldown_vector.push_back(cooldown);


			Coin coin;
			coin.height = COIN_HEIGHT;
			coin.width = COIN_WIDTH;
			coin.position.x = this->position.x;
			coin.position.y = this->position.y + 60;
			coin_vector.push_back(coin);

			current_player.credits -= player_ui.space_station_cost;
			current_player.space_station_count++;
			space_station_added = true;

			//wave_manager.add_ss_indicator();
		}
	}
	else {
		// ==================================================
		// FINDING NEAREST PLANET BY COMPARING DISTANCES
		// ==================================================
		current_planet = planet_vector[0];
		shortest_distance_spacestation = AEVec2Distance(&position, &planet_vector[0].position);

		for (size_t i = 1; i < planet_vector.size(); ++i) {
			if (AEVec2Distance(&position, &planet_vector[i].position) < shortest_distance_spacestation) {
				current_planet = planet_vector[i];
				shortest_distance_spacestation = AEVec2Distance(&position, &planet_vector[i].position);
			}
		}
	}
	
	// =============================================================
	// Process of space station (Cooldown Bar and Coin pop up logic)
	// =============================================================
	for (int i = 0; i < cooldown_vector.size(); ++i) {
		if (space_station_vector[i].current_capacity > 0 || coin_vector[i].lifespan<=1.0f) {

			if (space_station_vector[i].current_capacity >= 0 && coin_vector[i].lifespan == 0) {
				cooldown_vector[i].timer = 0;
				cooldown_vector[i].width = 0;
			}
			
			cooldown_vector[i].timer += g_dt;
			
			coin_vector[i].lifespan = cooldown_vector[i].timer;

			f32 speed = 80 / cooldown_vector[i].total_time;
			if (space_station_vector[i].current_capacity > 0) {
				cooldown_vector[i].width = cooldown_vector[i].timer * speed;
			}

			//After 1 second, stop drawing coin
			if (cooldown_vector[i].timer > 1) {
				coin_vector[i].is_draw = 0;
			}

			//After finsishing draw coin
			if (cooldown_vector[i].timer > 3 && space_station_vector[i].current_capacity >= 0) {
				space_station_vector[i].current_capacity--;
				current_player.credits += DEBRIS_VALUE;
				current_player.score += DEBRIS_SCORE;
				coin_vector[i].is_draw = 1;
				cooldown_vector[i].timer = 0;
				coin_vector[i].lifespan = 0;
			}			
		}
		else {
			coin_vector[i].is_draw = 0;
			cooldown_vector[i].width = 0;
			cooldown_vector[i].timer = 0;
		}

	}

	// =======================================
	// calculate the matrix for COOLDOWN
	// =======================================

	AEMtx33 scale, rot, trans;

	// For cooldown Bar
	for (int i = 0; i < cooldown_vector.size(); ++i) {
		Cooldown_Bar& cooldown = cooldown_vector[i];
		// cooldown
		AEMtx33Scale(&scale, cooldown.width, cooldown.height); 
		AEMtx33Rot(&rot, 0);
		AEMtx33Trans(&trans, cooldown.position.x, cooldown.position.y);
		AEMtx33Concat(&cooldown.transform, &rot, &scale);
		AEMtx33Concat(&cooldown.transform, &trans, &cooldown.transform);
	}


	// ===============================
	// calculate the matrix for COIN
	// ===============================
	
	for (int i = 0; i < coin_vector.size(); ++i) {
		Coin& coin = coin_vector[i];

		if (coin.is_draw == 1) {
			AEMtx33Scale(&scale, coin.width, coin.height);
			AEMtx33Rot(&rot, 0);
			AEMtx33Trans(&trans, coin.position.x, coin.position.y);
			AEMtx33Concat(&coin.transform, &rot, &scale);
			AEMtx33Concat(&coin.transform, &trans, &coin.transform);
		}
	}

	// =======================================
	// calculate the matrix for space station
	// =======================================
	
	// For UI
	if (player_ui.placing_station) {
		AEMtx33Scale(&scale, size, size);
		AEMtx33Rot(&rot, 0);
		AEMtx33Trans(&trans, position.x, position.y);
		AEMtx33Concat(&transform, &rot, &scale);
		AEMtx33Concat(&transform, &trans, &transform);
	}

	// For active space stations
	for (int i = 0; i < space_station_vector.size(); ++i) {
		SpaceStation& space_station = space_station_vector[i];

		// Space station
		AEMtx33Scale(&scale, space_station.size, space_station.size);
		AEMtx33Rot(&rot, 0);
		AEMtx33Trans(&trans, space_station.position.x, space_station.position.y);
		AEMtx33Concat(&space_station.transform, &rot, &scale);
		AEMtx33Concat(&space_station.transform, &trans, &space_station.transform);
	}
}

/******************************************************************************/
/*!
	Draw space station(s)
*/
/******************************************************************************/
void SpaceStation::draw(AEGfxVertexList* pMesh, PlayerUI player_ui)
{
	// ===========================================
	// Print out current capacity of space station
	// ===========================================

	for (int i = 0; i < space_station_vector.size(); i++) {
		AEVec2 pos;
		AEVec2Sub(&pos, &space_station_vector[i].position, &camera.position);
		capacity_spacestation = std::to_string(space_station_vector[i].current_capacity) + " / " + std::to_string(space_station_vector[i].max_capacity);
		AEGfxPrint(font_id, const_cast<s8*>(capacity_spacestation.c_str()),
			2 * (pos.x - FONT_ID_SIZE) / g_windowWidth,
			2 * (pos.y + (3 * FONT_ID_SIZE)) / g_windowHeight, 1.f, 1.f, 1.f, 1.f);
	}

	// ===========
	// DRAW COIN
	// ===========
	for (int i = 0; i < coin_vector.size(); ++i) {

		Coin& coin = coin_vector[i];
		if (coin.is_draw) {
			
			if (coin.lifespan < 1) {
				AEGfxSetTransparency(1-coin.lifespan);

				AEGfxTextureSet(coin_tex, 0, 0);
				AEGfxSetTransform(coin.transform.m);
				AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
			}
			else {
				AEGfxSetTransparency(0.f);
			}
		}
	}
	AEGfxSetTransparency(1.f);

	// ==============
	// DRAW COOLDOWN
	// ==============
	for (int i = 0; i < cooldown_vector.size(); ++i) {

		Cooldown_Bar& cooldown = cooldown_vector[i];

		AEGfxSetTintColor(1.f, 1.f, 1.f, 1.f);

		AEGfxTextureSet(shop_icon_tex, 0, 0);
		AEGfxSetTransform(cooldown.transform.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	}

	// ===================
	// DRAW SPACE STATION
	// ===================
	// For UI
	if (player_ui.placing_station) {

		//Description for Placeables
		/*std::string	station_text;
		station_text = "Hover the mouse in space to place Space Station";
		AEGfxPrint(font_id, const_cast<s8*>(station_text.c_str()), mouse_pos_world.x, mouse_pos_world.y + 100, 1.f, 1.f, 1.f, 1.f);*/

		if (space_station_valid_placement)
			AEGfxSetTintColor(0.f, 1.f, 0.f, 1.f);
		else
			AEGfxSetTintColor(1.f, 0.3f, 0.3f, 1.f);

		AEGfxTextureSet(space_station_tex, 0, 0);
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	}

	// Reset tint color
	AEGfxSetTintColor(1.f, 1.f, 1.f, 1.f);

	// For active space stations
	for (int i = 0; i < space_station_vector.size(); ++i) {

		SpaceStation& space_station = space_station_vector[i];

		AEGfxTextureSet(space_station_tex, 0, 0);
		AEGfxSetTransform(space_station.transform.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	}
}

void SpaceStation::free()
{
	space_station_vector.clear();
	cooldown_vector.clear();
	coin_vector.clear();

	if (next_state != GS_RESTART) {
		SpaceStationData.clear();
		SpaceStationDataMap.clear();
	}
}

void SpaceStation::unload()
{
	AEGfxTextureUnload(coin_tex);
}
