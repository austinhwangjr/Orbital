#include "AEEngine.h"
#include "SpaceStation.h"
#include <vector>
#include "Debris.h"
#include <string>
#include "Camera.h"

//VARIABLES
#define COOLDOWN_HEIGHT 5;
#define COOLDOWN_WIDTH 0;
#define COOLDOWN_TIME 3;
#define COIN_HEIGHT 50;
#define COIN_WIDTH 50;


int BUFFER_SAFE_DISTANCE = 30;

// Textures
extern AEGfxTexture* space_station_tex;

// Variables
bool space_station_valid_placement = false;
bool space_station_added;

// Mouse coordinates
extern AEVec2 mouse_pos_world;

// Vector of space stations, planets and debris
std::vector<SpaceStation> space_station_vector;
extern std::vector<Planets> planet_vector;
extern std::vector<std::vector<Debris>> debris_vector_all;

// Variables to print
std::string capacity_spacestation;
extern s8 font_id;
extern Camera camera;

extern AEGfxTexture* shop_icon_tex;

// Vector of cooldown bar
std::vector<Cooldown_Bar> cooldown_vector;


//Coin Sprite
AEGfxTexture* coin_tex;
std::vector<Coin> coin_vector;



void SpaceStation::load()
{
	coin_tex = AEGfxTextureLoad("Assets/Coin.png");
}

void SpaceStation::init()
{
	position.x			= 0.f;
	position.y			= 0.f;
	size				= 100.0f;
	current_capacity	= 0;
	max_capacity		= 10;
	initial_spawn		= false;
}

void SpaceStation::update(f64 frame_time, Player& player, PlayerUI& player_ui)
{
	int safe_position = 0;
	double radius_to_debris = 0;

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
		position.x = mouse_pos_world.x;
		position.y = mouse_pos_world.y;

		double radius_to_debris = 0;
		double radius_to_station = size;
		double radius_x;
		double radius_y;
		

		// =================
		// CALCULATE RADIUS
		// =================
		for (int i = 0; i < planet_vector.size(); i++) {
			for (int j = 0; j < debris_vector_all[i].size(); j++) {
				radius_x = pow((debris_vector_all[i][j].position.x - planet_vector[i].position.x), 2);
				radius_y = pow((debris_vector_all[i][j].position.y - planet_vector[i].position.y), 2);
				radius_to_debris = sqrt((radius_x + radius_y));
			}

		}

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
		if ((AEVec2Distance(&current_planet.position, &position) > radius_to_debris + BUFFER_SAFE_DISTANCE) && safe_position == space_station_vector.size()) {
			if (player.space_station_count < MAX_SPACE_STATION_CNT) {
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
		if (space_station_valid_placement && !space_station_added && player.credits >= player_ui.space_station_cost) {
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

			player.credits -= player_ui.space_station_cost;
			player.space_station_count++;
			space_station_added = true;
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
			
			cooldown_vector[i].timer += static_cast<f32>(frame_time);
			
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
				player.credits += DEBRIS_VALUE;
				player.score += DEBRIS_SCORE;
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
		capacity_spacestation = "Station Capacity: " + std::to_string(space_station_vector[i].current_capacity) + " / " + std::to_string(space_station_vector[i].max_capacity);
		AEGfxPrint(font_id, const_cast<s8*>(capacity_spacestation.c_str()),
			2 * (pos.x - 80) / AEGetWindowWidth(),
			2 * (pos.y + 50) / AEGetWindowHeight(), 0.3f, 1.f, 1.f, 1.f);
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
}

void SpaceStation::unload()
{
	AEGfxTextureUnload(coin_tex);
}