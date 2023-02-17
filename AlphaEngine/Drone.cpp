#include "AEEngine.h"
#include "Drone.h"
#include <iostream>
#include <vector>

// textures (temp)
extern AEGfxTexture* player_tex;
extern AEGfxTexture* tractor_beam_tex;

//Drone drone;

f32 drone_collision_x;
f32 drone_collision_y;

//bool		placing_drone = false;
//bool		placing_station = false;

bool		valid_placement = false;
bool		drone_added;

extern f32 mouse_x_actual, mouse_y_actual;

// drone, planet and debris vectors
std::vector<Drone> drone_vector;
extern std::vector<Planets> planet_vector;
extern std::vector<std::vector<Debris>> debris_vector_all;

void Drone::load()
{

}

void Drone::init(Player player)
{
	position.x			= 0.f;
	position.y			= 0.f;
	velocity.x			= 0.f;
	velocity.y			= 0.f;
	size				= player.size;
	dist_from_planet	= player.dist_from_planet;
	shortest_distance	= 0.f;
	rot_speed			= player.rot_speed / 2;
	direction			= player.direction;
	//beam_str;
	//beam_width;
	current_capacity	= 0;
	max_capacity		= 10;

	// tractor beam
	beam_pos.x			= 0.f;
	beam_pos.y			= 0.f;

	drone_collision_x	= 0.f;
	drone_collision_y	= 0.f;
}

void Drone::update(f64 frame_time, Player player, PlayerUI& player_ui)
{
	// if no longer placing drone
	if (!AEInputCheckCurr(AEVK_LBUTTON)) {
		player_ui.placing_drone = false;
	}

	// if placing drone
	if (player_ui.placing_drone) {
		drone_added = false;

		// drone follow mouse pos
		position.x = mouse_x_actual;
		position.y = mouse_y_actual;

		// determine planet closest to drone
		current_planet = planet_vector[0];
		shortest_distance = AEVec2Distance(&position, &planet_vector[0].position);

		for (size_t i = 1; i < planet_vector.size(); ++i) {
			if (AEVec2Distance(&position, &planet_vector[i].position) < shortest_distance) {
				current_planet = planet_vector[i];
				shortest_distance = AEVec2Distance(&position, &planet_vector[i].position);
			}
		}

		// if within planet's orbit, drone can be placed
		if (AEVec2Distance(&current_planet.position, &position) <= (current_planet.size / 2 + dist_from_planet)) {
			direction = atan2(position.y - current_planet.position.y, position.x - current_planet.position.x);
			position.x = current_planet.position.x + (current_planet.size / 2 + dist_from_planet) * AECos(direction);
			position.y = current_planet.position.y + (current_planet.size / 2 + dist_from_planet) * AESin(direction);

			valid_placement = true;
		}
		else
			valid_placement = false;
	}

	else if (!player_ui.placing_drone) {
		if (valid_placement && drone_added == false) {
			drone_vector.push_back(*this);
			drone_added = true;
		}
	}

	if (drone_vector.size() > 0) {
		for (int j = 0; j < drone_vector.size(); ++j) {
			Drone& drone = drone_vector[j];

			drone.position.x = drone.current_planet.position.x +
				(drone.current_planet.size / 2 + drone.dist_from_planet) * AECos(drone.direction);
			drone.position.y = drone.current_planet.position.y +
				(drone.current_planet.size / 2 + drone.dist_from_planet) * AESin(drone.direction);

			drone.direction += drone.rot_speed * static_cast<f32>(frame_time);

			// Drone's tractor beam
			drone.beam_pos.x = drone.position.x - AECos(drone.direction) * 20;
			drone.beam_pos.y = drone.position.y - AESin(drone.direction) * 20;

			// Space between planet and debris to change to proper variable
			drone_collision_x = drone.beam_pos.x - AECos(drone.direction) * 10;
			drone_collision_y = drone.beam_pos.y - AESin(drone.direction) * 10;

			AEVec2 coll_pos;
			coll_pos.x = drone_collision_x;
			coll_pos.y = drone_collision_y;

			if (debris_vector_all[current_planet.id].size() > 0)
			{
				for (int i = 0; i < debris_vector_all[current_planet.id].size(); ++i) {
					if (drone.current_capacity < drone.max_capacity && AEVec2Distance(&coll_pos, &debris_vector_all[current_planet.id][i].position) <= 10) {
						debris_vector_all[current_planet.id].erase(debris_vector_all[drone.current_planet.id].begin() + i);
						drone.current_capacity++;
						break;
					}
				}
			}
		}
	}
}

