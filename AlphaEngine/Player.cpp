/******************************************************************************/
/*!
\file		Player.cpp
\author 	Hwang Jing Rui, Austin
\par    	email: jingruiaustin.hwang\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the definition of functions for the player.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/
#include "AEEngine.h"
#include "Player.h"
#include "Easing.h"
#include "Data.h"
#include "GameStateList.h"

// Textures
AEGfxTexture* player_tex;
AEGfxTexture* tractor_beam_tex;

// Vectors of planets and debris
extern std::vector<Planets> 			planet_vector;
extern std::vector<std::vector<Debris>> debris_vector_all;

// IMPORT DATA VECTOR
std::map<std::string, f32> 	PlayerDataMap;
std::vector<Data> 			PlayerData;

/******************************************************************************/
/*!
	Load Textures and Data
*/
/******************************************************************************/
void Player::load()
{
	// Load textures
	player_tex			= AEGfxTextureLoad("Assets/MainLevel/ml_Spaceship2.png");
	tractor_beam_tex	= AEGfxTextureLoad("Assets/MainLevel/ml_TractorBeam.png");

	// Import data from file
	ImportDataFromFile("Assets/GameObjectData/PlayerData.txt", PlayerData, PlayerDataMap);
}

/******************************************************************************/
/*!
	Initialize Variables
*/
/******************************************************************************/
void Player::init()
{
	// =======
	// Player
	// =======
	state					= PLAYER_FLY;

	position.x				= PlayerDataMap["position.x"];
	position.y				= PlayerDataMap["position.y"];

	velocity.x				= PlayerDataMap["velocity.x"];
	velocity.y				= PlayerDataMap["velocity.y"];

	size					= PlayerDataMap["size"];

	mov_speed				= PlayerDataMap["mov_speed"];
	rot_speed				= PlayerDataMap["rot_speed"] * PI;

	shortest_distance		= PlayerDataMap["shortest_distance"];

	direction				= PlayerDataMap["direction"];

	current_capacity		= static_cast<int>(PlayerDataMap["current_capacity"]);
	max_capacity			= static_cast<int>(PlayerDataMap["max_capacity"]);

	can_leave_orbit			= true;
	beam_active 			= false;

	timer					= PlayerDataMap["timer"];
	max_timer				= PlayerDataMap["max_timer"];

	// ==============
	// Score-keeping
	// ==============
	score					= PlayerDataMap["score"];
	credits					= PlayerDataMap["credits"];

	// ===============
	// Upgrade Levels
	// ===============
	mov_speed_level			= PlayerDataMap["mov_speed_level"];
	capacity_level			= PlayerDataMap["capacity_level"];
	space_station_count		= PlayerDataMap["space_station_count"];
	beam_level				= PlayerDataMap["beam_level"];

	// =============
	// Tractor Beam
	// =============
	beam_pos.x				= PlayerDataMap["beam_pos.x"];
	beam_pos.y				= PlayerDataMap["beam_pos.y"];

	beam_width				= size * 0.6f;
	beam_height				= beam_width * 2.f;
}

/******************************************************************************/
/*!
	Update Player
*/
/******************************************************************************/
void Player::update(f32 frame_time)
{
	// Player is in orbit state
	if (state == PLAYER_ORBIT)
		orbit_state(frame_time);

	// Player is in transit state
	if (state == PLAYER_TRANSIT)
		transit_state(frame_time);

	// Player is in free-flying state
	if (state == PLAYER_FLY)
		flying_state(frame_time);

	// =========================================
	// Calculate the matrix for player and beam
	// =========================================
	
	AEMtx33 scale, rot, trans;

	// Player
	AEMtx33Scale(&scale, size, size);
	AEMtx33Rot(&rot, direction + PI / 2);
	AEMtx33Trans(&trans, position.x, position.y);
	AEMtx33Concat(&player_transform, &rot, &scale);
	AEMtx33Concat(&player_transform, &trans, &player_transform);

	// Tractor beam
	AEMtx33Scale(&scale, beam_width, beam_height);
	AEMtx33Rot(&rot, direction + PI / 2);
	AEMtx33Trans(&trans, beam_pos.x, beam_pos.y);
	AEMtx33Concat(&beam_transform, &rot, &scale);
	AEMtx33Concat(&beam_transform, &trans, &beam_transform);

}

