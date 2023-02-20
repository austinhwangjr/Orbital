#include "AEEngine.h"
#include "PlayerProj.h"
#include <vector>

// Textures
AEGfxTexture* player_proj_tex;

// Variables
//bool space_station_valid_placement = false;
//bool space_station_added;

// Mouse coordinates
extern AEVec2 mouse_pos_world;

// Vector of space stations
std::vector<PlayerProj> player_proj_vector;

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
}

void PlayerProj::update(f64 frame_time, Player& player)
{
	// =========================
	// Update according to input
	// =========================

	if (AEInputCheckTriggered(AEVK_LBUTTON)) {

		position = player.position;
		AEVec2Sub(&velocity, &mouse_pos_world, &player.position);
		AEVec2Normalize(&velocity, &velocity);
		AEVec2Scale(&velocity, &velocity, speed);

		player_proj_vector.push_back(*this);
	}

	// =====================================
	// Update position of player projectile
	// =====================================

	for (int i = 0; i < player_proj_vector.size(); ++i) {
		PlayerProj& player_proj = player_proj_vector[i];

		player_proj.position.x += player_proj.velocity.x * frame_time;
		player_proj.position.y += player_proj.velocity.y * frame_time;
	}

	// ====================
	// check for collision
	// ====================



	// ===================================
	// Update player projectile instances
	// ===================================

	

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

}

void PlayerProj::unload()
{
	AEGfxTextureUnload(player_proj_tex);
}