#include "AEEngine.h"
#include "SpaceStation.h"
#include <vector>

// Textures
extern AEGfxTexture* space_station_tex;

// Variables
bool space_station_valid_placement = false;
bool space_station_added;

// Mouse coordinates
extern AEVec2 mouse_pos_world;

// Vector of space stations
std::vector<SpaceStation> space_station_vector;

void SpaceStation::load()
{

}

void SpaceStation::init()
{
	position.x			= 0.f;
	position.y			= 0.f;
	size				= 32.0f;
	current_capacity	= 0;
	max_capacity		= 10;
}

void SpaceStation::update(f64 frame_time, PlayerUI& player_ui)
{
	// =========================
	// Update according to input
	// =========================
	
	// if no longer placing space station
	if (!AEInputCheckCurr(AEVK_LBUTTON)) {
		player_ui.placing_station = false;
	}

	// =================================
	// Update position of space station
	// =================================
	
	// if placing space station
	if (player_ui.placing_station) {
		space_station_valid_placement = true;
		space_station_added = false;

		// drone follow mouse pos
		position.x = mouse_pos_world.x;
		position.y = mouse_pos_world.y;
	}

	// ===============================
	// Update space station instances
	// ===============================

	// If no longer placing space station
	if (!player_ui.placing_station) {
		if (space_station_valid_placement && !space_station_added) {
			space_station_vector.push_back(*this);
			space_station_added = true;
		}
	}

	for (int i = 0; i < space_station_vector.size(); ++i) {
		SpaceStation& space_station = space_station_vector[i];
	}

	// =======================================
	// calculate the matrix for space station
	// =======================================

	AEMtx33 scale, rot, trans;

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
	// For UI
	if (player_ui.placing_station) {
		AEGfxTextureSet(space_station_tex, 0, 0);
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	}

	// For active space stations
	for (int i = 0; i < space_station_vector.size(); ++i) {

		SpaceStation& space_station = space_station_vector[i];

		AEGfxSetTintColor(1.f, 1.f, 1.f, 1.f);

		AEGfxTextureSet(space_station_tex, 0, 0);
		AEGfxSetTransform(space_station.transform.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	}
}

void SpaceStation::free()
{

}

void SpaceStation::unload()
{
	
}