/******************************************************************************/
/*!
	Draw Player and Tractor Beam
*/
/******************************************************************************/
void Player::draw(AEGfxVertexList* pMesh)
{
	AEGfxTextureSet(player_tex, 0, 0);
	AEGfxSetTransform(player_transform.m);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

	// tractor beam
	if (AEInputCheckCurr(AEVK_SPACE) && state == PLAYER_ORBIT) {
		AEGfxTextureSet(tractor_beam_tex, 0, 0);
		AEGfxSetTransform(beam_transform.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	}

}

/******************************************************************************/
/*!
	Clean Object Instances
*/
/******************************************************************************/
void Player::free()
{	
	if (next_state != GS_RESTART) {
		PlayerData.clear();
		PlayerDataMap.clear();
	}
}

/******************************************************************************/
/*!
	Free Textures
*/
/******************************************************************************/
void Player::unload()
{
	AEGfxTextureUnload(player_tex);
	AEGfxTextureUnload(tractor_beam_tex);
}

/******************************************************************************/
/*!
	Additional Functions
*/
/******************************************************************************/
// Orbit State of Player
void Player::orbit_state(f32 frame_time)
{
	// ================
	// Check for input
	// ================

	if (AEInputCheckCurr(AEVK_A)) {
		direction += (rot_speed / 2) * static_cast<f32>(frame_time);

		position.x = current_planet.position.x + (static_cast<f32>(current_planet.size) / 2 + current_planet.orbit_range) * AECos(direction);
		position.y = current_planet.position.y + (static_cast<f32>(current_planet.size) / 2 + current_planet.orbit_range) * AESin(direction);
	}

	if (AEInputCheckCurr(AEVK_D)) {
		direction -= (rot_speed / 2) * static_cast<f32>(frame_time);

		position.x = current_planet.position.x + (static_cast<f32>(current_planet.size) / 2 + current_planet.orbit_range) * AECos(direction);
		position.y = current_planet.position.y + (static_cast<f32>(current_planet.size) / 2 + current_planet.orbit_range) * AESin(direction);
	}

	if (AEInputCheckPrev(AEVK_W))
		can_leave_orbit = false;
	else
		can_leave_orbit = true;

	if (AEInputCheckCurr(AEVK_W) && can_leave_orbit) {
		AEVec2Zero(&velocity);
		state = PLAYER_TRANSIT;
	}

	// Draw tractor beam
	if (AEInputCheckCurr(AEVK_SPACE))
		beam_active = true;
	else
		beam_active = false;

	// ================================
	// Update player and beam position
	// ================================

	if (AEInputCheckCurr(AEVK_W) && can_leave_orbit) {
		position.x += AECos(direction);
		position.y += AESin(direction);
	}
	else {
		position.x = current_planet.position.x + (static_cast<f32>(current_planet.size) / 2.f + current_planet.orbit_range) * AECos(direction);
		position.y = current_planet.position.y + (static_cast<f32>(current_planet.size) / 2.f + current_planet.orbit_range) * AESin(direction);
	}

	if (beam_active) {
		beam_pos.x = position.x - AECos(direction) * ((beam_height + size) / 2);
		beam_pos.y = position.y - AESin(direction) * ((beam_height + size) / 2);
	}

	// ================================
	// Check for beam-debris collision
	// ================================
	
	if (beam_active) {
		// Check for collision between tractor beam and debris
		for (int i = 0; i < debris_vector_all[current_planet.id].size(); ++i) {
			Debris& debris = debris_vector_all[current_planet.id][i];
			
			if (debris.active) {
				// Debris to move towards player when in contact with beam	
				if ((current_capacity < max_capacity + capacity_level) && 
					AEVec2Distance(&beam_pos, &debris.position) <= beam_height / 2) {
					debris.state = MOVE_TOWARDS_PLAYER;
				}
				// Beam active, but not colliding with player beam or drone beam
				else if (debris.state == MOVE_TOWARDS_PLAYER && debris.state != MOVE_TOWARDS_DRONE)
					debris.state = MOVE_TOWARDS_PLANET;

				if ((current_capacity < max_capacity + capacity_level) && AEVec2Distance(&position, &debris.position) <= (size + debris.size) / 2) {
					// Debris to be destroyed when in contact with player
					current_capacity++;
					debris.active = false;
				}
			}
		}
	}
	else {
		for (int i = 0; i < debris_vector_all[current_planet.id].size(); ++i) {
			Debris& debris = debris_vector_all[current_planet.id][i];
			// Beam no longer active, if debris was moving towards player, make it move towards planet
			if (debris.state == MOVE_TOWARDS_PLAYER)
				debris.state = MOVE_TOWARDS_PLANET;
		}
	}
}

// Transit State of Player
void Player::transit_state(f32 frame_time)
{
	// ================
	// Check for input
	// ================

	if (AEInputCheckCurr(AEVK_W)) {
		AEVec2 added;
		AEVec2Set(&added, AECos(direction), AESin(direction));

		// Find the velocity according to the acceleration
		AEVec2Scale(&added, &added, mov_speed / 2.f);
		velocity.x = velocity.x + added.x * static_cast<f32>(frame_time);
		velocity.y = velocity.y + added.y * static_cast<f32>(frame_time);

		// Limit player's speed
		AEVec2Scale(&velocity, &velocity, 0.99f);

		// Add to timer. Change to flying state after 1s
		timer += static_cast<f32>(frame_time);
		if (timer >= max_timer) {
			// Change state and reset timer
			state = PLAYER_FLY;
			timer = 0.f;
		}
	}
	
	else {
		// Move player back to orbit
		AEVec2 diff;
		AEVec2Sub(&diff, &current_planet.position, &position);
		AEVec2Normalize(&diff, &diff);
		AEVec2Scale(&diff, &diff, mov_speed * static_cast<f32>(frame_time));
		AEVec2Add(&position, &position, &diff);

		timer -= static_cast<f32>(frame_time);

		// Debris to rotate around planet when in orbit range
		if (AEVec2Distance(&current_planet.position, &position) <= (current_planet.size / 2 + current_planet.orbit_range)) {
			// Change state and reset timer
			state = PLAYER_ORBIT;
			timer = 0.f;
		}
	}

	// =======================
	// Update player position
	// =======================

	position.x = position.x + velocity.x * static_cast<f32>(frame_time);
	position.y = position.y + velocity.y * static_cast<f32>(frame_time);
}

// Flying State of Player
void Player::flying_state(f32 frame_time)
{
	// ================
	// Check for input
	// ================

	if (AEInputCheckCurr(AEVK_W))
	{
		AEVec2 added;
		AEVec2Set(&added, AECos(direction), AESin(direction));

		// Find the velocity according to the acceleration
		f32 easing = easeOutQuad(static_cast<f32>(frame_time));
		AEVec2Scale(&added, &added, mov_speed * static_cast<f32>(mov_speed_level + 1) / 2.f * easing);
		velocity.x = velocity.x + added.x;
		velocity.y = velocity.y + added.y;

		// Limit player's speed
		AEVec2Scale(&velocity, &velocity, 0.99f);
	}

	if (AEInputCheckCurr(AEVK_S))
	{
		AEVec2 added;
		AEVec2Set(&added, -AECos(direction), -AESin(direction));

		// Find the velocity according to the decceleration
		f32 easing = easeOutQuad(static_cast<f32>(frame_time));
		AEVec2Scale(&added, &added, mov_speed * static_cast<f32>(mov_speed_level + 1) / 2.f * easing);
		velocity.x = velocity.x + added.x;
		velocity.y = velocity.y + added.y;

		// Limit player's speed
		AEVec2Scale(&velocity, &velocity, 0.99f);
	}

	if (AEInputCheckCurr(AEVK_A)) {
		direction += rot_speed * static_cast<f32>(frame_time);
		direction = AEWrap(direction, -PI, PI);
	}

	if (AEInputCheckCurr(AEVK_D)) {
		direction -= rot_speed * static_cast<f32>(frame_time);
		direction = AEWrap(direction, -PI, PI);
	}

	if (AEVec2Distance(&current_planet.position, &position) <= (current_planet.size / 2 + current_planet.orbit_range)) {
		direction = static_cast<f32>(atan2(position.y - current_planet.position.y, position.x - current_planet.position.x));
		state = PLAYER_ORBIT;
	}

	// =======================
	// Update player position
	// =======================

	position.x = position.x + velocity.x * static_cast<f32>(frame_time);
	position.y = position.y + velocity.y * static_cast<f32>(frame_time);	

	// ===================================
	// Update active game object instances
	// ===================================

	/* if (0 <= Halo_Timer)
	{
		Halo_Timer -= frame_time;
	}*/

	// Determine planet closest to player
	current_planet = planet_vector[0];
	shortest_distance = AEVec2Distance(&position, &planet_vector[0].position);

	for (size_t i = 1; i < planet_vector.size(); ++i) {
		if (AEVec2Distance(&position, &planet_vector[i].position) < shortest_distance) {
			current_planet = planet_vector[i];
			shortest_distance = AEVec2Distance(&position, &planet_vector[i].position);
		}
	}

	// Player not in planet orbit, debris to return to planet
	for (int i = 0; i < debris_vector_all[current_planet.id].size(); ++i) {
		Debris& debris = debris_vector_all[current_planet.id][i];
		if (debris.state == MOVE_TOWARDS_PLAYER)
			debris.state = MOVE_TOWARDS_PLANET;
	}
}
