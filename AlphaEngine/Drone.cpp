/******************************************************************************/
/*!
\file		Drone.cpp
\author 	Hwang Jing Rui, Austin
\par    	email: jingruiaustin.hwang\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the definition of functions for the Drone.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/
#include "AEEngine.h"
#include "Drone.h"
#include "Planet.h"
#include <vector>
#include <string>
#include "Data.h"
#include <iostream>
#include <fstream>
#include "GameStateManager.h"

// Textures
AEGfxTexture* drone_tex;
extern AEGfxTexture* tractor_beam_tex;
extern AEGfxTexture* shop_icon_tex;

// Variables
bool drone_valid_placement = false;
bool drone_added;

// Mouse coordinates
extern AEVec2 mouse_pos_world;

// Vectors of drones, planets and debris
std::vector<std::vector<Drone>> drone_vector_all;
extern std::vector<Planets> planet_vector;
extern std::vector<std::vector<Debris>> debris_vector_all;

extern s8 font_id;

//IMPORT DATA VECTOR
std::map<std::string, f32> DroneDataMap;
std::vector<Data> DroneData;

/******************************************************************************/
/*!
	Load Textures and Data
*/
/******************************************************************************/
void Drone::load()
{
	drone_tex = AEGfxTextureLoad("Assets/MainLevel/ml_Drone.png");
	ImportPlayerDataFromFile("Assets/GameObjectData/DroneData.txt", DroneData, DroneDataMap);

}

/******************************************************************************/
/*!
	Initialize Variables
*/
/******************************************************************************/
void Drone::init(Player player)
{
	// ======
	// Drone
	// ======
	position.x				= DroneDataMap["Position_X"];
	position.y				= DroneDataMap["Position_Y"];

	velocity.x				= DroneDataMap["Velocity_X"];
	velocity.y				= DroneDataMap["Velocity_Y"];

	size					= player.size;

	rot_speed				= player.rot_speed / 10.f;

	shortest_distance		= DroneDataMap["Shortest_Distance"];

	direction				= player.direction;

	current_capacity		= static_cast<int>(DroneDataMap["Current_Capacity"]);
	max_capacity			= static_cast<int>(DroneDataMap["Max_Capacity"]);

	// =============
	// Cooldown Bar
	// =============
	cd_bar.position.x		= position.x;
	cd_bar.position.y		= position.y - size;
	cd_bar.height			= DroneDataMap["CoolDownBar_height"];
	cd_bar.width			= DroneDataMap["CoolDownBar_width"];
	cd_bar.max_width		= DroneDataMap["CoolDownBar_max_width"];
	cd_bar.timer			= DroneDataMap["CoolDownBar_timer"];
	cd_bar.total_time		= DroneDataMap["CoolDownBar_total_time"];

	// =============
	// Tractor Beam
	// =============
	beam_pos.x				= DroneDataMap["TractorBeam_position_X"];
	beam_pos.y				= DroneDataMap["TractorBeam_position_Y"];

	beam_width				= size;
	beam_height				= beam_width * 3 / 2;
}

