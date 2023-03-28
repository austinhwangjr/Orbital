/******************************************************************************/
/*!
\file		PlayerProj.cpp
\author 	Hwang Jing Rui, Austin
\par    	email: jingruiaustin.hwang\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the definition of functions for the player 
			projectile.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/
#include <iostream>
#include <fstream>
#include <vector>
#include "AEEngine.h"
#include "PlayerProj.h"
#include "SpaceStation.h"
#include "GameStateManager.h"
#include "Data.h"

// Textures
AEGfxTexture* player_proj_tex;

// Mouse coordinates
extern AEVec2 mouse_pos_world;

// Vector of projectiles, space stations and shop buttons
std::vector<PlayerProj> player_proj_vector;
extern std::vector<SpaceStation> space_station_vector;
extern std::vector<ShopOption> button_vector;

extern f32 cam_x, cam_y;

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
	ImportPlayerDataFromFile("Assets/GameObjectData/PlayerProjectileData.txt", ProjData, ProjDataMap);
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
void PlayerProj::update(f64 frame_time, Player& player, PlayerUI& player_ui)
{
	// =========================
	// Update according to input
	// =========================

	if (AEInputCheckTriggered(AEVK_LBUTTON) && !player_ui.shop_triggered && !player_ui.button_clicked(button_vector[0])) {
		if (player.current_capacity != 0) {
			position = player.position;
			AEVec2Sub(&velocity, &mouse_pos_world, &player.position);
			AEVec2Normalize(&velocity, &velocity);
			AEVec2Scale(&velocity, &velocity, speed);

			player_proj_vector.push_back(*this);
			player.current_capacity--;
		}
	}

	// =====================================
	// Update position of player projectile
	// =====================================

	for (int i = 0; i < player_proj_vector.size(); ++i) {
		PlayerProj& player_proj = player_proj_vector[i];

		player_proj.position.x += player_proj.velocity.x * static_cast<f32>(frame_time);
		player_proj.position.y += player_proj.velocity.y * static_cast<f32>(frame_time);
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
		if (player_proj.position.x > cam_x + AEGetWindowWidth() / 3 || player_proj.position.x < cam_x - AEGetWindowWidth() / 3) {
			if (player_proj.position.y > cam_y + AEGetWindowHeight() / 3 || player_proj.position.y < cam_y - AEGetWindowHeight() / 3) {
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

/******************************************************************************/
/*!
	Custom Functions
*/
/******************************************************************************/
// Import data from txt file
int ImportPlayerProjDataFromFile(const char* FileName, std::vector<Data>& ProjData, std::map<std::string, f32>& ProjDatamap)
{

	std::ifstream ifs{ FileName, std::ios_base::in };
	if (!ifs.is_open()) {											// Check if file exist/open	
		std::cout << FileName << "does not exist." << '\n';
		return 0;
	}

	std::string line;
	while (std::getline(ifs, line)) {
		Data Node;
		std::string word;
		int find_word = 1;

		for (char const ch : line) {

			if (find_word) {
				if (ch == '/') {
					break;
				}

				if (ch == '\n') {
					break;
				}

				if (ch == ' ' || ch == '\t') {
					if (!word.empty()) {    // if ch is a whitespace and word contains some letters
						Node.variable_name = word;
						find_word = 0;
						word.clear();
					}
				}
				else {
					word.push_back(ch);

				}
			}
			else if (!find_word) {
				word.push_back(ch);
			}
		}

		if (find_word == 0) {
			Node.data = word;
			ProjData.push_back(Node);
		}

	}

	for (size_t i = 0; i < ProjData.size(); i++) {
		ProjDatamap[ProjData[i].variable_name] = std::stof(ProjData[i].data);
	}

	ifs.close();

	return 1;
}