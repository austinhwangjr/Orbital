#include "AEEngine.h"
#include "SpaceStation.h"
#include <vector>
#include "Debris.h"
#include <string>
#include "Camera.h"

// Textures
extern AEGfxTexture* space_station_tex;

// Variables
bool space_station_valid_placement = false;
bool space_station_added;

// Mouse coordinates
extern AEVec2 mouse_pos_world;

// Vector of space stations
std::vector<SpaceStation> space_station_vector;

// Vector of planets
extern std::vector<Planets> planet_vector;

//Vector of Debris
extern std::vector<std::vector<Debris>> debris_vector_all;

// Variables to print
std::string capacity_spacestation;
extern s8		font_id;
extern Camera camera;

extern AEGfxTexture* shop_icon_tex;

// Vector of cooldown bar
std::vector<Cooldown_Bar> cooldown_vector;


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

void SpaceStation::update(f64 frame_time, Player& player, PlayerUI& player_ui)
{
	int safe_position = 0;
	

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
		// drone follow mouse pos
		position.x = mouse_pos_world.x;
		position.y = mouse_pos_world.y;

		double radius_to_debris = 0;
		double radius_to_station = size;
		double radius_x;
		double radius_y;
		// add if condition check for collision to make it true
		for (int i = 0; i < planet_vector.size(); i++) {
			for (int j = 0; j < debris_vector_all[i].size(); j++) {
				radius_x = pow((debris_vector_all[i][j].position.x - planet_vector[i].position.x), 2);
				radius_y = pow((debris_vector_all[i][j].position.y - planet_vector[i].position.y), 2);
				radius_to_debris = sqrt((radius_x + radius_y));
			}

		}

		for (int k = 0; k < space_station_vector.size(); ++k) {
			if (AEVec2Distance(&position, &space_station_vector[k].position) >= size * 5) {
				safe_position += 1;
			}
		}

		if ((AEVec2Distance(&current_planet.position, &position) > radius_to_debris + 15) && safe_position == space_station_vector.size()) {

			space_station_valid_placement = true;
			space_station_added = false;

		}
		else {
			space_station_valid_placement = false;
		}
	}

	// ===============================
	// Update space station instances
	// ===============================

	// If no longer placing space station
	if (!player_ui.placing_station) {
		if (space_station_valid_placement && !space_station_added && player.credits >= player_ui.space_station_cost) {
			// Add space station to vector
			space_station_vector.push_back(*this);

			Cooldown_Bar cooldown;
			cooldown.height = 5;
			cooldown.width = 0;
			cooldown.position.x = this->position.x;
			cooldown.position.y = this->position.y;
			cooldown.timer = 0;
			cooldown.total_time = 3;
			cooldown_vector.push_back(cooldown);
			player.credits -= player_ui.space_station_cost;
			space_station_added = true;
		}
	}
	else {
		current_planet = planet_vector[0];
		shortest_distance_spacestation = AEVec2Distance(&position, &planet_vector[0].position);

		for (size_t i = 1; i < planet_vector.size(); ++i) {
			if (AEVec2Distance(&position, &planet_vector[i].position) < shortest_distance_spacestation) {
				current_planet = planet_vector[i];
				shortest_distance_spacestation = AEVec2Distance(&position, &planet_vector[i].position);
			}
		}
	}



	for (int i = 0; i < space_station_vector.size(); ++i) {
		SpaceStation& space_station = space_station_vector[i];
	}

	
	// =================================
	// Process of space station (Cooldown Bar)
	// =================================
	for (int i = 0; i < cooldown_vector.size(); ++i) {
		if (space_station_vector[i].current_capacity != 0) {
			cooldown_vector[i].timer += frame_time;
			

			f32 speed = 80 / cooldown_vector[i].total_time;
			cooldown_vector[i].width = cooldown_vector[i].timer * speed; 


			if (cooldown_vector[i].timer > 3) {
				space_station_vector[i].current_capacity--;
				player.credits += 100;
				cooldown_vector[i].timer = 0;
			}
		}
		else {
			cooldown_vector[i].width = 0;
		}
	}





	// =======================================
	// calculate the matrix for space station
	// =======================================

	AEMtx33 scale, rot, trans;

	// For cooldown Bar
	for (int i = 0; i < cooldown_vector.size(); ++i) {
		Cooldown_Bar& cooldown = cooldown_vector[i];
		// cooldown
		AEMtx33Scale(&scale, cooldown.width, cooldown.height); 
		AEMtx33Rot(&rot, 0);
		AEMtx33Trans(&trans, cooldown.position.x, cooldown.position.y - 35);
		AEMtx33Concat(&cooldown.transform, &rot, &scale);
		AEMtx33Concat(&cooldown.transform, &trans, &cooldown.transform);
	}

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
		capacity_spacestation = "Station Capacity: " + std::to_string(space_station_vector[i].current_capacity) + " / " + std::to_string(space_station_vector[i].max_capacity);
		AEGfxPrint(font_id, const_cast<s8*>(capacity_spacestation.c_str()),
			2 * (pos.x - 80) / AEGetWindowWidth(),
			2 * (pos.y + 25) / AEGetWindowHeight(), 0.3f, 1.f, 1.f, 1.f);
	}


	// For active vector
	for (int i = 0; i < cooldown_vector.size(); ++i) {

		Cooldown_Bar& cooldown = cooldown_vector[i];

		AEGfxSetTintColor(1.f, 1.f, 1.f, 1.f);

		AEGfxTextureSet(shop_icon_tex, 0, 0);
		AEGfxSetTransform(cooldown.transform.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	}

	// For UI
	if (player_ui.placing_station) {

		if (space_station_valid_placement)
			AEGfxSetTintColor(0.f, 1.f, 0.f, 1.f);
		else
			AEGfxSetTintColor(1.f, 0.3f, 0.3f, 1.f);

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