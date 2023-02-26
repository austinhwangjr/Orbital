#include "AEEngine.h"
#include "Drone.h"
#include <vector>

// Textures
extern AEGfxTexture* player_tex;
extern AEGfxTexture* tractor_beam_tex;

// Variables
bool drone_valid_placement = false;
bool drone_added;

// Mouse coordinates
extern AEVec2 mouse_pos_world;

// Vectors of drones, planets and debris
std::vector<Drone> drone_vector;
extern std::vector<Planets> planet_vector;
extern std::vector<std::vector<Debris>> debris_vector_all;

void Drone::load()
{

}

void Drone::init(Player player)
{
	// Drone
	position.x				= 0.f;
	position.y				= 0.f;

	velocity.x				= 0.f;
	velocity.y				= 0.f;

	size					= player.size;

	rot_speed				= player.rot_speed / 8.f;

	dist_from_planet		= player.dist_from_planet;
	shortest_distance		= 0.f;

	direction				= player.direction;

	current_capacity		= 0;
	max_capacity			= 3;

	// Tractor beam
	beam_pos.x				= 0.f;
	beam_pos.y				= 0.f;

	beam_width				= size;
	beam_height				= beam_width * 3 / 2;
}

void Drone::update(f64 frame_time, Player& player, PlayerUI& player_ui)
{
	// =========================
	// Update according to input
	// =========================
	
	// If no longer placing drone
	if (!AEInputCheckCurr(AEVK_LBUTTON)) {
		player_ui.placing_drone = false;
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
		if (AEVec2Distance(&current_planet.position, &position) <= (static_cast<f32>(current_planet.size) / 2.f + dist_from_planet)) {
			if (current_planet.current_drones < DRONES_MAX) {
				direction = static_cast<f32>(atan2(static_cast<double>(position.y - current_planet.position.y),
					static_cast<double>(position.x - current_planet.position.x)));
				position.x = current_planet.position.x + (static_cast<f32>(current_planet.size) / 2.f + dist_from_planet) * AECos(direction);
				position.y = current_planet.position.y + (static_cast<f32>(current_planet.size) / 2.f + dist_from_planet) * AESin(direction);
				beam_pos.x = position.x - AECos(direction) * 20;
				beam_pos.y = position.y - AESin(direction) * 20;

				drone_valid_placement = true;
			}
			else
				drone_valid_placement = false;
		}
		else
			drone_valid_placement = false;
	}

	// Each active drone to orbit around planet
	for (int i = 0; i < drone_vector.size(); ++i) {
		Drone& drone = drone_vector[i];

		drone.position.x = drone.current_planet.position.x +
			(static_cast<f32>(drone.current_planet.size) / 2.f + drone.dist_from_planet) * AECos(drone.direction);
		drone.position.y = drone.current_planet.position.y +
			(static_cast<f32>(drone.current_planet.size) / 2.f + drone.dist_from_planet) * AESin(drone.direction);

		drone.direction += drone.rot_speed * static_cast<f32>(frame_time);

		// Drone's tractor beam
		drone.beam_pos.x = drone.position.x - AECos(drone.direction) * (drone.beam_height * 2 / 3);
		drone.beam_pos.y = drone.position.y - AESin(drone.direction) * (drone.beam_height * 2 / 3);
	}

	// ================================
	// check for beam-debris collision
	// ================================

	for (int i = 0; i < drone_vector.size(); ++i) {
		Drone& drone = drone_vector[i];

		if (debris_vector_all[drone.current_planet.id].size() > 0) {
			for (int i = 0; i < debris_vector_all[drone.current_planet.id].size(); ++i) {
				Debris& debris = debris_vector_all[current_planet.id][i];
				// Debris to move towards drone when in contact with beam
				if (drone.current_capacity < drone.max_capacity && 
					AEVec2Distance(&drone.beam_pos, &debris.position) <= drone.beam_height / 2) {

					debris_vector_all[current_planet.id].erase(debris_vector_all[drone.current_planet.id].begin() + i);
					drone.current_capacity++;
					break;
					//debris.move_towards_drone = true;
					/*debris.orbit_around_planet = false;
					break;*/
				}
				//else
				//	//debris.move_towards_drone = false;

				//if (AEVec2Distance(&drone.position, &debris.position) <= (drone.size + debris.size) / 2) {
				//	// Debris to be destroyed when in contact with drone
				//	debris.to_erase = true;
				//	break;
				//}
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
		for (size_t i = 0; i < planet_vector.size(); ++i) {
			if (planet_vector[i].id == current_planet.id) {
				planet_vector[i].current_drones++;
				drone_vector.push_back(*this);
				player.credits -= player_ui.drone_cost;
				drone_added = true;
			}
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
	for (int i = 0; i < drone_vector.size(); ++i) {
		Drone& drone = drone_vector[i];
		
		// Drone
		AEMtx33Scale(&scale, drone.size, drone.size);
		AEMtx33Rot(&rot, drone.direction + PI / 2);
		AEMtx33Trans(&trans, drone.position.x, drone.position.y);
		AEMtx33Concat(&drone.drone_transform, &rot, &scale);
		AEMtx33Concat(&drone.drone_transform, &trans, &drone.drone_transform);

		// Tractor beam
		AEMtx33Scale(&scale, drone.size, drone.size * 3 / 2);
		AEMtx33Rot(&rot, drone.direction + PI / 2);
		AEMtx33Trans(&trans, drone.beam_pos.x, drone.beam_pos.y);
		AEMtx33Concat(&drone.beam_transform, &rot, &scale);
		AEMtx33Concat(&drone.beam_transform, &trans, &drone.beam_transform);
	}

	// ===========================
	// Remove debris to be erased
	// ===========================
	for (int i = 0; i < debris_vector_all[current_planet.id].size(); ++i) {
		if (debris_vector_all[current_planet.id][i].to_erase) {
			debris_vector_all[current_planet.id].erase(debris_vector_all[current_planet.id].begin() + i);
			current_capacity++;
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

		AEGfxTextureSet(player_tex, 0, 0);
		AEGfxSetTransform(drone_transform.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	}

	// For active drones
	for (int i = 0; i < drone_vector.size(); ++i) {
		
		Drone& drone = drone_vector[i];

		AEGfxSetTintColor(1.f, 1.f, 1.f, 1.f);

		// Drone
		AEGfxTextureSet(player_tex, 0, 0);
		AEGfxSetTransform(drone.drone_transform.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

		// Tractor beam
		AEGfxTextureSet(tractor_beam_tex, 0, 0);
		AEGfxSetTransform(drone.beam_transform.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	}
}

void Drone::free()
{
	drone_vector.clear();
}

void Drone::unload()
{

}