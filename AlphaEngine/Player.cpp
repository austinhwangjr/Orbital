#include "AEEngine.h"
#include "Player.h"
#include <iostream>

AEGfxTexture* player_tex;
//Player player;

extern std::vector<Planets> planet_vector;

f32* cam_x;
f32* cam_y;

bool player_leave_orbit;

// Test for multiple planet check
f32 shortest_distance;

// Tractor beam test
AEGfxTexture* tractor_beam_tex;

// Test some variables
f32 collision_x;
f32 collision_y;

extern std::vector<std::vector<Debris>> debris_vector_all;

void Player::load()
{
	// load texture
	player_tex			= AEGfxTextureLoad("Assets/test-player.png");
	tractor_beam_tex	= AEGfxTextureLoad("Assets/TractorBeam.png");
}

void Player::init()
{
	position.x			= 100.f;
	position.y			= 100.f;
	velocity.x			= 0.f;
	velocity.y			= 0.f;
	size				= 20.f;
	direction			= 0.f;
	current_capacity	= 0;
	max_capacity		= 5;

	// variables
	dist_from_planet	= 50.f;
	mov_speed			= 150.f;
	rot_speed			= 1.5f * PI;
	speed_upgrade		= 1.0f;

	state = PLAYER_FLY;
	player_leave_orbit	= true;

	shortest_distance	= 0.f;

	// tractor beam
	beam_pos.x			= 0.f;
	beam_pos.y			= 0.f;

	collision_x			= 0.f;
	collision_y			= 0.f;
}