/******************************************************************************/
/*!
	Update Drone
*/
/******************************************************************************/
void Drone::update(f64 frame_time, Player& player, PlayerUI& player_ui)
{
	// =========================
	// Update according to input
	// =========================
	
	// Make flag true when left mouse button is not pressed currently
	if (AEInputCheckPrev(AEVK_LBUTTON) && !AEInputCheckCurr(AEVK_LBUTTON))
		player_ui.drone_placement_flag = true;

	// If left mouse is triggered, place drone if valid
	if (AEInputCheckTriggered(AEVK_LBUTTON) && player_ui.placing_drone && player_ui.drone_placement_flag) {
		player_ui.placing_drone = false;
	}

	// Make flag false when no longer placing drone
	if (!player_ui.placing_drone) {
		player_ui.drone_placement_flag = false;
	}

	// =========================
	// Update position of drone
	// =========================

	// If placing drone
	if (player_ui.placing_drone) {
		// Drone not added yet
		drone_added = false;

		// Drone to follow mouse position
		position.x = mouse_pos_world.x;
		position.y = mouse_pos_world.y;

		// if within planet's orbit, drone can be placed
		if (AEVec2Distance(&current_planet.position, &position) <= (static_cast<f32>(current_planet.size) / 2.f + current_planet.orbit_range)) {
			if (current_planet.current_drones < DRONES_MAX) {
				direction = static_cast<f32>(atan2(static_cast<double>(position.y - current_planet.position.y),
					static_cast<double>(position.x - current_planet.position.x)));
				position.x = current_planet.position.x + (static_cast<f32>(current_planet.size) / 2.f + current_planet.orbit_range) * AECos(direction);
				position.y = current_planet.position.y + (static_cast<f32>(current_planet.size) / 2.f + current_planet.orbit_range) * AESin(direction);
				beam_pos.x = position.x - AECos(direction) * ((beam_height + size) / 2);
				beam_pos.y = position.y - AESin(direction) * ((beam_height + size) / 2);

				drone_valid_placement = true;
			}
			else
				drone_valid_placement = false;
		}
		else
			drone_valid_placement = false;
	}

	// Each active drone to orbit around planet
	for (size_t i = 0; i < drone_vector_all.size(); ++i) {
		for (size_t j = 0; j < drone_vector_all[i].size(); j++) {
			Drone& drone = drone_vector_all[i][j];

			// Set drone position
			drone.position.x = drone.current_planet.position.x +
				(static_cast<f32>(drone.current_planet.size) / 2.f + drone.current_planet.orbit_range) * AECos(drone.direction);
			drone.position.y = drone.current_planet.position.y +
				(static_cast<f32>(drone.current_planet.size) / 2.f + drone.current_planet.orbit_range) * AESin(drone.direction);

			// Set drone direction
			drone.direction += drone.rot_speed * static_cast<f32>(frame_time);

			// Drone's tractor beam
			drone.beam_pos.x = drone.position.x - AECos(drone.direction) * ((drone.beam_height + drone.size) / 2);
			drone.beam_pos.y = drone.position.y - AESin(drone.direction) * ((drone.beam_height + drone.size) / 2);
		}
	}

	// ================================
	// check for beam-debris collision
	// ================================

	for (size_t i = 0; i < drone_vector_all.size(); ++i) {
		for (size_t j = 0; j < drone_vector_all[i].size(); j++) {
			Drone& drone = drone_vector_all[i][j];

			if (debris_vector_all[i].size() > 0) {
				for (int k = 0; k < debris_vector_all[i].size(); ++k) {
					Debris& debris = debris_vector_all[i][k];

					if (debris.active) {
						// Debris to move towards drone when in contact with beam
						if ((drone.current_capacity < drone.max_capacity) &&
							AEVec2Distance(&drone.beam_pos, &debris.position) <= drone.beam_height / 2) {
							debris.move_towards_drone_id = static_cast<int>(j + 1);
							debris.state = MOVE_TOWARDS_DRONE;
						}
						// Not colliding with player beam or drone beam
						else if (debris.state != MOVE_TOWARDS_PLAYER && debris.state == MOVE_TOWARDS_DRONE)
							debris.state = MOVE_TOWARDS_PLANET;

						if (debris.move_towards_drone_id > 0 && (drone.current_capacity < drone.max_capacity) &&
							AEVec2Distance(&drone.position, &debris.position) <= (drone.size + debris.size) / 2) {
							// Debris to be destroyed when in contact with player
							drone.current_capacity++;
							debris.active = false;
						}
					}
				}
			}
		}
	}

	// =======================
	// update drone instances
	// =======================

	if (player_ui.placing_drone) {
		// Determine planet closest to drone
		current_planet = planet_vector[0];
		shortest_distance = AEVec2Distance(&position, &planet_vector[0].position);

		for (size_t i = 1; i < planet_vector.size(); ++i) {
			if (AEVec2Distance(&position, &planet_vector[i].position) < shortest_distance) {
				current_planet = planet_vector[i];
				shortest_distance = AEVec2Distance(&position, &planet_vector[i].position);
			}
		}
	}

	else if (drone_valid_placement && !drone_added && player.credits >= player_ui.drone_cost) {
		// Add drone to vector
		planet_vector[current_planet.id].current_drones++;
		drone_vector_all[current_planet.id].push_back(*this);
		player.credits -= player_ui.drone_cost;
		drone_added = true;
	}

	// ===================
	// Cooldown bar logic
	// ===================

	for (size_t i = 0; i < drone_vector_all.size(); ++i) {
		for (size_t j = 0; j < drone_vector_all[i].size(); j++) {
			Drone& drone = drone_vector_all[i][j];

			// Drone to process debris only when it has at least 1 debris
			if (drone.current_capacity > 0) {

				drone.cd_bar.timer += static_cast<f32>(frame_time);

				f32 speed = drone.cd_bar.max_width / drone.cd_bar.total_time;
				if (drone.current_capacity > 0) {
					drone.cd_bar.width = drone.cd_bar.timer * speed;
				}

				// After processing debris, reset timer
				if (drone.cd_bar.timer > drone.cd_bar.total_time && drone.current_capacity >= 0) {
					drone.current_capacity--;
					drone.cd_bar.timer = 0;
					player.credits += DEBRIS_VALUE / 2;
					player.score += DEBRIS_SCORE / 2;
				}
			}
			else {
				drone.cd_bar.width = 0;
				drone.cd_bar.timer = 0;
			}

			drone.cd_bar.position.x = drone.position.x;
			drone.cd_bar.position.y = drone.position.y + drone.size;
		}
	}

	// ========================================
	// calculate the matrix for drone and beam
	// ========================================

	AEMtx33 scale, rot, trans;

	// For UI
	if (player_ui.placing_drone) {
		AEMtx33Scale(&scale, size, size);
		AEMtx33Rot(&rot, direction + PI / 2);
		AEMtx33Trans(&trans, position.x, position.y);
		AEMtx33Concat(&drone_transform, &rot, &scale);
		AEMtx33Concat(&drone_transform, &trans, &drone_transform);
	}

	// For active drones
	for (size_t i = 0; i < drone_vector_all.size(); ++i) {
		for (size_t j = 0; j < drone_vector_all[i].size(); j++) {
			Drone& drone = drone_vector_all[i][j];

			// Drone
			AEMtx33Scale(&scale, drone.size, drone.size);
			AEMtx33Rot(&rot, drone.direction + PI / 2);
			AEMtx33Trans(&trans, drone.position.x, drone.position.y);
			AEMtx33Concat(&drone.drone_transform, &rot, &scale);
			AEMtx33Concat(&drone.drone_transform, &trans, &drone.drone_transform);

			// Cooldown bar
			AEMtx33Scale(&scale, drone.cd_bar.width, drone.cd_bar.height);
			AEMtx33Rot(&rot, 0);
			AEMtx33Trans(&trans, drone.cd_bar.position.x, drone.cd_bar.position.y);
			AEMtx33Concat(&drone.cd_bar.transform, &rot, &scale);
			AEMtx33Concat(&drone.cd_bar.transform, &trans, &drone.cd_bar.transform);

			// Tractor beam
			AEMtx33Scale(&scale, drone.size, drone.size * 3 / 2);
			AEMtx33Rot(&rot, drone.direction + PI / 2);
			AEMtx33Trans(&trans, drone.beam_pos.x, drone.beam_pos.y);
			AEMtx33Concat(&drone.beam_transform, &rot, &scale);
			AEMtx33Concat(&drone.beam_transform, &trans, &drone.beam_transform);
		}
	}
}