void Drone::draw(AEGfxVertexList* pMesh, PlayerUI player_ui)
{
	AEMtx33 scale{}, rotate{}, translate{}, transform{};

	// For UI
	if (player_ui.placing_drone) {

		if (valid_placement)
			AEGfxSetTintColor(1.f, 0.8f, 0.f, 1.f);
		else
			AEGfxSetTintColor(0.8f, 0.f, 0.f, 1.f);


		AEGfxTextureSet(player_tex, 0, 0);
		AEMtx33Scale(&scale, size, size);
		AEMtx33Rot(&rotate, direction + PI / 2);
		AEMtx33Trans(&translate, position.x, position.y);
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	}

	for (int i = 0; i < drone_vector.size(); ++i) {
		
		Drone& drone = drone_vector[i];

		AEGfxSetTintColor(1.f, 1.f, 1.f, 1.f);

		AEGfxTextureSet(player_tex, 0, 0);
		AEMtx33Scale(&scale, drone.size, drone.size);
		AEMtx33Rot(&rotate, drone.direction + PI / 2);
		AEMtx33Trans(&translate, drone.position.x, drone.position.y);
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

		// tractor beam
		AEGfxTextureSet(tractor_beam_tex, 0, 0);
		AEMtx33Scale(&scale, drone.size, drone.size * 3 / 2);
		AEMtx33Rot(&rotate, drone.direction + PI / 2);
		AEMtx33Trans(&translate, drone.beam_pos.x, drone.beam_pos.y);
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	}
	//AEMtx33 scale{}, rotate{}, translate{}, transform{};

	//AEGfxSetTintColor(1.f, 1.f, 1.f, 1.f);

	//if (active && player_ui.placing_drone == false) {

	//	AEGfxSetTintColor(1.f, 1.f, 1.f, 1.f);

	//	AEGfxTextureSet(player_tex, 0, 0);
	//	AEMtx33Scale(&scale, size, size);
	//	AEMtx33Rot(&rotate, direction + PI / 2);
	//	AEMtx33Trans(&translate, position.x, position.y);
	//	AEMtx33Concat(&transform, &rotate, &scale);
	//	AEMtx33Concat(&transform, &translate, &transform);
	//	AEGfxSetTransform(transform.m);
	//	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	//}

	//// draw drone when it is being placed/active
	//if (active || player_ui.placing_drone) {

	//	if (valid_placement)
	//		AEGfxSetTintColor(1.f, 0.8f, 0.f, 1.f);
	//	else
	//		AEGfxSetTintColor(0.8f, 0.f, 0.f, 1.f);

	//	AEGfxTextureSet(player_tex, 0, 0);
	//	AEMtx33Scale(&scale, size, size);
	//	AEMtx33Rot(&rotate, direction + PI / 2);
	//	AEMtx33Trans(&translate, position.x, position.y);
	//	AEMtx33Concat(&transform, &rotate, &scale);
	//	AEMtx33Concat(&transform, &translate, &transform);
	//	AEGfxSetTransform(transform.m);
	//	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

	//	// tractor beam
	//	AEGfxTextureSet(tractor_beam_tex, 0, 0);
	//	AEMtx33Scale(&scale, size, size * 3 / 2);
	//	AEMtx33Rot(&rotate, direction + PI / 2);
	//	AEMtx33Trans(&translate, beam_pos.x, beam_pos.y);
	//	AEMtx33Concat(&transform, &rotate, &scale);
	//	AEMtx33Concat(&transform, &translate, &transform);
	//	AEGfxSetTransform(transform.m);
	//	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	//}
}

void Drone::free()
{

}

void Drone::unload()
{

}