void Player::update(f64 frame_time)
{
	if (state == PLAYER_ORBIT) {
		position.x = current_planet.position.x + (current_planet.size / 2 + dist_from_planet) * AECos(direction);
		position.y = current_planet.position.y + (current_planet.size / 2 + dist_from_planet) * AESin(direction);

		if (AEInputCheckCurr(AEVK_A)) {
			direction += (rot_speed / 2) * speed_upgrade * static_cast<f32>(frame_time);
		}

		if (AEInputCheckCurr(AEVK_D)) {
			direction -= (rot_speed / 2) * speed_upgrade * static_cast<f32>(frame_time);
		}

		if (AEInputCheckPrev(AEVK_W))
			player_leave_orbit = false;
		else
			player_leave_orbit = true;

		if (AEInputCheckCurr(AEVK_W) && player_leave_orbit) {
			position.x += AECos(direction);
			position.y += AESin(direction);

			AEVec2Zero(&velocity);

			state = PLAYER_FLY;
		}
	}

	// Player is in free-flying mode
	else if (state == PLAYER_FLY) {

		current_planet = planet_vector[0];
		shortest_distance = AEVec2Distance(&position, &planet_vector[0].position);

		for (size_t i = 1; i < planet_vector.size(); ++i) {
			if (AEVec2Distance(&position, &planet_vector[i].position) < shortest_distance) {
				current_planet = planet_vector[i];
				shortest_distance = AEVec2Distance(&position, &planet_vector[i].position);
			}
		}

		// INPUT-------------------------------------

		if (AEInputCheckCurr(AEVK_W)) {
			AEVec2 added;
			AEVec2Set(&added, AECos(direction), AESin(direction));

			// Find the velocity according to the acceleration
			// Limit your speed over here
			AEVec2Scale(&added, &added, mov_speed * speed_upgrade);
			velocity.x = velocity.x + added.x * static_cast<f32>(frame_time);
			velocity.y = velocity.y + added.y * static_cast<f32>(frame_time);
			AEVec2Scale(&velocity, &velocity, 0.99f);
		}

		if (AEInputCheckCurr(AEVK_S)) {
			AEVec2 added;
			AEVec2Set(&added, -AECos(direction), -AESin(direction));

			// Find the velocity according to the decceleration
			// Limit your speed over here
			AEVec2Scale(&added, &added, mov_speed * speed_upgrade);
			velocity.x = velocity.x + added.x * static_cast<f32>(frame_time);
			velocity.y = velocity.y + added.y * static_cast<f32>(frame_time);
			AEVec2Scale(&velocity, &velocity, 0.99f);
		}

		if (AEInputCheckCurr(AEVK_A)) {
			direction += rot_speed * speed_upgrade * static_cast<f32>(frame_time);
			direction = AEWrap(direction, -PI, PI);
		}

		if (AEInputCheckCurr(AEVK_D)) {
			direction -= rot_speed * speed_upgrade * static_cast<f32>(frame_time);
			direction = AEWrap(direction, -PI, PI);
		}

		if (AEVec2Distance(&current_planet.position, &position) <= (current_planet.size / 2 + dist_from_planet)) {
			direction = atan2(position.y - current_planet.position.y, position.x - current_planet.position.x);
			state = PLAYER_ORBIT;
		}
	}

	// ======================================================
	// update physics of all active game object instances
	// 
	//	-- Positions of the instances are updated here with the already computed velocity (above)
	// ======================================================

	// update position
	position.x = position.x + velocity.x * static_cast<f32>(frame_time);
	position.y = position.y + velocity.y * static_cast<f32>(frame_time);

	// ====================
	// check for collision
	// ====================


	// ===================================
	// update active game object instances
	// Example:
	//		-- Wrap specific object instances around the world (Needed for the assignment)
	//		-- Removing the bullets as they go out of bounds (Needed for the assignment)
	//		-- If you have a homing missile for example, compute its new orientation 
	//			(Homing missiles are not required for the Asteroids project)
	//		-- Update a particle effect (Not required for the Asteroids project)
	// ===================================


	// =====================================
	// calculate the matrix for all objects
	// =====================================




	

	// camera stuff lol
	/*
	if (AEInputCheckCurr(AEVK_F)) AEGfxSetCamPosition(player.current_planet.position.x, player.current_planet.position.y);
	else AEGfxSetCamPosition(0, 0);
	*/

	// if spacebar, draw tractor beam
	if (AEInputCheckCurr(AEVK_SPACE) && state == PLAYER_ORBIT) {
		beam_pos.x = position.x - AECos(direction) * 20;
		beam_pos.y = position.y - AESin(direction) * 20;

		// Space between planet and debris to change to proper variable
		collision_x = beam_pos.x - AECos(direction) * 10;
		collision_y = beam_pos.y - AESin(direction) * 10;

		AEVec2 coll_pos;
		coll_pos.x = collision_x;
		coll_pos.y = collision_y;

		if (debris_vector_all[current_planet.id].size() > 0)
		{
			for (int i = 0; i < debris_vector_all[current_planet.id].size(); ++i) {
				if (current_capacity < max_capacity && AEVec2Distance(&coll_pos, &debris_vector_all[current_planet.id][i].position) <= 10) {
					debris_vector_all[current_planet.id].erase(debris_vector_all[current_planet.id].begin() + i);
					current_capacity++;
					break;
				}
			}
		}
	}
}

void Player::draw(AEGfxVertexList* pMesh)
{
	AEGfxTextureSet(player_tex, 0, 0);

	AEMtx33 scale = { 0 };
	AEMtx33Scale(&scale, size, size);

	AEMtx33 rotate = { 0 };
	AEMtx33Rot(&rotate, direction + PI / 2);

	AEMtx33 translate = { 0 };
	AEMtx33Trans(&translate, position.x, position.y);

	AEMtx33 transform = { 0 };
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);

	AEGfxSetTransform(transform.m);

	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

	// tractor beam
	if (AEInputCheckCurr(AEVK_SPACE) && state == PLAYER_ORBIT) {
		AEGfxTextureSet(tractor_beam_tex, 0, 0);
		AEMtx33Scale(&scale, size, size * 3 / 2);
		AEMtx33Rot(&rotate, direction + PI / 2);
		AEMtx33Trans(&translate, beam_pos.x, beam_pos.y);
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	}
}

void Player::free()
{

}

void Player::unload()
{
	AEGfxTextureUnload(player_tex);
	AEGfxTextureUnload(tractor_beam_tex);
}