/******************************************************************************/
/*!
	Draw drone(s)
*/
/******************************************************************************/
void Drone::draw(AEGfxVertexList* pMesh, PlayerUI player_ui)
{
	// For UI
	if (player_ui.placing_drone) {
		if (drone_valid_placement)
			AEGfxSetTintColor(0.f, 1.f, 0.f, 1.f);
		else
			AEGfxSetTintColor(1.f, 0.3f, 0.3f, 1.f);

		AEGfxTextureSet(drone_tex, 0, 0);
		AEGfxSetTransform(drone_transform.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	}

	// Reset tint color
	AEGfxSetTintColor(1.f, 1.f, 1.f, 1.f);

	// For active drones
	for (size_t i = 0; i < drone_vector_all.size(); ++i) {
		for (size_t j = 0; j < drone_vector_all[i].size(); j++) {
			Drone& drone = drone_vector_all[i][j];

			// Drone
			AEGfxTextureSet(drone_tex, 0, 0);
			AEGfxSetTransform(drone.drone_transform.m);
			AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
			
			// Cooldown bar
			AEGfxTextureSet(shop_icon_tex, 0, 0);
			AEGfxSetTransform(drone.cd_bar.transform.m);
			AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

			// Tractor beam
			AEGfxTextureSet(tractor_beam_tex, 0, 0);
			AEGfxSetTransform(drone.beam_transform.m);
			AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
		}
	}
}

/******************************************************************************/
/*!
	Clean Object Instances
*/
/******************************************************************************/
void Drone::free()
{
	for (int i = 0; i < drone_vector_all.size(); ++i)
		drone_vector_all[i].clear();
	
	drone_vector_all.clear();

	if (next_state != GS_RESTART) {
		DroneData.clear();
		DroneDataMap.clear();
	}
}

/******************************************************************************/
/*!
	Free Textures
*/
/******************************************************************************/
void Drone::unload()
{
	AEGfxTextureUnload(drone_tex);

}

/******************************************************************************/
/*!
	Additional Functions
*/
/******************************************************************************/
// Import data from txt file
int ImportDroneDataFromFile(const char* FileName, std::vector<Data>& DroneData, std::map<std::string, f32>& DroneDatamap)
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
			DroneData.push_back(Node);
		}

	}

	for (size_t i = 0; i < DroneData.size(); i++) {
		DroneDatamap[DroneData[i].variable_name] = std::stof(DroneData[i].data);
	}

	ifs.close();

	return 1;
}