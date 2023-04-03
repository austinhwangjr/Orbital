/******************************************************************************/
/*!
\file		PlayerProj.cpp
\author 	Hwang Jing Rui, Austin (90%)
\co-author	Leong Wen Bin, Aceson (5%)
			Ong You Yang (5%)
\par    	email: jingruiaustin.hwang\@digipen.edu
				   leong.w\@digipen.edu
				   youyang.o\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the definition of functions for the player 
			projectile.

			Aceson - File I/O and Deletion Of Projectile upon Off-Screen

			You Yang - Audio-related aspects

All content (C) 2023 DigiPen Institute of Technology Singapore. All rights reserved.
 */
/******************************************************************************/
#include <iostream>
#include <fstream>
#include <vector>
#include "AEEngine.h"
#include "PlayerProj.h"
#include "SpaceStation.h"
#include "GameStateList.h"
#include "Data.h"
#include "AudioManager.h"

// Textures
AEGfxTexture* player_proj_tex;

// Vector of projectiles, space stations and shop buttons
std::vector<PlayerProj> player_proj_vector;
extern std::vector<SpaceStation> space_station_vector;
extern std::vector<ShopOption> button_vector;

//IMPORT DATA VECTOR
std::map<std::string, f32> ProjDataMap;
std::vector<Data> ProjData;

/******************************************************************************/
/*!
	Load Textures and Data
*/
/******************************************************************************/
void PlayerProj::load()
{
	// Load texture
	player_proj_tex = AEGfxTextureLoad("Assets/MainLevel/ml_Debris.png");

	// Import data from file
	ImportDataFromFile("Assets/GameObjectData/PlayerProjectileData.txt", ProjData, ProjDataMap);

	// Load sound
	AudioManager::LoadSound("Assets/BGM/bgm_ml_PlayerShootingDebris.mp3", false);
}

/******************************************************************************/
/*!
	Initialize Variables
*/
/******************************************************************************/
void PlayerProj::init()
{
	position.x		= ProjDataMap["Position_X"];
	position.y		= ProjDataMap["Position_Y"];

	velocity.x		= ProjDataMap["Velocity_X"];
	velocity.y		= ProjDataMap["Velocity_Y"];

	size			= ProjDataMap["Size"];

	speed			= ProjDataMap["Speed"];

	direction		= ProjDataMap["Direction"];

	is_delete		= static_cast<int>(ProjDataMap["Delete_flag"]);
}

/******************************************************************************/
/*!
	Update Player Projectile
*/
/******************************************************************************/
void PlayerProj::update(Player& current_player, PlayerUI& player_ui)
{
	// =========================
	// Update according to input
	// =========================

	if (AEInputCheckTriggered(AEVK_LBUTTON) && !player_ui.shop_triggered && !player_ui.button_clicked(button_vector[0])) {
		if (current_player.current_capacity != 0) {
			position = current_player.position;
			AEVec2Sub(&velocity, &g_mouseWorld, &current_player.position);
			AEVec2Normalize(&velocity, &velocity);
			AEVec2Scale(&velocity, &velocity, speed);

			player_proj_vector.push_back(*this);
			current_player.current_capacity--;

			// Play shooting sound effect
			AudioManager::PlayOnce("Assets/BGM/bgm_ml_PlayerShootingDebris.mp3", 0.05f);
		}
	}


	// =====================================
	// Update position of player projectile
	// =====================================

	for (int i = 0; i < player_proj_vector.size(); ++i) {
		PlayerProj& player_proj = player_proj_vector[i];

		player_proj.position.x += player_proj.velocity.x * g_dt;
		player_proj.position.y += player_proj.velocity.y * g_dt;
	}

	// ====================
	// Check for collision
	// ====================

	for (int i = 0; i < player_proj_vector.size(); ++i) {
		for (int j = 0; j < space_station_vector.size(); j++) {

			// If space station has space to hold more debris
			if (space_station_vector[j].current_capacity < space_station_vector[j].max_capacity) {
				f32 radius = size / 2 + space_station_vector[j].size / 2;

				if (AEVec2Distance(&player_proj_vector[i].position, &space_station_vector[j].position) <= radius) {
					player_proj_vector[i].is_delete = 1;
					space_station_vector[j].current_capacity += 1;
				}
			}
		}
	}

	// ======================================================
	// Delete player projectile if outside screen boundaries
	// ======================================================
	for (int i = 0; i < player_proj_vector.size(); ++i) {
		PlayerProj& player_proj = player_proj_vector[i];
		if (player_proj.position.x > g_camPos.x + g_windowWidth / 3 || player_proj.position.x < g_camPos.x - g_windowWidth / 3) {
			if (player_proj.position.y > g_camPos.y + g_windowHeight / 3 || player_proj.position.y < g_camPos.y - g_windowHeight / 3) {
				player_proj.is_delete = 1;
			}
		}
	}

	// ===================================
	// Update player projectile instances
	// ===================================

	//Erase projectile upon collision
	for (int i = 0; i < player_proj_vector.size(); i++) {
		if (player_proj_vector[i].is_delete == 1) {
			player_proj_vector.erase(player_proj_vector.begin() + i);
		}
	}

	// ===========================================
	// Calculate the matrix for player projectile
	// ===========================================

	AEMtx33 scale, rot, trans;

	for (int i = 0; i < player_proj_vector.size(); ++i) {
		PlayerProj& player_proj = player_proj_vector[i];

		// Space station
		AEMtx33Scale(&scale, player_proj.size, player_proj.size);
		AEMtx33Rot(&rot, 0);
		AEMtx33Trans(&trans, player_proj.position.x, player_proj.position.y);
		AEMtx33Concat(&player_proj.transform, &rot, &scale);
		AEMtx33Concat(&player_proj.transform, &trans, &player_proj.transform);
	}
}

/******************************************************************************/
/*!
	Draw Player Projectile
*/
/******************************************************************************/
void PlayerProj::draw(AEGfxVertexList* pMesh)
{
	for (int i = 0; i < player_proj_vector.size(); ++i) {

		PlayerProj& player_proj = player_proj_vector[i];

		AEGfxSetTintColor(1.f, 1.f, 1.f, 1.f);

		AEGfxTextureSet(player_proj_tex, 0, 0);
		AEGfxSetTransform(player_proj.transform.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	}
}

/******************************************************************************/
/*!
	Clean Object Instances
*/
/******************************************************************************/
void PlayerProj::free()
{
	player_proj_vector.clear();

	if (next_state != GS_RESTART) {
		ProjData.clear();
		ProjDataMap.clear();
	}
}

/******************************************************************************/
/*!
	Free Textures
*/
/******************************************************************************/
void PlayerProj::unload()
{
	AEGfxTextureUnload(player_proj_tex);
}

