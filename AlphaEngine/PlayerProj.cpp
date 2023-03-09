#include "AEEngine.h"
#include "PlayerProj.h"
#include <vector>
#include "SpaceStation.h"

// Textures
AEGfxTexture* player_proj_tex;

// Mouse coordinates
extern AEVec2 mouse_pos_world;

// Vector of projectiles, space stations and shop buttons
std::vector<PlayerProj> player_proj_vector;
extern std::vector<SpaceStation> space_station_vector;
extern std::vector<ShopOption> button_vector;

extern f32 cam_x, cam_y;



void PlayerProj::load()
{
	player_proj_tex = AEGfxTextureLoad("Assets/Debris.png");
}

void PlayerProj::init()
{
	position.x		= 0.f;
	position.y		= 0.f;

	velocity.x		= 0.f;
	velocity.y		= 0.f;

	size			= 30.f;

	speed			= 150.f;

	direction		= 0.f;

	is_delete = 0;
}

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
	// check for collision
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



	// =======================================
	// Delete projectile if go out of screen
	// =======================================
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

	// =======================================
	// calculate the matrix for space station
	// =======================================

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
	Draw space station(s)
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

void PlayerProj::free()
{
	player_proj_vector.clear();
}

void PlayerProj::unload()
{
	AEGfxTextureUnload(player_proj_tex);
}